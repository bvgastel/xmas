/**
Copyright 2010-2015 Bernard van Gastel, bvgastel@bitpowder.com.
This file is part of Bit Powder Libraries.

Bit Powder Libraries is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Bit Powder Libraries is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Bit Powder Libraries.  If not, see <http://www.gnu.org/licenses/>.
*/

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

#ifndef NCONCURRENCY
#include "spinlock.h"
#endif

#ifdef __FreeBSD__
#include <malloc_np.h> // FreeBSD
#elif defined(__APPLE__)
#include <malloc/malloc.h>
#else
#include <malloc.h>
#endif

#ifdef __WIN32__
#define bzero(b,len) (memset((b), '\0', (len)), (void) 0)
#endif

//#define NO_MEMORYPOOL_ALIGNMENT
#define EXTRA_SAFE

/*
 * TODO:
 * - reserve(int chunks, int maxSizeOfItem): make users of reserve specify more clearly what they need
 * - on a large allocation, allocate a power of two and returnUnused() the remaining memory
 * - for MemoryPoolPage, maybe use CacheInstances, instead of own implementation (but now, all the unused MemoryPools are put in the pool with one locking operation)
 */

/**
 * Two important situations that need to be taken into account when modifying the implementation of MemoryPool:
 * - when restoring/clearing a MemoryPool containing a STL container with custom allocator: the memory freed/deallocted
 * by the STL container must not be re-added to the MemoryPool, as the MemoryPage containing this memory is going to be
 * deallocted
 * - if new memory is allocated (especially memory that is allocated out of band of the memory pool system), needs to
 * be deallocted after all the delayed objects are executed, as a STL container can access this memory during its
 * destructor phase.
 */

namespace bitpowder {
namespace lib {

#ifdef __MINGW32__
const size_t MEMORYPOOL_SIZE = 4096;
#else
const size_t MEMORYPOOL_SIZE = getpagesize();
#endif

#ifndef NCONCURRENCY
SpinLock newPoolLock;
#endif
Stack<MemoryPoolPage*,MemoryPoolPage,&MemoryPoolPage::prev> newPool;

void MemoryPool::restore(const MemoryPoolStatus& status) {
    rewinding = true;
    MemoryPoolPage *currentMemoryPool = current;
    Deque<MemoryPoolPage*> currentAll;
    currentAll.swap(all);
    current = nullptr;
    // PASS 1, execute delayed items
    // also perform it for the newly added delayed items in the pool that was current when status() was called
    while (objects.back() != status.objects)
        objects.pop_back()->execute();
    // PASS 2, dispose of pages (not earlier, because a delay function of an 'earlier' memorypool can depend on memory of a 'later' memorypool...)
    Queue<MemoryPoolPage*,MemoryPoolPage,&MemoryPoolPage::prev> readd;
    Deque<MemoryPoolPage*> toNextPages;
    while (currentMemoryPool && currentMemoryPool != status.current) {
        MemoryPoolPage *prev = currentAll.prev(currentMemoryPool);
        assert(currentAll.front() == currentMemoryPool || currentAll.front() != currentAll.back());
        currentAll.erase(currentMemoryPool);
        if (currentMemoryPool->allocatedFromPool) {
            readd.push(currentMemoryPool);
        } else {
            toNextPages.push_front(currentMemoryPool);
        }
        currentMemoryPool = prev;
    }
    checkAssert(currentMemoryPool == status.current);

    currentAll.insert(currentAll.next(currentMemoryPool), std::move(toNextPages));

    // rewind nextPages, as these are never allocated from the global pool, we don't have to readded them
    while (currentAll.back() && currentAll.back() != status.nextPage && currentAll.back() != currentMemoryPool)
        currentAll.pop_back();

    // reset available to previous value
    if (status.current) {
        assert(status.current->available <= status.available);
        status.current->available = status.available;
    }

    // PASS 3, if new memorypools are made during executing this function, append the old linked list to the last one of the new one
    if (all.empty()) {
        current = status.current;
    } else {
        // do not modifiy 'current'!
        Deque<MemoryPoolPage*> active = all.split(current); // split the part currently in use
        currentAll.insert(currentAll.next(currentMemoryPool), std::move(active));
        currentAll.push_back(std::move(all));
    }
    all = std::move(currentAll);

    // this should be done fairly late, as here memory gets deallocated, and in that memory MemoryPoolPages can exists (that are in the linked lists that are traversed above)
    while (memoryObjects.back() != status.memoryObjects) {
        auto object = memoryObjects.pop_back();
        object->execute();
    }

    // add all the memorypools to be recycled in one operation to the pool
    // this should be executed as last, as 'memoryObjects' objects can live inside these pools
    if (!readd.empty()) {
	for (const auto& page : readd)
            page->reset();
#ifndef NCONCURRENCY
        SpinLock::Locker l(newPoolLock);
#endif
        newPool.splice(readd);
    }

    if (current) {
#ifdef EXTRA_SAFE
        assert(current->available <= current->size);
        bzero(&((char*)current->data)[current->size - current->available], current->available);
#endif
    }

    rewinding = false;
}

MemoryPoolStatus MemoryPool::status() {
    MemoryPoolStatus retval(current, all.back());
    if (retval.current) {
        retval.available = retval.current->available;
        retval.objects = objects.back();
        retval.memoryObjects = memoryObjects.back();
    }
    return retval;
}

bool MemoryPool::addMemory(void *data, size_t _size) {
    if (data == nullptr || _size <= 512)
        return false;

#ifndef NO_MEMORYPOOL_ALIGNMENT
    // make multiple of alignment
    size_t extra = ((long)data) & (DEFAULT_ALIGN_ON - 1);
    if (extra) {
        if (DEFAULT_ALIGN_ON-extra > _size) {
            data = nullptr;
            _size = 0;
        } else {
            data = &((char*)data)[DEFAULT_ALIGN_ON-extra];
            _size -= (DEFAULT_ALIGN_ON-extra);
        }
    }
#endif

    if (data == nullptr || _size <= sizeof(MemoryPoolPage)) // || size > USHRT_MAX /* length of short */)
        return false;

#ifdef EXTRA_SAFE
    bzero(data, _size);
#endif

    MemoryPoolPage *page = new (data) MemoryPoolPage(_size-sizeof(MemoryPoolPage));
    all.push_back(page);
    if (!current)
        current = page;
    return true;
}

void MemoryPool::addMemory(MemoryPool* mp) {
    if (mp) {
        size_t size = 0;
        void *data = mp->allocAll(size);
        if (data && size > 0)
            addMemory(data, size);
    }
}

// allocs new pool
MemoryPoolPage* MemoryPool::allocExtra() {
    if (current && all.next(current)) {
        return current = all.next(current);
    }
    assert(all.back() == current);
    MemoryPoolPage *page = nullptr;
    size_t size = MEMORYPOOL_SIZE;
    // try to fetch one from the cache
    if (page == nullptr) {
#ifndef NCONCURRENCY
        SpinLock::Locker l(newPoolLock);
#endif
        page = newPool.pop_back();
    }
    // if not found alloc new one
    if (page == nullptr) {
        page = (MemoryPoolPage*)malloc(size);
        assert(page != nullptr);
        new(page) MemoryPoolPage(size - sizeof(MemoryPoolPage));
        page->allocatedFromPool = true;
        //fprintf(stderr, "alloc extra: %p\n", pool);
    }
    // really add the memory to the pools
    assert(page->available == page->size);
    assert(page->size == MEMORYPOOL_SIZE-sizeof(MemoryPoolPage));
    assert(page->next == nullptr);
    assert(page->prev == nullptr);
    assert(page->allocatedFromPool);

    all.insert(all.next(current), page);
    return current = page;
}

MemoryPool::MemoryPool() : current(nullptr), all() {
}

MemoryPool::MemoryPool(void *data, size_t size) : current(nullptr), all() {
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
        assert(current == nullptr);
    }
}

void* MemoryPool::alloc(size_t size, char alignOn, MemoryPoolPage **pool) {
    if (size == 0)
        return nullptr;

#ifndef NO_MEMORYPOOL_ALIGNMENT
    // make multiple of alignment
    size_t extra = size & (alignOn - 1);
    if (extra > 0) {
        size += alignOn - extra;
    }
#endif
    // do not facilitate large requests to the pool, except if there is (because of a super memorypool) room available
    if (size > MEMORYPOOL_SIZE/2 && (current == nullptr || size > current->available)) {
        //fprintf(stderr, "too large request: %i\n", size);;
        std::size_t bytes = size;
    #ifdef __APPLE__
        bytes = malloc_good_size(bytes);
    #endif
        void *data = malloc(bytes);
    #ifdef __FreeBSD__
        bytes = malloc_usable_size(data);
    #endif
    #ifdef __linux__
        bytes = malloc_usable_size(data);
    #endif
        addMemory(&((char*)data)[size], bytes-size);
        delayMemory([data] {free(data);});
        if (pool != nullptr)
            *pool = current;
        return data;
    }
    // search for pool with enough space
    MemoryPoolPage *c = current;
    while (c == nullptr || size > c->available) {
        c = allocExtra();
    }
    // 2015-01-28 Guus Bonnema: std::size_t is always unsigned since c++11
    // TODO: Check
    //checkAssert(c->available >= 0);     // TODO: check : std::size_t c->available is unsigned: always >= 0
    // Remark: the C size_t is equivalent to C++ std::size_t except for the namespace.
    checkAssert(size <= c->available);
    // alloc
    void* data = &((char*)c->data)[c->size - c->available];
    c->available -= size;
    checkAssert(c->available >= 0);
    if (pool != nullptr)
        *pool = c;
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
    if (src == nullptr)
        return nullptr;
    void* dst = alloc(len, alignOn);
    if (dst)
        memcpy(dst, src, len);
    return dst;
}

char* MemoryPool::strndup(const char *src, size_t len) {
    if (src == nullptr)
        return nullptr;
    char* dst = (char*)alloc(len+1, sizeof(char));
    if (!dst)
        return nullptr;
    memcpy(dst, src, len);
    dst[len] = '\0';
    return dst;
}

char* MemoryPool::strdup(const char *src) {
    if (src == nullptr)
        return nullptr;
    //return mpmemdup(src, strlen(src)+1);
    return strndup(src, strlen(src));
}

int MemoryPool::vprintf(char **strPtr, size_t max, const char *format, va_list ap) {
    if (max == 0)
        return -1;
    MemoryPoolPage *p = nullptr;
    int size = 0;
    char* str = (char*)alloc(max, sizeof(char), &p);
    if (str) {
        //check(str != nullptr);
        //check(p != nullptr);
        size = vsnprintf(str, max, format, ap);
        // check for error
        if (size < 0) {
            returnUnused(str, max);
            if (strPtr != nullptr)
                *strPtr = nullptr;
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
    if (strPtr != nullptr)
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
    MemoryPoolPage *p = nullptr;
    char* str = (char*)alloc(max, sizeof(char), &p);
    //check(str != nullptr);
    int size = 0;
    //check(p != nullptr);
    if (str) {
        size = vsnprintf(str, max, format, ap);
        // check for error
        if (size < 0) {
            // check if pointer is really alloced inside memorypool
            returnUnused(str, max);
            if (strPtr != nullptr)
                *strPtr = nullptr;
            return size;
        }
        if (((size_t)size) < max) {
            // some space is left!
            returnUnused(str+size, max-size);
        }
    }
    if (strPtr != nullptr)
        *strPtr = str;
    return size;
}

bool MemoryPool::returnUnused(char *buffer, size_t len)
{
    if (current && buffer+len == next(sizeof(char))) {
        current->available += len;
        return true;
    } else {
        return addMemory(buffer, len);
        //return false;
    }
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
    MemoryPoolPage *current = nullptr;

#ifndef NCONCURRENCY
    SpinLock::Locker l(newPoolLock);
#endif
    Stack<MemoryPoolPage*,MemoryPoolPage,&MemoryPoolPage::prev> pools(std::move(newPool));
#ifndef NCONCURRENCY
    l.unlock();
#endif

    while ((current = pools.pop_back())) {
        //fprintf(stderr, "vacuuming: %p\n", current);
        assert(current->allocatedFromPool);
        if (current->allocatedFromPool)
            free(current);
    }
}

MemoryPool::MemoryPool(MemoryPool *from, size_t size, size_t n) : current(nullptr) {
    if (from && from->available() >= size) {
        void *data = from->alloc(size+sizeof(MemoryPoolPage));
        if (data) {
            addMemory(data, size+sizeof(MemoryPoolPage));
        }
        from->delayMemory([this] {
            destroy(this);
        });
    }
}

MemoryPool::MemoryPool(MemoryPool &from, size_t size, size_t n) : current(nullptr) {
    if (from.available() >= size) {
        void* data = from.alloc(size+sizeof(MemoryPoolPage));
        if (data) {
            addMemory(data, size+sizeof(MemoryPoolPage));
        }
        from.delayMemory([this] {
            destroy(this);
        });
    }
}

void* MemoryPool::calloc(size_t size, char alignOn) {
    void* retval = alloc(size, alignOn);
    bzero(retval, size);
    return retval;
}

void MemoryPool::reserve(unsigned int bytes, char alignOn) {
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
    if (bytes > MEMORYPOOL_SIZE/2 && (current == nullptr || bytes > current->available)) {
	#ifdef __APPLE__
        bytes = malloc_good_size(bytes);
	#endif
        void *data = malloc(bytes);
    #ifdef __FreeBSD__
        bytes = malloc_usable_size(data);
    #endif
    #ifdef __linux__
        bytes = malloc_usable_size(data);
    #endif
        addMemory(data, bytes);
        delayMemory([data] {free(data);});
        return;
    }
    if (current == nullptr || bytes > current->available) {
        allocExtra();
    }
}

MemoryPoolPage::MemoryPoolPage(std::size_t size) : allocatedFromPool(false), size(size), available(size)
{
}

void MemoryPoolPage::reset() {;
    assert(size == MEMORYPOOL_SIZE-sizeof(MemoryPoolPage));
    assert(available <= size);
#ifdef EXTRA_SAFE
    bzero(data, size-available);
#endif
    available = size;
}

}
}
#endif
