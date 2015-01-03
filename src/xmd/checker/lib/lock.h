/*
 *  lock.h
 *  SpeedHTTPd
 *
 *  Created by Bernard van Gastel on 03-03-12.
 *  Copyright 2010 Bit Powder. All rights reserved.
 *
 */

#ifndef _LOCK_H_
#define _LOCK_H_

#include "common.h"
#include "exception.h"
#include <assert.h>
#include <mutex>
#include <condition_variable>

namespace bitpowder {
namespace lib {

class Lock {
    NO_COPY(Lock);
    std::mutex m;
public:
    Lock() : m() {
    }
    ~Lock() {
        assert(m.try_lock());
    }
    void _lock() {
        m.lock();
    }
    // always unlock from the same thread as the lock
    inline void _unlock() {
        m.unlock();
    }

    class Locker {
        NO_COPY(Locker);
        friend class Lock;
        std::unique_lock<std::mutex> locker;
    public:
        Locker() {
        }
        Locker(Lock* lock) {
            if (lock)
                locker = std::move(std::unique_lock<std::mutex>{lock->m});
        }
        Locker(Lock& lock) : locker(lock.m) {
        }
        Locker(Locker&& lock) {
            *this = std::move(lock);
        }
        Locker& operator=(Locker&& lock) {
            if (this == &lock)
                return *this;
            unlock();
            this->locker = std::move(lock.locker);
            return *this;
        }
        void lock(Lock &l) {
            if (!hasLock())
                locker = std::move(std::unique_lock<std::mutex>{l.m});
        }
        void unlock() {
            if (hasLock())
                locker.unlock();
        }
        bool hasLock() {
            return locker.owns_lock();
        }
    };

    class ConditionVariable {
#ifndef NDEBUG
        Lock &lock;
#endif
        std::condition_variable cv;
    public:
        ConditionVariable(Lock &lock)
#ifndef NDEBUG
            : lock(lock)
#endif
        {
        }
        void wait(Locker &locker) {
#ifndef NDEBUG
            checkAssert(locker.locker.mutex() == &lock.m);
            checkAssert(locker.locker.owns_lock());
#endif
            cv.wait(locker.locker);
        }
        void notify_one() {
            cv.notify_one();
        }
        void notify_all() {
            cv.notify_all();
        }
    };

    inline Locker locker() {
        return {*this};
    }
};

}
}

#endif

