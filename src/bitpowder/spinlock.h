/**
Copyright 2010-2014 Bernard van Gastel, bvgastel@bitpowder.com.
All rights reserved. This file is part of Bit Powder Libraries.

Bit Powder Libraries is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Bit Powder Libraries is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
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

