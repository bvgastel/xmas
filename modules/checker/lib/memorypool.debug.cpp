#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <assert.h>
#include <string.h>
#include <limits.h>
#include "exception.h"
#include "spinlock.h"
#include "memorypool.h"
#include "common.h"
#include "thread.h"

//#define NO_MEMORYPOOL_ALIGNMENT

namespace bitpowder {
namespace lib {

void MemoryPool::restore(MemoryPoolStatus status) {
    MemoryPoolItem *currentMemoryPool = memory;
    MemoryPoolItem *currentDelayed  = delayed;
    memory = NULL;
    delayed = NULL;
    // pass 1, execute delayed items
    // pools added after the call to mpstatus()
    MemoryPoolItem *cd = currentDelayed;
    while (cd && cd != status.delayed) {
        MemoryPoolItem *next = cd->next;
        if (cd->func != NULL)
            (cd->func)(cd->data);
        //assert(mp->current == NULL);
        free(cd);
        cd = next;
    }
    // pass 2, free memory
    MemoryPoolItem *cm = currentMemoryPool;
    while (cm && cm != status.start) {
        MemoryPoolItem *next = cm->next;
        if (cm->func != NULL)
            (cm->func)(cm->data);
        //assert(mp->current == NULL);
        free(cm);
        cm = next;
    }
    // pass 3, if new memorypools are made during executing this function, append the old linked list to the last one of the new one
    if (memory != NULL) {
        MemoryPoolItem *d = memory;
        while (d->next != NULL)
            d = d->next;
        d->next = cm;
    } else {
        memory = cm;
    }

    if (delayed != NULL) {
        MemoryPoolItem *d = delayed;
        while (d->next != NULL)
            d = d->next;
        d->next = cd;
    } else {
        delayed = cd;
    }
}

void MemoryPool::reserve(size_t size) {
}

MemoryPoolStatus MemoryPool::status() {
    //check(this);
    return MemoryPoolStatus(memory, delayed);
}

void MemoryPool::addMemory(void *data, size_t _size) {
}

void MemoryPool::addMemory(MemoryPool *mp) {
}

MemoryPool::MemoryPool() : memory(nullptr), delayed(nullptr) {
}

MemoryPool::MemoryPool(void *data, size_t size) : memory(nullptr), delayed(nullptr) {
    addMemory(data, size);
}

MemoryPool::~MemoryPool() {
    clear();
}

void MemoryPool::clear() {
    if (memory || delayed) {
        MemoryPoolStatus status;
        restore(status);
        check(memory == NULL);
        check(delayed == NULL);
    }
}

void *MemoryPool::alloc(size_t size, char alignOn, MemoryPoolItem **poolPtr) {
    MemoryPoolItem *i = (MemoryPoolItem*)malloc(sizeof(MemoryPoolItem));
    check(i);
    i->func = free;
    i->data = malloc(size);
    i->next = memory;
    memory = i;
    if (poolPtr)
        *poolPtr = i;
    return i->data;
}

void* MemoryPool::allocAll(size_t &size, char alignOn) {
    int requestedSize = DEFAULT_SIZE;
    void *retval = alloc(requestedSize, alignOn);
    if (retval)
        size = requestedSize;
    return retval;
}

void *MemoryPool::memdup(const void *src, size_t len, char alignOn) {
    if (src == NULL)
        return NULL;
    void *dst = alloc(len);
    if (dst)
        memcpy(dst, src, len);
    return dst;
}

char *MemoryPool::strndup(const char *src, size_t len) {
    if (src == NULL)
        return NULL;
    char *dst = (char*)alloc(len+1, sizeof(char));
    if (dst) {
        memcpy(dst, src, len);
        dst[len] = '\0';
    }
    return dst;
}

char *MemoryPool::strdup(const char *src) {
    if (src == NULL)
        return NULL;
    //return mpmemdup(src, strlen(src)+1);
    return strndup(src, strlen(src));
}

int MemoryPool::vprintf(char **strPtr, size_t max, const char *format, va_list ap) {
    char *str = (char*)alloc(max);
    check(str);
    int size = vsnprintf(str, max, format, ap);
    if (size >= max) {
        str[max-1] = '\0';
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
    return vprintf(strPtr, max, format, ap);
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
}

MemoryPool::MemoryPool(MemoryPool *from, size_t size) : memory(nullptr), delayed(nullptr) {
    if (from) {
        from->delay((DelayedFunc*)(void (*)(MemoryPool*))&destroy<MemoryPool>, this);
    }
}

MemoryPool::MemoryPool(MemoryPool &from, size_t size) : memory(nullptr), delayed(nullptr) {
    from.delay((DelayedFunc*)(void (*)(MemoryPool*))&destroy<MemoryPool>, this);
}

void *MemoryPool::calloc(size_t size, char alignOn) {
    void *retval = alloc(size, alignOn);
#ifndef EXTRA_SAFE
    memset(retval, 0, size);
#endif
    return retval;
}

}
}

