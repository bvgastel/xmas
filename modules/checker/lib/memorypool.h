/*****
 * MemoryPool
 * - with delayed function execution
 * - fast allocation of temporary memory
 * - fast saving and restoring of the current state
 */

#ifdef MEMORY_DEBUG
#include "memorypool.debug.h"
#endif

#ifndef _MEMORYPOOL_H_
#define _MEMORYPOOL_H_

#include <limits>
#include <cstddef>

//#define MEMORY_STATISTICS

#define MEMORYPOOL_SIZE _MemoryPoolPageSize

#include <stdlib.h>
#include <stdarg.h>
#include <assert.h>
#include <unistd.h>
#include "exception.h"
#include "common.h"
#include "deque.h"
#include "stack.h"

namespace bitpowder {
namespace lib {

extern const size_t _MemoryPoolPageSize;

struct DelayedItemBase {
    DelayedItemBase *next;
    DelayedItemBase() : next(nullptr) {}
    virtual void execute() = 0;
};

template <typename F, typename... T>
struct DelayedItem : public DelayedItemBase {
    F func;
    std::tuple<T...> args;
    void *extra[0];
    DelayedItem(F f, const T & ... func) : func(std::move(f)), args(func...) {
    }
    void execute() {
        callFunction(func, args);
    }
};

class MemoryPoolPage {
    template <unsigned int SIZE>
    friend class StaticMemoryPool;
public:
    MemoryPoolPage *prev;
    MemoryPoolPage *next;
private:
    MemoryPoolPage(size_t size);

    Stack<DelayedItemBase*> objects;
    bool allocatedFromPool:1;
    int size:31; // including this struct
    int available;
    void *data[0]; // type is double so it is word aligned, according to the C spec

    friend class MemoryPool;
    void reset();
};

class MemoryPoolStatus {
    MemoryPoolPage *current;
    DelayedItemBase *objects;
    int available;
    MemoryPoolPage *nextPage;
    int nested;
public:
    MemoryPoolStatus(MemoryPoolPage *start = nullptr, MemoryPoolPage *nextPage = nullptr) : current(start), objects(nullptr), available(0), nextPage(nextPage), nested(0) {};
    //MemoryPoolStatus(MemoryPoolPage *start = nullptr, MemoryPoolPage *nextPage = nullptr, DelayedItemBase *objects = nullptr, unsigned int available = 0) : start(start), nextPage(nextPage), objects(objects), available(available) {};

    friend class MemoryPool;
};

#define ALIGN_ON sizeof(void*)

class MemoryPool {
protected:
    MemoryPool(const MemoryPool&);
    MemoryPool& operator=(const MemoryPool&);
    // NOT: sorted with minimal pools first, checks if space is available until it comes in a larger pool
    // NOT: a way to start searching in pools who have indeeed enough space, by making it an array, and the first item is for bytes < 16, second for < 32, etc
    // because otherwise MemoryPoolStatus struct becomes to large
    MemoryPoolPage *current;
    int nested;
    Deque<MemoryPoolPage*> all;
    //unsigned int available; // TODO: this could help to minimise page faults

    MemoryPoolPage* initMemory(void *data, size_t size);
public:
    // alloc memory from the current memory pool (alloc new one if there is not enough memory). Only usable in the current autorelease pool
    void* alloc(size_t size, char alignOn = ALIGN_ON, MemoryPoolPage **pool = NULL);

    void* allocAll(size_t &size, char alignOn = ALIGN_ON);
    void* calloc(size_t size, char alignOn = ALIGN_ON);

    // do not let memory allocations after this one cross into the next memory pool, restore after with mprestore. Should be done when allocatin a new autoreleasepool
    MemoryPoolStatus status();
    void restore(MemoryPoolStatus status);

    size_t available() {
        return current ? current->available : (MEMORYPOOL_SIZE-sizeof(MemoryPoolPage));
    }
    void *next(char alignOn = ALIGN_ON) {
        return current ? &((char*)current->data)[current->size - sizeof(MemoryPoolPage) - current->available] : nullptr;
    }

#ifdef MEMORY_STATISTICS
    size_t freed;
    size_t allocated;
    size_t used() {
        return allocated - freed;
    }
#define MEMORY_STATISTICS_INIT freed(0), allocated(0),
#else
#define MEMORY_STATISTICS_INIT
#endif

    // with spare memory add a memory pool. Do not forget to autorelease if memory is needed for something else!
    MemoryPool(void *data, size_t size);
    MemoryPool();
    MemoryPool(MemoryPool *from, size_t size, size_t n = 1);
    MemoryPool(MemoryPool &from, size_t size, size_t n = 1);
    ~MemoryPool();

    void clear();

    void addMemory(void *data, size_t size);
    void addMemory(MemoryPool *mp);

    // some utility functions that uses memory
    void* memdup(const void *data, size_t length, char alignOn = ALIGN_ON);
    char* strdup(const char *str);
    char* strndup(const char *str, size_t length);

    int printf(char **str, size_t max, const char *format, ...) __attribute__ ((format (printf, 4, 5)));
    int vprintf(char **str, size_t max, const char *format, va_list ap);
    // not terminating the string, useful for generating byte strings
    int printf_nt(char **str, size_t max, const char *format, ...) __attribute__ ((format (printf, 4, 5)));
    int vprintf_nt(char **str, size_t max, const char *format, va_list ap);

    void returnUnused(char *buffer, size_t len);

    template <typename T>
    void delay(T &&delayedFunc, size_t extra = 0, void **extraPtr = nullptr) {
        MemoryPoolPage *p = NULL;
        DelayedItem<T> *obj = (DelayedItem<T>*)alloc(sizeof(DelayedItem<T>)+extra, ALIGN_ON, &p);
        checkAssert(p != NULL);
        //check(obj != NULL);
        new ((void*)obj) DelayedItem<T>(std::move(delayedFunc));
        p->objects.push_back(obj);
        if (extraPtr)
            *extraPtr = &obj->extra;
    }
    template <typename T, typename Object>
    void *allocAndDelayOn(size_t size, T delayedFunc) {
        MemoryPoolPage* p = NULL;
        DelayedItem<T, Object*>* obj = (DelayedItem<T, Object*>*)alloc(sizeof(DelayedItem<T, Object*>) + size, ALIGN_ON, &p);
        checkAssert(p != NULL);
        //check(obj != NULL);
        Object *object = (Object*)&obj->extra[0];
        new((void*)obj) DelayedItem<T, Object*>(delayedFunc, object);
        p->objects.push_back(obj);
        return object;
    }

    void reserve(unsigned int chunkSize, char alignOn = ALIGN_ON);

    static void vacuum();

    template <typename T>
    class Allocator {
    public:
        MemoryPool *mp;
        int nested;
        typedef size_t size_type;
        typedef ptrdiff_t difference_type;
        typedef T* pointer;
        typedef const T* const_pointer;
        typedef T& reference;
        typedef const T& const_reference;
        typedef T value_type;

        template <class U>
        struct rebind {
            typedef Allocator<U> other;
        };

        inline Allocator(MemoryPool *mp) : mp(mp), nested(mp->nested) {
        }

        template <class U>
        inline Allocator(const Allocator<U> &mpa) : mp(mpa.mp), nested(mp->nested) {
        }

        inline pointer address(reference x) const {
            return &x;
        }
        inline const_pointer address(const_reference x) const {
            return &x;
        }
        inline pointer allocate(size_type size, const void* hint = 0) {
#ifdef MEMORY_STATISTICS
            fprintf(stderr, "- allocate: %li\n", sizeof(value_type)*size);
            mp->allocated += sizeof(value_type)*size;
#endif
            pointer retval = (pointer)mp->alloc(sizeof(value_type)*size);
            //std::cerr << "allocating pointer " << retval << std::endl;
            return retval;
        }

        inline void deallocate(pointer p, size_type n) {
#ifdef MEMORY_STATISTICS
            fprintf(stderr, "- deallocate: %li\n", sizeof(value_type)*n);
            mp->freed += sizeof(value_type)*n;
#endif
            //if (mp->nested == nested)
                //mp->returnUnused((char*)p, n);
        }
        inline void deallocate(void *p, size_type n) {
#ifdef MEMORY_STATISTICS
            fprintf(stderr, "- deallocate: %li\n", sizeof(value_type)*n);
            mp->freed += sizeof(value_type)*n;
#endif
            //if (mp->nested == nested)
                //mp->returnUnused((char*)p, n);
        }
        inline size_type max_size() const {
            //return mp->available() / sizeof(value_type);
            return std::numeric_limits<size_type>::max();
        }
        void construct(pointer p,  const_reference val) {
            new((void*)p) T(val);
        }
        template<class U, typename... Args>
        void construct(U* p,  Args&&... args) {
            new((void*)p) U(std::forward<Args>(args)...);
        }
        inline void construct(pointer p) {
            new((void*)p) T();
        }
        inline void destroy(pointer p) {
            p->T::~T();
        }
    };

    template <class T>
    Allocator<T> allocator() {
        return Allocator<T>(this);
    }
    operator MemoryPool*() {
        return this;
    }
private:
    MemoryPoolPage* allocExtra(size_t newSize = MEMORYPOOL_SIZE);
};

template <unsigned int SIZE>
class StaticMemoryPool : public MemoryPool {
    char buffer[SIZE+sizeof(MemoryPoolPage)];
public:
    StaticMemoryPool() : MemoryPool() {
        addMemory(buffer, SIZE+sizeof(MemoryPoolPage));
    }
    operator MemoryPool*() {
        return this;
    }
    MemoryPool *operator*() {
        return this;
    }
    MemoryPool *operator->() {
        return this;
    }
    void clear() {
        MemoryPool::clear();
        addMemory(buffer, SIZE+sizeof(MemoryPoolPage));
    }
};


}
}

template <class T>
inline void* operator new(std::size_t nbytes, bitpowder::lib::MemoryPool::Allocator<T> alloc) {
#ifdef MEMORY_STATISTICS
    fprintf(stderr, "- allocate: %li\n", nbytes);
    alloc.mp->allocated += nbytes;
#endif
    return alloc.mp->alloc(nbytes);
}

inline void* operator new(std::size_t nbytes, bitpowder::lib::MemoryPool *mp) {
#ifdef MEMORY_STATISTICS
    fprintf(stderr, "- allocate: %li\n", nbytes);
    mp->allocated += nbytes;
#endif
    return mp->alloc(nbytes);
}

inline void* operator new(std::size_t nbytes, bitpowder::lib::MemoryPool &mp) {
#ifdef MEMORY_STATISTICS
    fprintf(stderr, "- allocate: %li\n", nbytes);
    mp.allocated += nbytes;
#endif
    return mp.alloc(nbytes);
}

template <class T>
inline void* operator new(std::size_t nbytes, bitpowder::lib::MemoryPool *mp, void (*f)(T *) = nullptr) {
#ifdef MEMORY_STATISTICS
    fprintf(stderr, "- allocate: %li\n", nbytes);
    mp->allocated += nbytes;
#endif
    return mp->allocAndDelayOn<void (*)(T*), T>(nbytes, f);
}

template <class T>
inline void* operator new(std::size_t nbytes, bitpowder::lib::MemoryPool &mp, void (*f)(T *) = nullptr) {
#ifdef MEMORY_STATISTICS
    fprintf(stderr, "- allocate: %li\n", nbytes);
    mp.allocated += nbytes;
#endif
    return mp.allocAndDelayOn<void (*)(T*), T>(nbytes, f);
}


#endif

