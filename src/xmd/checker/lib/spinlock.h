/*
 *  spinlock.h
 *  SpeedHTTPd
 *
 *  Created by Bernard van Gastel on 29-04-10.
 *  Copyright 2010 Bit Powder. All rights reserved.
 *
 */

#ifndef _SPINLOCK_H_
#define _SPINLOCK_H_

#include "common.h"
#include <atomic>
#include <thread>

#include <assert.h>

namespace bitpowder {
namespace lib {

class SpinLock {
    std::atomic_flag m;
public:
    inline SpinLock() : m(false) {
    }
    inline SpinLock(const SpinLock&) : m(false) {
    }
    inline ~SpinLock() {
        assert(!m.test_and_set());
    }
    inline void _lock() {
        while (m.test_and_set())
            std::this_thread::yield();
    }
    // always unlock from the same thread as the lock
    inline void _unlock() {
        m.clear();
    }

    // std::lock_guard is not the same, as std::lock_guard can not be std::move()'d, and Locker can. Also you can explicitly unlock and later "relock" a Locker object.
    class Locker {
        NO_COPY(Locker);
        friend class SpinLock;
        SpinLock *spinlock;
    public:
        Locker() : spinlock(nullptr) {
        }
        Locker(SpinLock* lock) : spinlock(lock) {
            if (spinlock)
                spinlock->_lock();
        }
        Locker(SpinLock& lock) : spinlock(&lock) {
            spinlock->_lock();
        }
        Locker(Locker&& lock) : spinlock(nullptr) {
            *this = std::move(lock);
        }
        Locker& operator=(Locker&& lock) {
            if (this == &lock)
                return *this;
            unlock();
            spinlock = lock.spinlock;
            lock.spinlock = nullptr;
            return *this;
        }
        ~Locker() {
            if (spinlock)
                spinlock->_unlock();
            spinlock = nullptr;
        }
        void lock(SpinLock &l) {
            if (!spinlock) {
                spinlock = &l;
                spinlock->_lock();
            }
        }
        void unlock() {
            if (spinlock) {
                spinlock->_unlock();
                spinlock = nullptr;
            }
        }
        bool hasLock() {
            return spinlock != nullptr;
        }
    };
    inline Locker locker() {
        return Locker(*this);
    }
};

}
}

#endif

