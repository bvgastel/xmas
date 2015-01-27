/**
Copyright 2012-2014 Bernard van Gastel, bvgastel@bitpowder.com.
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

