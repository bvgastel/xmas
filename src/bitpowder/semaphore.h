/*
 *  semaphore.h
 *  SpeedHTTPd
 *
 *  Created by Bernard van Gastel on 29-04-10.
 *  Copyright 2010 Bit Powder. All rights reserved.
 *
 */

#ifndef _BPSEMAPHORE_H_
#define _BPSEMAPHORE_H_

#define USE_CONDITION_VARIABLE

#ifdef USE_CONDITION_VARIABLE

#include <mutex>
#include <condition_variable>

namespace bitpowder {
namespace lib {

class Semaphore {
    std::mutex m;
    std::condition_variable c;
    int count;
    int waiting = 0;
public:
    Semaphore(int initialValue) : count(initialValue) {
    }
    ~Semaphore() {
#ifndef NDEBUG
        std::unique_lock<std::mutex> l(m);
        checkAssert(waiting == 0);
#endif
    }
    void signal() {
        std::unique_lock<std::mutex> l(m);
        ++count;
        if (waiting >= count) {
            l.unlock();
            c.notify_one();
        }
    }
    void wait() {
        std::unique_lock<std::mutex> l(m);
        while (count == 0) {
            waiting++;
            c.wait(l);
            waiting--;
        }
        --count;
    }
};

}
}

#else

#include "spinlock.h"

// see also: http://stackoverflow.com/questions/3640853/performance-test-sem-t-v-s-dispatch-semaphore-t-and-pthread-once-t-v-s-dispat

#ifdef __APPLE__
#include <mach/semaphore.h>
#include <mach/task.h>
#include <mach/mach.h>
#define CSemaphore semaphore_t
#define semaphoreInit(ptr, init) semaphore_create(mach_task_self(), ptr, SYNC_POLICY_FIFO, init)
#define semaphoreSignal(ptr) semaphore_signal(*(ptr))
#define semaphoreWait(ptr) semaphore_wait(*(ptr))
#define semaphoreDestroy(ptr) semaphore_destroy(mach_task_self(), *(ptr))
/*
#include <dispatch/dispatch.h>
#define CSemaphore dispatch_semaphore_t
#define semaphoreInit(ptr, init) *ptr = dispatch_semaphore_create(init)
#define semaphoreSignal(ptr) dispatch_semaphore_signal(*ptr)
#define semaphoreWait(ptr) dispatch_semaphore_wait(*ptr, DISPATCH_TIME_FOREVER)
#define semaphoreDestroy(ptr) dispatch_release(*ptr)
*/
#else
#include <semaphore.h>
#define CSemaphore sem_t
#define semaphoreInit(ptr, init) sem_init(ptr, 0, init)
#define semaphoreSignal sem_post
#define semaphoreWait sem_wait
#define semaphoreDestroy sem_destroy
/*
#include <dispatch/dispatch.h>
#define CSemaphore dispatch_semaphore_t
#define semaphoreInit(ptr, init) *ptr = dispatch_semaphore_create(init)
#define semaphoreSignal(ptr) dispatch_semaphore_signal(*ptr)
#define semaphoreWait(ptr) dispatch_semaphore_wait(*ptr, DISPATCH_TIME_FOREVER)
#define semaphoreDestroy(ptr) dispatch_release(*ptr)
*/
#endif

// sys/sem.h
//dispatchSemaphore = semget(IPC_PRIVATE, 1, IPC_CREAT | SEM_R | SEM_A);
// Signal ->        struct sembuf sem_unlock = { 0, 1, 0 };
//                  checkRetval(semop(dispatchSemaphore, &sem_unlock, 1));
// Wait ->          struct sembuf sem_lock = { 0, -1, 0 };
//                  checkRetval(semop(dispatchSemaphore, &sem_lock, 1));


//#define SEMAPHORE_LOCKING

namespace bitpowder {
namespace lib {

class Semaphore {
    NO_COPY(Semaphore);
    CSemaphore s;
#ifdef SEMAPHORE_LOCKING
    int waiting;
    int posted;
    SpinLock m;
#endif
public:
    inline Semaphore(int initialValue) : s()
  #ifdef SEMAPHORE_LOCKING
      , waiting(0), posted(0)
  #endif
    {
        semaphoreInit(&s, initialValue);
    }
    inline ~Semaphore() {
        semaphoreDestroy(&s);
    }
    inline void signal() {
#ifdef SEMAPHORE_LOCKING
        SpinLock::Locker l(m);
        if (waiting == 0) {
            posted++;
            return;
        }
        waiting--;
        l.unlock();
#endif
        semaphoreSignal(&s);

    }
    inline void wait() {
#ifdef SEMAPHORE_LOCKING
        SpinLock::Locker l(m);
        if (posted > 0) {
            posted--;
            return;
        }
        waiting++;
        l.unlock();
#endif
        semaphoreWait(&s);
    }
};

}
}

#endif

#endif

