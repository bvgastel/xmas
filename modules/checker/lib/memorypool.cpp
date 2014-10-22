#ifdef MEMORY_DEBUG
#include "memorypool.debug.cpp"
#else

#include "memorypool.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <assert.h>
#include <string.h>
#include <limits.h>

#include "spinlock.h"

#ifdef __linux__
#include <malloc.h>
#elif defined(__APPLE__)
#include <malloc/malloc.h>
#else
#include <malloc_np.h> // FreeBSD
#endif

//#define NO_MEMORYPOOL_ALIGNMENT
//#define EXTRA_SAFE

// FIXME: for MemoryPoolPage, use CacheInstances, instead of own implementation

/*
 * TODO:
 * - reserve(int chunks, int maxSizeOfItem): make users of reserve specify more clearly what they need
 * - on a large allocation, allocate a power of two and returnUnused() the remaining memory
 */

namespace bitpowder {
namespace lib {

const size_t _MemoryPoolPageSize = getpagesize();

SpinLock newPoolLock;
Stack<MemoryPoolPage*,MemoryPoolPage,&MemoryPoolPage::prev> newPool;

void MemoryPool::restore(MemoryPoolStatus status) {
    //checkAssert(this);
    MemoryPoolPage *currentMemoryPool = current;
    Deque<MemoryPoolPage*> currentAll;
    currentAll.swap(all);
    current = nullptr;
    nested = status.nested;
    // PASS 1, execute delayed items
    // pools added after the call to status()
    MemoryPoolPage *c = currentMemoryPool;
    while (c && c != status.current) {
        for (auto it : c->objects)
            (it++)->execute();
        c = currentAll.prev(c);
    }
    // also perform it for the newly added delayed items in the pool that was current when status() was called
    if (c != nullptr) {
        checkAssert(c == status.current);
        while (c->objects.back() != status.objects)
            c->objects.pop_back()->execute();
    }
    // PASS 2, dispose of pages (not earlier, because a delay function of an 'earlier' memorypool can depend on memory of a 'later' memorypool...)
    Queue<MemoryPoolPage*,MemoryPoolPage,&MemoryPoolPage::prev> readd;
    Deque<MemoryPoolPage*> toNextPages;
    while (currentMemoryPool && currentMemoryPool != status.current) {
        MemoryPoolPage *prev = currentAll.prev(currentMemoryPool);
        currentAll.erase(currentMemoryPool);
        if (currentMemoryPool->allocatedFromPool) {
            currentMemoryPool->reset();
            readd.push(currentMemoryPool);
        } else
            toNextPages.push_front(currentMemoryPool);
        //assert(mp->current == NULL);
        currentMemoryPool = prev;
    }
    checkAssert(currentMemoryPool == status.current);

    // add all the memorypools to be recycled in one operation to the pool
    if (!readd.empty()) {
        SpinLock::Locker l(newPoolLock);
        newPool.splice(readd);
    }

    currentAll.insert(currentAll.next(currentMemoryPool), std::move(toNextPages));

    // rewind nextPages, as these are never allocated from the global pool, we don't have to readded them
    while (currentAll.back() && currentAll.back() != status.nextPage && currentAll.back() != currentMemoryPool) {
        currentAll.pop_back();
    }

    // reset available to previous value
    if (c != NULL)
        c->available = status.available;

    // PASS 3, if new memorypools are made during executing this function, append the old linked list to the last one of the new one
    if (all.empty()) {
        current = status.current;
    } else {
        // do not modifiy 'current'!
        Deque<MemoryPoolPage*> active = all.split(current); // split the part currently in use
        currentAll.insert(currentAll.next(currentMemoryPool), std::move(active));
        currentAll.push_back(all);
    }
    all.swap(currentAll);
}

MemoryPoolStatus MemoryPool::status() {
    MemoryPoolStatus retval(current, all.back());
    if (retval.current) {
        retval.available = retval.current->available;
        retval.objects = retval.current->objects.back();
        retval.nested = nested++;
    }
    return retval;
}

MemoryPoolPage* MemoryPool::initMemory(void *data, size_t _size) {
    //MemoryPoolPage *pool = (MemoryPoolPage*)data;
    return new (data) MemoryPoolPage(_size);
}

void MemoryPool::addMemory(void *data, size_t _size) {
    if (data == NULL)
        return;

#ifndef NO_MEMORYPOOL_ALIGNMENT
    // make multiple of alignment
    size_t extra = ((long)data) & (ALIGN_ON - 1);
    if (extra) {
        data = &((char*)data)[ALIGN_ON-extra];
        _size -= (ALIGN_ON-extra);
    }
#endif

    if (data == NULL || _size <= sizeof(MemoryPoolPage)) // || size > USHRT_MAX /* length of short */)
        return;
#ifdef EXTRA_SAFE
    memset(data, 0, _size - sizeof(MemoryPoolPage));
#endif
    MemoryPoolPage *page = initMemory(data, _size);
    all.push_back(page);
    if (!current)
        current = page;
}

void MemoryPool::addMemory(MemoryPool *mp) {
    if (mp) {
        size_t size = 0;
        void *data = mp->allocAll(size);
        if (data && size > 0) {
            //fprintf(stderr, "added %i bytes of memory to MemoryPool\n", size);
            addMemory(data, size);
        }
    }
}

// allocs new pool
MemoryPoolPage* MemoryPool::allocExtra(size_t newSize) {
    if (current && all.next(current)) {
        current = all.next(current);
        return current;
    }
    MemoryPoolPage *pool = NULL;
    size_t size = 0;
    // try to fetch one from the cache
    if (pool == NULL) {
        SpinLock::Locker l(newPoolLock);
        pool = newPool.pop_back();
    }
    // if not found alloc new one
    if (pool == NULL) {
        pool = (MemoryPoolPage *)malloc(newSize);
        //checkAssert(pool);
        size = newSize;
        //fprintf(stderr, "alloc extra: %p\n", pool);
    } else {
        // sets the size to the one fetched from the cache
        size = pool->size;
    }
    // really add the memory to the pools
    if (pool != NULL) {
        initMemory(pool, size);
        pool->allocatedFromPool = true;
        all.insert(all.next(current), pool);
        current = pool;
    }
    return pool;
}

MemoryPool::MemoryPool() : MEMORY_STATISTICS_INIT current(nullptr), nested(1), all() {
}

MemoryPool::MemoryPool(void *data, size_t size) : MEMORY_STATISTICS_INIT current(nullptr), nested(1), all() {
    addMemory(data, size);
}

MemoryPool::~MemoryPool() {
    clear();
}

void MemoryPool::clear() {
    assert(this);
    if (current) {
        MemoryPoolStatus status;
        restore(status);
        assert(current == NULL);
        nested = 1;
    }
}

void* MemoryPool::alloc(size_t size, char alignOn, MemoryPoolPage **pool) {
    eprintf("alloc %li bytes", size);
#ifdef MEMORY_DEBUG
    if (pool != NULL)
        *pool = mp->current;
    return malloc(size);
#endif
    if (size == 0)
        return NULL;
    //checkAssert(this);

#ifndef NO_MEMORYPOOL_ALIGNMENT
    // make multiple of alignment
    size_t extra = size & (alignOn - 1);
    if (extra > 0) {
        size += alignOn - extra;
    }
#endif
    // do not facilitate large requests to the pool, except if there is (because of a super memorypool) room available
    if (size > (MEMORYPOOL_SIZE - sizeof(MemoryPoolPage)) && (current == NULL || size > current->available)) {
        //fprintf(stderr, "too large request: %i\n", size);;
        void *data = malloc(size);
        delay([=] {free(data);});
        if (pool != NULL)
            *pool = current;
        return data;
    }
    // search for pool with enough space
    MemoryPoolPage *c = current;
    while (c == NULL || size > c->available) {
        c = allocExtra();
    }
    checkAssert(c->available >= 0);
    checkAssert(size <= c->available);
    // alloc
    void* data = &((char*)c->data)[c->size - sizeof(MemoryPoolPage) - c->available];
    c->available -= size;
    checkAssert(c->available >= 0);
    // the following is slightly faster, but don't work in combination with mpprintf
    //current->available -= size;
    //void *data = &((char*)current->data)[current->available];
    if (pool != NULL)
        *pool = c;
#ifdef EXTRA_SAFE
    memset(data, 0, size);
#endif
    eprintf("alloc returned %p", data);
    return data;
}

void* MemoryPool::allocAll(size_t &size, char alignOn) {
    int requestedSize = current ? current->available : 0;
    void* retval = requestedSize == 0 ? nullptr : alloc(requestedSize, alignOn);
    if (retval)
        size = requestedSize;
    return retval;
}

void* MemoryPool::memdup(const void *src, size_t len, char alignOn) {
    if (src == NULL)
        return NULL;
    void* dst = alloc(len, alignOn);
    if (dst)
        memcpy(dst, src, len);
    return dst;
}

char* MemoryPool::strndup(const char *src, size_t len) {
    if (src == NULL)
        return NULL;
    char* dst = (char*)alloc(len+1, sizeof(char));
    if (dst) {
        memcpy(dst, src, len);
        dst[len] = '\0';
    }
    return dst;
}

char* MemoryPool::strdup(const char *src) {
    if (src == NULL)
        return NULL;
    //return mpmemdup(src, strlen(src)+1);
    return strndup(src, strlen(src));
}

int MemoryPool::vprintf(char **strPtr, size_t max, const char *format, va_list ap) {
    if (max == 0)
        return -1;
    MemoryPoolPage *p = NULL;
    int size = 0;
    char* str = (char*)alloc(max, sizeof(char), &p);
    if (str) {
        //check(str != NULL);
        //check(p != NULL);
        size = vsnprintf(str, max, format, ap);
        // check for error
        if (size < 0) {
            returnUnused(str, max);
            if (strPtr != NULL)
                *strPtr = NULL;
            return size;
        }
        // check if there was enough size in the string, otherwise for safety terminate string
        if ((size_t)size >= max) {
            str[max-1] = '\0';
        } else {
            // some space is left!
            returnUnused(str+size+1, max-size-1);
        }
    }
    if (strPtr != NULL)
        *strPtr = str;
    return size;
}

int MemoryPool::printf(char **strPtr, size_t max, const char *format, ...) {
    va_list ap;
    va_start(ap, format);
    int retval = vprintf(strPtr, max, format, ap);
    va_end(ap);
    return retval;
}

// not terminating the string, useful for generating byte strings
int MemoryPool::vprintf_nt(char **strPtr, size_t max, const char *format, va_list ap) {
    if (max == 0)
        return -1;
    MemoryPoolPage *p = NULL;
    char* str = (char*)alloc(max, sizeof(char), &p);
    //check(str != NULL);
    int size = 0;
    //check(p != NULL);
    if (str) {
        size = vsnprintf(str, max, format, ap);
        // check for error
        if (size < 0) {
            // check if pointer is really alloced inside memorypool
            returnUnused(str, max);
            if (strPtr != NULL)
                *strPtr = NULL;
            return size;
        }
        if (((size_t)size) < max) {
            // some space is left!
            returnUnused(str+size, max-size);
        }
    }
    if (strPtr != NULL)
        *strPtr = str;
    return size;
    /*
     slowww
    int size = vasprintf(strPtr, format, ap);
    delay(mp, free, *strPtr);
    return size;
     */
}

void MemoryPool::returnUnused(char *buffer, size_t len)
{
    if (buffer+len == next(sizeof(char)) && current)
        current->available += len;
    //else
    //    addMemory(buffer, len);
}

int MemoryPool::printf_nt(char **strPtr, size_t max, const char *format, ...) {
    va_list ap;
    va_start(ap, format);
    int retval = vprintf_nt(strPtr, max, format, ap);
    va_end(ap);
    return retval;
}

// clean up all remembered and unused pools
void MemoryPool::vacuum() {
    MemoryPoolPage *current = NULL;

    SpinLock::Locker l(newPoolLock);
    Stack<MemoryPoolPage*,MemoryPoolPage,&MemoryPoolPage::prev> pools(std::move(newPool));
    l.unlock();

    while ((current = pools.pop_back())) {
        //fprintf(stderr, "vacuuming: %p\n", current);
        assert(current->allocatedFromPool);
        if (current->allocatedFromPool)
            free(current);
    }
}

MemoryPool::MemoryPool(MemoryPool *from, size_t size, size_t n) : MEMORY_STATISTICS_INIT current(nullptr) {
    if (from && from->available() >= size) {
        void *data = from->alloc(size+sizeof(MemoryPoolPage));
        if (data) {
            addMemory(data, size+sizeof(MemoryPoolPage));
        }
        from->delay([this] {
            destroy(this);
        });
    }
}

MemoryPool::MemoryPool(MemoryPool &from, size_t size, size_t n) : MEMORY_STATISTICS_INIT current(nullptr) {
    if (from.available() >= size) {
        void* data = from.alloc(size+sizeof(MemoryPoolPage));
        if (data) {
            addMemory(data, size+sizeof(MemoryPoolPage));
        }
        from.delay([this] {
            destroy(this);
        });
    }
}

void* MemoryPool::calloc(size_t size, char alignOn) {
    void* retval = alloc(size, alignOn);
//#ifndef EXTRA_SAFE
    memset(retval, 0, size);
//#endif
    return retval;
}

void MemoryPool::reserve(unsigned int bytes, char alignOn) {
#ifdef MEMORY_DEBUG
    return;
#endif
    if (bytes == 0)
        return;
#ifndef NO_MEMORYPOOL_ALIGNMENT
    // make multiple of alignment
    size_t extra = bytes & (alignOn - 1);
    if (extra > 0) {
        bytes += alignOn - extra;
    }
#endif
    // do not facilitate large requests to the pool
    if (bytes > (MEMORYPOOL_SIZE - sizeof(MemoryPoolPage))) {
	#ifdef __APPLE__
        bytes = malloc_good_size(bytes);
	#endif
        void *data = malloc(bytes);
        delay([data] {free(data);});
	#ifndef __APPLE__
        bytes = malloc_usable_size(data);
	#endif
        addMemory(data, bytes);
        return;
    }
    if (current == NULL || bytes > current->available) {
        allocExtra();
    }
}

MemoryPoolPage::MemoryPoolPage(size_t size) : prev(nullptr), next(nullptr), objects(), allocatedFromPool(false), size(size), available(size-sizeof(MemoryPoolPage))
{
}

void MemoryPoolPage::reset() {
    objects.clear();
    unsigned int newSize = size - sizeof(MemoryPoolPage);
#ifdef EXTRA_SAFE
    memset(data, 0, newSize);
#endif
    available = newSize;
}

}
}
#endif
