/*
 *  dispatch.cpp
 *  server-cpp
 *
 *  Created by Bernard van Gastel on 29-04-10.
 *  Copyright 2010 Bit Powder. All rights reserved.
 *
 */

#include "dispatch.h"

#include "stack.h"
#include "common.h"
#include "spinlock.h"
#include "thread.h"
#include <stdio.h>
#include <stdlib.h>
//#include <semaphore.h>
#include <errno.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/sysctl.h>
#include <unistd.h>

#ifdef EXTRA_DEBUG
#include "exception.h"
#endif

#ifdef PROFILING
#include <google/profiler.h>
#endif


//#define DEBUG

// for MainDispatcher
#ifdef __APPLE__
#ifdef __BLOCKS__
// bugfix for GCC: http://stackoverflow.com/questions/26527077/compiling-with-accelerate-framework-on-osx-yosemite
#ifndef __has_extension
#define __has_extension(x) 0
#endif

#include <dispatch/dispatch.h>
#endif
#endif

namespace bitpowder {
namespace lib {

const Dispatch::Priority Dispatch::PRIORITY_HIGH;
const Dispatch::Priority Dispatch::PRIORITY_MEDIUM;
const Dispatch::Priority Dispatch::PRIORITY_LOW;
const Dispatch::Priority Dispatch::priorities;

#ifdef __BLOCKS__
void DispatchBlock::execute() {
    func();
}
#endif

#ifdef __APPLE__
#ifdef __BLOCKS__
MainDispatcher::MainDispatcher() : lib::DispatcherFor<lib::DispatchOperation*,lib::Dispatch::priorities>("dispatcher-to-main-thread", 1)
{
}

void MainDispatcher::execute()
{
    DispatchOperation *e = nullptr;
    auto l = m.locker();
    do {
        try {
            while ((e = fetch())) {
                l.unlock();
                e->execute();
                l.lock(m);
            }
        } catch (Exception &exp) {
            std::cerr << exp << std::endl;
            if (e)
                e->aborted(exp);
            l.lock(m);
        } catch (const std::string &str) {
            if (e) {
                Exception exp(str);
                e->aborted(exp);
            }
            l.lock(m);
        } catch (...) {
            if (e) {
                Exception empty;
                e->aborted(empty);
            }
            l.lock(m);
        }
    } while (e);
    dispatched = false;
}

MainDispatcher* MainDispatcher::main() {
    static MainDispatcher* mainDispatcher = new MainDispatcher();
    return mainDispatcher;
}

void MainDispatcher::dispatchQueue(DispatchObjectQueue& queue, lib::Dispatch::Priority priority) {
    if (priority >= Dispatch::priorities)
        priority = Dispatch::priorities-1;
    auto l = m.locker();
    q[priority].splice(queue);
    l.unlock();
    if (!dispatched.exchange(true)) {
        dispatch_async(dispatch_get_main_queue(), ^{
            MainDispatcher::main()->execute();
        });
    }
}
#endif
#endif

GlobalDispatcher::GlobalDispatcher(int maximumExecuting) : DispatcherFor("GlobalDispatcher", maximumExecuting), cv(m) {
    //std::cerr << "dispatcher inited for " << maximumExecuting << " threads..." << std::endl;
}

void GlobalDispatcher::run() {
    global()->doRun();
}

GlobalDispatcher *GlobalDispatcher::global() {
    static GlobalDispatcher *singleton = new GlobalDispatcher(0);
    return singleton;
}

SerialDispatchQueue *SerialDispatchQueue::global() {
    static SerialDispatchQueue *singleton = new SerialDispatchQueue("global-serial-dispatch-queue", nullptr);
    return singleton;
}

#ifndef NDEBUG
std::atomic<long long> signals(0);
#endif
/*
void GlobalDispatcher::dispatch(DispatchOperation *o, unsigned int priority) {
    DispatchObjectQueue queue;
    queue.push(o);
    dispatchQueue(queue, priority);
}
*/

void GlobalDispatcher::dispatchQueue(DispatchObjectQueue &queue, Dispatch::Priority priority) {
    if (priority >= Dispatch::priorities)
        priority = Dispatch::priorities-1;
    //DispatchAction trigger = dispatchGlobalQueue.dispatchTo(o);
    int count = 0;
    for (DispatchObjectQueue::iterator it = queue.begin(); it != queue.end(); ++it)
        count++;
    //check(count > 0);
    auto l = m.locker();
    // can there be an extra runners
    q[priority].splice(queue);
    available += count;
    bool notify_all = waiting < count && waiting > 0;
    l.unlock();
    if (notify_all)
        cv.notify_all();
    else while (count-- > 0)
        cv.notify_one();
}

void GlobalDispatcher::doRun() {
#ifdef PROFILING
    ProfilerRegisterThread();
#endif
    DispatchOperation *e = nullptr;
    auto l = m.locker();
    executing++;
    if (executing > maximumExecuting)
        maximumExecuting++;
    while (1) {
        try {
            while (1) {
                while (!(e = fetch())) {
                    ++waiting;
                    cv.wait(l);
                    --waiting;
                }
                l.unlock();
                e->execute();
                l.lock(m);
            }
        } catch (Exception &exp) {
            std::cerr << exp << std::endl;
            if (e)
                e->aborted(exp);
            l.lock(m);
        } catch (const std::string &str) {
            if (e) {
                Exception exp(str);
                e->aborted(exp);
            }
            l.lock(m);
        } catch (...) {
            if (e) {
                Exception empty;
                e->aborted(empty);
            }
            l.lock(m);
        }
    }
}

void GlobalDispatcher::ensure(int absolute, int multiplier)
{
    int total = absolute + std::thread::hardware_concurrency() * multiplier;
    GlobalDispatcher *g = global();
    auto lock = g->m.locker();
    if (g->maximumExecuting < total) {
        //std::cerr << "raising parrallelism of DispatchQueues to " << total << std::endl;
        while (g->maximumExecuting < total) {
            Thread::start<GlobalDispatcher::run>();
            g->maximumExecuting++;
        }
        lock.unlock();
    }
}

class Worker : public DispatchQueueFor<shared_object<Work>> {
    concurrent_count refcount;
    Semaphore sem;
public:
    Worker(int max = std::thread::hardware_concurrency());
    void dispatchQueue(DispatchObjectQueue &queue, Dispatch::Priority priority = Dispatch::PRIORITY_MEDIUM);
    void executeOn(shared_object<Work> item);
    void abortedOn(shared_object<Work> item, Exception &e);
    void dispatchSelf(Dispatcher<DispatchOperation*> *t, Dispatch::Priority priority);
    void execute();
    void wait();
};

Worker::Worker(int max) : DispatchQueueFor<shared_object<Work>>("worker", max), sem(0) {
}

void Worker::dispatchQueue(Dispatcher::DispatchObjectQueue &queue, Dispatch::Priority priority) {
    for (const auto &it : queue)
        refcount++;
    DispatchQueueFor::dispatchQueue(queue, priority);
}

void Worker::executeOn(shared_object<Work> item) {
    item->execute();
    --refcount;
}

void Worker::abortedOn(shared_object<Work> item, Exception &e) {
    item->aborted(e);
    --refcount;
}

void Worker::dispatchSelf(Dispatcher<DispatchOperation *> *t, Dispatch::Priority priority) {
    refcount++;
    DispatchQueueFor::dispatchSelf(t, priority);
}

void Worker::execute() {
    DispatchQueueFor::execute();
    if (--refcount == 0) {
        sem.signal();
    }
}

void Worker::wait() {
    sem.wait();
}

void parallel(WorkQueue &allItems) {
    Worker worker;
    GlobalDispatcher::ensure(0, 1);
    worker.dispatchQueue(allItems);
    worker.wait();
}

}
}
