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
#include <mutex>

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
        Lock *lck;
    public:
        Locker() : lck(nullptr) {
        }
        Locker(Lock &lock) : lck(&lock) {
            lck->_lock();
        }
        Locker(Locker &&locker) : lck(locker.lck) {
        }
        ~Locker() {
            if (lck)
                lck->_unlock();
        }
        void lock(Lock &l) {
            if (!lck) {
                lck = &l;
                lck->_lock();
            }
        }

        void unlock() {
            if (lck) {
                lck->_unlock();
                lck = nullptr;
            }
        }
    };
    inline Locker locker() {
        return Locker(*this);
    }
};

}
}

#endif

