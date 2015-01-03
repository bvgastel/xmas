/*
 *  memory.h
 *  SpeedHTTPd
 *
 *  Created by Bernard van Gastel on 18-03-12.
 *  Copyright 2012 Bit Powder. All rights reserved.
 *
 */
#ifndef MEMORY_H
#define MEMORY_H

#ifdef CONCURRENCY
#include "spinlock.h"
#endif
#include "stack.h"
#include "exception.h"
#ifndef NDEBUG
#endif
#include <set>

//#ifdef __linux__
//#define MEMORY_DEBUG_MEMORY_H
//#endif

#ifdef MEMORY_DEBUG
#define MEMORY_DEBUG_MEMORY_H
#endif

namespace bitpowder {
namespace lib {

template <class T>
class CachedInstance {
public:
    CachedInstance *next;
    CachedInstance() : next(nullptr) {
    }
};

class CacheOfInstancesBase {
public:
    static std::set<CacheOfInstancesBase*>* getAllInstanceCaches();
    static void purgeAll();
    virtual void purge() const = 0;
};

template <class T>
class CacheOfInstances : CacheOfInstancesBase {
public:
#ifdef CONCURRENCY
    mutable SpinLock instanceCacheLock;
#endif
    mutable Stack< CachedInstance<T>* > instanceCache;

    void purge() const {
#ifdef CONCURRENCY
        SpinLock::Locker l(instanceCacheLock);
#endif
        Stack<CachedInstance<T>*> local = instanceCache.pop_all();
#ifdef CONCURRENCY
        l.unlock();
#endif
        //std::cout << "purging instances of " << typeid(T).name() << std::endl;
        while (!local.empty()) {
            //std::cout << "freeing a instance of " <<  typeid(T).name() << std::endl;
            CachedInstance<T> *instance = local.pop_back();
            free(instance);
        }
    }
    CacheOfInstances() : instanceCache() {
        getAllInstanceCaches()->insert(this);
    }
};

template <class T>
class CacheInstances {
#ifndef MEMORY_DEBUG_MEMORY_H
    static CacheOfInstances<T> InstanceCache;
#ifndef NDEBUG
    static std::set<void*> instances;
#endif
public:
    ~CacheInstances() {
    }
    class Reserved {
        T *pointer = nullptr;
        void clear() {
            if (pointer) {
                T *c = pointer;
#ifdef CONCURRENCY
                SpinLock::Locker l(InstanceCache.instanceCacheLock);
#endif
                InstanceCache.instanceCache.push_back((CachedInstance<T>*)c);
#ifndef NDEBUG
                instances.erase(pointer);
#endif
                pointer = nullptr;
            }
        }

    public:
        Reserved(T *pointer = nullptr) : pointer(pointer) {
        }
        Reserved(Reserved &&r) : pointer(r.pointer) {
            r.pointer = nullptr;
        }
        Reserved& operator=(Reserved &&r) {
            if (this == &r)
                return *this;
            clear();
            pointer = r.pointer;
            r.pointer = nullptr;
            return *this;
        }
        ~Reserved() {
            clear();
        }

        void *get() {
            T *retval = pointer;
            pointer = nullptr;
            return retval;
        }

        void reserve() {
            if (pointer)
                return;
#ifdef CONCURRENCY
            SpinLock::Locker l(InstanceCache.instanceCacheLock);
#endif
            pointer = (T*)InstanceCache.instanceCache.pop_back();
#ifdef CONCURRENCY
            l.unlock();
#endif
            if (!pointer)
                pointer = (T*)malloc(sizeof(T));
#ifndef NDEBUG
#ifdef CONCURRENCY
            l.lock(InstanceCache.instanceCacheLock);
#endif
            instances.insert((void*)pointer);
#endif
        }
    };

    static Reserved reserve() {
        Reserved retval;
        retval->reserve();
        return retval;
    }

    static void *operator new(std::size_t size, Reserved &r) {
#ifndef NDEBUG
        checkAssert(sizeof(T) == size);
#endif
        void *retval = r.get();
        return retval ? retval : malloc(sizeof(T));
    }

    static void operator delete(void *pointer, std::size_t size) {
#ifdef CONCURRENCY
        SpinLock::Locker l(InstanceCache.instanceCacheLock);
#endif
#ifndef NDEBUG
        // to avoid the assert in Stack ensuring the next pointer is always nullptr
        InstanceCache.instanceCache.push_back(new (pointer) CachedInstance<T>());
        instances.erase(pointer);
#else
        T *c = (T*)pointer;
        InstanceCache.instanceCache.push_back( (CachedInstance<T>*)c);
#endif
    }
    static void *operator new(std::size_t size) {
        T *retval = nullptr;
        {
#ifdef CONCURRENCY
            SpinLock::Locker l(InstanceCache.instanceCacheLock);
#endif
            retval = (T*)InstanceCache.instanceCache.pop_back();
        }
        if (!retval) {
            retval = (T*)malloc(size);
            //std::cout << "new " << retval << " of " << size << " bytes" << std::endl;
        }
#ifndef NDEBUG
#ifdef CONCURRENCY
        SpinLock::Locker l(InstanceCache.instanceCacheLock);
#endif
        instances.insert((void*)retval);
#endif
        return retval;
    }

#ifndef NDEBUG
    static void showInstances() {
        fprintf(stderr, "%lu instances\n", instances.size());
        for (std::set<void*>::iterator it = instances.begin(); it != instances.end(); ++it) {
            fprintf(stderr, "* %p\n", *it);
        }
    }
    static void showCachedInstancesHelper(CachedInstance<T> *pointer) {
        fprintf(stderr, "* %p\n", pointer);
    }
    static void showCachedInstances() {
        std::for_each(InstanceCache.instanceCache.begin(), InstanceCache.instanceCache.end(), showCachedInstancesHelper);
        while (!InstanceCache.instanceCache.empty())
            free(InstanceCache.instanceCache.pop_back());
    }
#endif
#else
public:
    class Reserved {
    public:
        void reserve() {
        }
    };
    static void *operator new(size_t size) {
        void *pointer = malloc(size);
        return pointer;
    }
    static void *operator new(size_t size, Reserved &r) {
        void *pointer = malloc(size);
        return pointer;
    }
    static void operator delete(void *pointer, std::size_t size) {
        free(pointer);
    }
    static void showInstances() {
    }
    static void showCachedInstances() {
    }
#endif
};
#ifndef MEMORY_DEBUG_MEMORY_H
template <typename T> CacheOfInstances<T> CacheInstances<T>::InstanceCache;

#ifndef NDEBUG
template <typename T> std::set<void*> CacheInstances<T>::instances;
#endif
#endif

}
}

/**
 * @brief Variable size allocations are supported
 *
 * Usages:
 * - new(extra) SomeType;
 * - new(extra, MemoryPool) SomeType;
 */

template <typename... Args>
void* operator new(std::size_t nbytes, std::size_t extra, Args&&... args) {
    return operator new(nbytes+extra, std::forward<Args>(args)...);
}

#endif // MEMORY_H
