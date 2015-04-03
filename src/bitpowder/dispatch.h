/**
Copyright 2010-2015 Bernard van Gastel, bvgastel@bitpowder.com.
This file is part of Bit Powder Libraries.

Bit Powder Libraries is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Bit Powder Libraries is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Bit Powder Libraries.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _DISPATCH_H_
#define _DISPATCH_H_

#include "common.h"
#include "spinlock.h"
#include "lock.h"
#include "queue.h"
#include "stack.h"
#include "atomic.h"
#include "semaphore.h"
#include "thread.h"
#include "shared_object.h"
#include "assert.h"
#include <algorithm>
#include <atomic>

namespace bitpowder {
namespace lib {

class DispatchOperation {
    NO_COPY(DispatchOperation);
public:
    DispatchOperation *next = nullptr;
    DispatchOperation() {
    }
    virtual ~DispatchOperation() {
    }
    virtual void execute() = 0;
    virtual void aborted(Exception &e) = 0;
};

template <class T>
class DispatchLambdaWrapper : public DispatchOperation {
    T lambda;
public:
    DispatchLambdaWrapper(T&& lambda) : lambda(std::move(lambda)) {
    }
    void execute() {
        lambda();
        delete this;
    }
    void aborted(Exception &e) {
        delete this;
    }
};

template <class T>
DispatchLambdaWrapper<T> *DispatchLambda(T&& lambda) {
    return new DispatchLambdaWrapper<T>(std::move(lambda));
}

#ifdef __BLOCKS__
#include <Block.h>

class DispatchBlock : public DispatchOperation {
    void (^func)(void);
    bool valid:1;
public:
    DispatchBlock(void (^func2)(void) = nullptr) : func(), valid(false) {
        if (func2) {
            func = func2;
            valid = true;
        }
    }
    virtual ~DispatchBlock() {
        if (valid)
            Block_release(func);
    }
    inline void set(void (^func2)(void)) {
        if (func && valid)
            Block_release(func);
        if (func2) {
            func = func2;
        }
        valid = func2; // != nullptr;
    }
    void execute();
};
#endif

class Dispatch {
public:
    typedef unsigned short Priority;
    static const Priority PRIORITY_TIME = 4;
    static const Priority PRIORITY_EVENT = 3;
    static const Priority PRIORITY_HIGH = 2;
    static const Priority PRIORITY_MEDIUM = 1;
    static const Priority PRIORITY_LOW = 0;
    static const Priority priorities = 5;
};

template <class DispatchObject>
class Dispatcher {
public:
    typedef Queue<DispatchObject> DispatchObjectQueue;
    void dispatch(DispatchObject o, Dispatch::Priority priority = Dispatch::PRIORITY_MEDIUM) {
        DispatchObjectQueue queue;
        queue.push(o);
        dispatchQueue(queue, priority);
    }
    virtual void dispatchQueue(DispatchObjectQueue &queue, Dispatch::Priority priority = Dispatch::PRIORITY_MEDIUM) = 0;
};

template <class DispatchObject, Dispatch::Priority numberOfPriorities = 1>
class DispatcherFor : public Dispatcher<DispatchObject> {
protected:
    int maximumExecuting;

    typename Dispatcher<DispatchObject>::DispatchObjectQueue q[numberOfPriorities];
    int executing; // currently executing
    const char *desc;
protected:
    DispatcherFor(const char *description, int maximumExecuting) : maximumExecuting(maximumExecuting), q(), executing(0), desc(description) {
        typename Dispatcher<DispatchObject>::DispatchObjectQueue emptyQueue;
        for (Dispatch::Priority i = 0; i < numberOfPriorities; ++i)
            q[i].clear();
    }
    virtual ~DispatcherFor() {
    }
    void fetch(typename Dispatcher<DispatchObject>::DispatchObjectQueue &target) {
        if (maximumExecuting == 1 && !q[numberOfPriorities-1].empty()) {
            target.splice(q[numberOfPriorities-1]);
            return;
        }
        DispatchObject object = maximumExecuting == 1 ? fetch(numberOfPriorities-1) : fetch(numberOfPriorities);
        if (object)
            target.push(object);
    }
    DispatchObject fetch(Dispatch::Priority startQueue = numberOfPriorities) {
        for (Dispatch::Priority i = startQueue; i-- > 0; )
            if (!q[i].empty())
                return q[i].pop();
        return nullptr;
    }
public:
    const char* description() const {
        return desc;
    }
};

#ifdef __APPLE__
#ifdef __BLOCKS__
class MainDispatcher : public lib::DispatcherFor<lib::DispatchOperation*,lib::Dispatch::priorities> {
protected:
    bool dispatched = false;
    Lock m;
    MainDispatcher();
    void execute();
public:
    static MainDispatcher* main();

    static void mainDispatch(lib::DispatchOperation *o, lib::Dispatch::Priority priority = lib::Dispatch::PRIORITY_MEDIUM) {
        main()->dispatch(o, priority);
    }
    static void mainDispatchQueue(DispatchObjectQueue &o, lib::Dispatch::Priority priority = lib::Dispatch::PRIORITY_MEDIUM) {
        main()->dispatchQueue(o, priority);
    }
    virtual void dispatchQueue(DispatchObjectQueue &queue, lib::Dispatch::Priority priority = lib::Dispatch::PRIORITY_MEDIUM);
};
#endif
#endif

class GlobalDispatcher : public DispatcherFor<DispatchOperation*,Dispatch::priorities> {
protected:
    friend class DispatchQueueInit;
    Lock m;
    Lock::ConditionVariable cv;
    int available = 0;
    int waiting = 0;
    GlobalDispatcher(int maximumExecuting);
    void doRun();
public:
    static void ensure(int absolute, int multiplier = 0);
    static void run();
    inline static void start() {
        Thread::start<&GlobalDispatcher::run>();
    }
    static GlobalDispatcher *global();
    // needed for a std::for_each
    static void globalDispatch(DispatchOperation *o, Dispatch::Priority priority = Dispatch::PRIORITY_MEDIUM) {
        global()->dispatch(o, priority);
    }
    static void globalDispatchQueue(DispatchObjectQueue &o, Dispatch::Priority priority = Dispatch::PRIORITY_MEDIUM) {
        global()->dispatchQueue(o, priority);
    }
    //virtual bool dispatch(DispatchOperation *o, Dispatch::Priority priority = Dispatch::PRIORITY_MEDIUM);
    virtual void dispatchQueue(DispatchObjectQueue &queue, Dispatch::Priority priority = Dispatch::PRIORITY_MEDIUM);
};

inline void dispatch(DispatchOperation *operation, Dispatch::Priority priority = Dispatch::PRIORITY_MEDIUM) {
    bitpowder::lib::unused(priority);
    GlobalDispatcher::globalDispatch(operation);
}

// in cases where a DispatchQueue is not enough (e.g. maximum x tasks, but not maximum x tasks running)
class DispatchLimiter {
    SpinLock m;
    Queue<DispatchOperation*> q;
    int left;
public:
    DispatchLimiter(int max) : m(), q(), left(max) {
    }

    void dispatch(DispatchOperation *op) {
        SpinLock::Locker l(m);
        bool doDispatch = left > 0;
        if (doDispatch) {
            left--;
        } else {
            q.push(op);
        }
        l.unlock();
        if (doDispatch)
            GlobalDispatcher::globalDispatch(op);
    }

    void trigger() {
        DispatchOperation *op = nullptr;
        SpinLock::Locker l(m);
        if (!q.empty()) {
            op = q.pop();
        } else {
            left++;
        }
        l.unlock();
        if (op)
            GlobalDispatcher::globalDispatch(op);
    }

    void trigger(int count) {
        Queue<DispatchOperation*> op;
        SpinLock::Locker l(m);
        while (count > 0 && !q.empty()) {
            op.push(q.pop());
            count--;
        }
        left += count;
        l.unlock();
        GlobalDispatcher::globalDispatchQueue(op);
    }
};


// DispatchQueue<templated>

//template <class On, class Object, void (Object::*executionFunction)(On *on) = &Object::execute, void (Object::*exceptionFunction)(On *on, Exception &e) = &Object::aborted>
template <class On, Dispatch::Priority priorities = 1>
class DispatchQueueFor : public DispatcherFor<On, priorities>, public DispatchOperation {
protected:
    std::atomic<Dispatcher<DispatchOperation*>*> targetQueue;
    std::atomic<int> targetPriority;
    bool dispatched; // is a new runner being dispatched?
    bool active;

    SpinLock m;

    virtual void executeOn(On object) = 0;
    virtual void abortedOn(On object, Exception &e) = 0;
    virtual void dispatchSelf(Dispatcher<DispatchOperation*> *t, Dispatch::Priority priority) {
        t->dispatch(this, priority);
    }

    void aborted(Exception &e) {
        // failure is not an option, otherwise something very strange is happening
        bitpowder::lib::unused(e);
        perror("aborted in DispatchQueueFor, should not happen");
        abort();
    }

    void execute() {
        typename DispatcherFor<On>::DispatchObjectQueue e;
        //std::cout << "[Dispatch:" << this << "] waiting to begin executing dispatch queue" << std::endl;
        SpinLock::Locker l(this->m);
        //std::cout << "[Dispatch:" << this << "] begin executing dispatch queue" << std::endl;
        //fprintf(stderr, "begin executing dispatch queue %p: %i/%i\n", this, executing, maximumExecuting);
        dispatched = false;
        checkAssert(this->executing < this->maximumExecuting);
        this->executing++;
        // qLength > 1 because there will always be an item executed in the following code
        unsigned int count = 0;
        for (int i = 0; i < priorities && count < 2; ++i)
            count += this->q[i].multipleItems() ? 2 : !this->q[i].empty() ? 1 : 0;
        if (active && count >= 2 && this->executing < this->maximumExecuting) {
            // there are more items, and there is room for another executing dispatch, so do it
            dispatched = true;
            Dispatcher<DispatchOperation*> *t = targetQueue;
            Dispatch::Priority p = priority();
            l.unlock();
            dispatchSelf(t, p);
            l.lock(this->m);
        }
        while (true) {
            On op = nullptr;
            try {
                while (true) {
                    if (active && e.empty()) {
                        if (this->maximumExecuting == 1)
                            // order is not changed if we take all, more efficient
                            DispatcherFor<On, priorities>::fetch(e);
                        else
                            e.push(DispatcherFor<On, priorities>::fetch());
                    }
                    if (e.empty())
                        goto end;
                    l.unlock();
                    while (!e.empty()) {
                        op = e.pop();
                        //std::cout << "[Dispatch:" << this << "] executing " << op << std::endl;
                        executeOn(op);
                    }
                    //if (stop)
                        //goto end;
                    l.lock(this->m);
                }
            } catch (Exception& exc) {
                if (op)
                    abortedOn(op, exc);
            } catch (std::exception& e) {
                if (op) {
                    Exception exc(e.what());
                    abortedOn(op, exc);
                }
                l.lock(this->m);
            } catch (std::string& str) {
                if (op) {
                    Exception exc(str);
                    abortedOn(op, exc);
                }
            } /*catch (...) {
                std::cerr << "catch all" << std::endl;
                if (op) {
                    try {
                        Exception empty;
                        abortedOn(op, empty);
                    } catch (...) {
                    }
                }
            }*/
            l.lock(this->m);
        }
        //fprintf(stderr, "end executing dispatch queue: %i\n", debug);
end:
        checkAssert(this->executing > 0);
        this->executing--;
    }
public:
    DispatchQueueFor(const char *description, int maximumExecuting = 1, Dispatcher<DispatchOperation*> *targetQueue = GlobalDispatcher::global(), Dispatch::Priority priority = Dispatch::PRIORITY_MEDIUM) : DispatcherFor<On>(description, maximumExecuting), targetQueue(targetQueue), targetPriority(priority), dispatched(false), active(true) {
    }
    virtual ~DispatchQueueFor() {
    }
    void dispatchQueue(typename Dispatcher<On>::DispatchObjectQueue &queue, Dispatch::Priority priority = Dispatch::PRIORITY_MEDIUM) {
        if (queue.empty())
            return;
        if (priority >= priorities)
            priority = priorities-1;
        Dispatcher<DispatchOperation*> *t = nullptr;
        int tPriority = Dispatch::PRIORITY_MEDIUM;
        SpinLock::Locker l(this->m);
        // can there be an extra runner?
        //std::cout << "[Dispatch:" << this << "] executing: " << this->executing << " < " << this->maximumExecuting << " and already dispatched: " << dispatched << std::endl;
        if (active && this->executing < this->maximumExecuting && !dispatched) {
            // if already dispatched, do not dispatch again, this will result in errors (as a DispatchOperation can only be enqueued once)
            dispatched = true;
            t = targetQueue;
            tPriority = targetPriority;
        }
        this->q[priority].splice(queue);
        l.unlock();
        //eprintf("start queue %p = %i\n", this, action);
        if (t)
            dispatchSelf(t, tPriority);
    }

    inline Dispatcher<DispatchOperation*> *target(Dispatcher<DispatchOperation*> *targetQueue) {
        return this->targetQueue.exchange(targetQueue);
    }
    inline void start() {
        Dispatcher<DispatchOperation*> *t = nullptr;
        int tPriority = Dispatch::PRIORITY_MEDIUM;
        SpinLock::Locker l(this->m);
        //std::cout << "starting dispatch queue; active=" << active << " dispatched=" << dispatched << std::endl;
        if (!active && !dispatched) {
            //std::cout << "started dispatch queue" << std::endl;
            active = true;
            dispatched = true;
            t = targetQueue;
            tPriority = targetPriority;
        }
        l.unlock();
        if (t)
            dispatchSelf(t, tPriority);
    }
    inline void pause() {
        SpinLock::Locker l(this->m);
        active = false;
    }

    inline Dispatcher<DispatchOperation> *target() const {
        Dispatcher<DispatchOperation> *q = targetQueue;
        return q;
    }

    inline Dispatch::Priority priority(Dispatch::Priority newPriority) {
        return this->targetPriority.exchange(newPriority);
    }

    inline Dispatch::Priority priority() const {
        Dispatch::Priority p = targetPriority;
        return p;
    }
};

class DispatchQueue : public DispatchQueueFor<DispatchOperation*> {
public:
    DispatchQueue(const char *description, int maximumExecuting = 1, Dispatcher *targetQueue = GlobalDispatcher::global(), Dispatch::Priority priority = Dispatch::PRIORITY_MEDIUM) : DispatchQueueFor<DispatchOperation*>(description, maximumExecuting, targetQueue, priority) {
    }
    void executeOn(DispatchOperation *op) {
        if (op)
            op->execute();
    }
    void abortedOn(DispatchOperation *op, Exception &e) {
        if (op)
            op->aborted(e);
    }
};

// FIXME: avoid having a public stop method, by adding a special wrapper around DispatchOperation (that can be fast checked by hxtension.h), called SerialDispatchOperation, which gets a special argument SerialDispatchQueueControl, which has a method stop(). This way the API will force correct use of the stop method below.
class SerialDispatchQueue : public DispatchQueue {
public:
    class Delegate {
    public:
        virtual void stopped() = 0;
    };
    Delegate *onStop;
protected:
    std::atomic<bool*> shouldStop;
public:
    SerialDispatchQueue(const char *description, Delegate *onStop, Dispatcher *targetQueue = GlobalDispatcher::global(), Dispatch::Priority priority = Dispatch::PRIORITY_MEDIUM) : DispatchQueue(description, 1, targetQueue, priority), onStop(onStop), shouldStop(nullptr) {
    }
    void execute() {
        bool shouldStop = false;
        this->shouldStop.exchange(&shouldStop);
        DispatchQueue::execute();
        if (shouldStop && onStop)
            onStop->stopped();
        else
            this->shouldStop.exchange(nullptr);
    }
    // ONLY CALL FROM DISPATCH QUEUE ITSELF!
    void stop() {
        bool *shouldStop = this->shouldStop;
        if (!shouldStop)
            throw Exception("stop() on SerialDispatchQueue called from outside DispatchQueue or thread");
        *shouldStop = true;
    }
    static SerialDispatchQueue *global();
};

class SerialDispatchQueue2 : public DispatchQueue {
public:
    SerialDispatchQueue2(const char *description, Dispatcher *targetQueue = GlobalDispatcher::global(), Dispatch::Priority priority = Dispatch::PRIORITY_MEDIUM) : DispatchQueue(description, 1, targetQueue, priority) {
    }
};

// Useful for GUI applications in a tight control/display loop, or if the control is external in some way (e.g. external events)
template <class On, Dispatch::Priority priorities>
class SerialDispatchBuffer : public DispatcherFor<On, priorities> {
    SerialDispatchBuffer(const char *description) : DispatcherFor<On, priorities>(description, 1) {
    }

    template <typename... Args>
    void execute(Args... args) {
        typename DispatcherFor<On>::DispatchObjectQueue e;
        SpinLock::Locker l(this->m);
        while (true) {
            On *op = nullptr;
            try {
                while (true) {
                    if (e.empty())
                        DispatcherFor<On, priorities>::fetch(e);
                    if (e.empty())
                        goto end;
                    l.unlock();
                    while (!e.empty()) {
                        op = e.pop();
                        //std::cout << "[Dispatch:" << this << "] executing " << op << std::endl;
                        op(std::forward(args...));
                    }
                    l.lock(this->m);
                }
            } catch (Exception& exc) {
                if (op)
                    abortedOn(op, exc);
                l.lock(this->m);
            } catch (std::exception& e) {
                if (op) {
                    Exception exc(e.what());
                    abortedOn(op, exc);
                }
                l.lock(this->m);
            } catch (std::string& str) {
                if (op) {
                    Exception exc(str);
                    abortedOn(op, exc);
                }
                l.lock(this->m);
            } catch (...) {
                if (op) {
                    Exception empty;
                    abortedOn(op, empty);
                }
                l.lock(this->m);
            }
        }
end:
        return;
    }
};

// SUPPORT FOR LAMBDA FUNCTIONS

template <class X>
class OperationOn : public DispatchOperation {
    template <class Y, class Z>
    friend class RemoteDispatchQueueBase;
    X *operationOn;
public:
    OperationOn() : operationOn(nullptr) {
    }
    virtual void executeOn(X *) = 0;
    virtual void abortedOn(X *, Exception &) = 0;
    void execute() {
        X *operationOn = this->operationOn;
        this->operationOn = nullptr;
        executeOn(operationOn);
    }
    void aborted(Exception &e) {
        X *operationOn = this->operationOn;
        this->operationOn = nullptr;
        abortedOn(operationOn, e);
    }
};

template <class On, class T>
class DispatchLambdaOnWrapper : public OperationOn<On> {
    T lambda;
public:
    DispatchLambdaOnWrapper(T lambda) : lambda(lambda) {
    }
    void executeOn(On *on) {
        lambda(on);
        delete this;
    }
    void abortedOn(On *on, Exception &e) {
        delete this;
    }
};

template <class On, class T>
DispatchLambdaOnWrapper<On, T> *DispatchLambdaOn(T lambda) {
    return new DispatchLambdaOnWrapper<On, T>(lambda);
}

// REMOTE DISPATCH QUEUE

template <class X, class Y>
class RemoteDispatchQueueBase : DispatchOperation {
protected:
    template <class A, class B>
    friend class RemoteDispatchQueueBase;
    X *self;
    std::atomic<RemoteDispatchQueueBase<Y,X>*> other;
    DispatchQueue &queue;
    Dispatch::Priority priority;
    static RemoteDispatchQueueBase<Y,X>* EMPTY;
    static RemoteDispatchQueueBase<Y,X>* DISCONNECTED;
    std::atomic<bool> dispatched;

    void execute() {
        //std::cerr << "[RemoteDispatchQueueBase: " << this << "] executing disconnect operation" << std::endl;
        dispatched = false;
        RemoteDispatchQueueBase<Y,X> *old;
        while ((old = other.exchange(nullptr)) == nullptr) {
        }
        if (old == DISCONNECTED) {
        //checkAssert(old == DISCONNECTED);
            other.exchange(EMPTY);
            checkAssert(next == nullptr);
            disconnected();
        } else
            other.exchange(old);
    }
    void aborted(Exception &e) {
        //std::cerr << "[RemoteDispatchQueueBase: " << this << "] aborting disconnect operation" << std::endl;
    }
public:
    RemoteDispatchQueueBase(X *self, DispatchQueue &queue, Dispatch::Priority priority = Dispatch::PRIORITY_MEDIUM) : DispatchOperation(), self(self), other(EMPTY), queue(queue), priority(priority), dispatched(false) {
        checkAssert(next == nullptr);
    }
    ~RemoteDispatchQueueBase() {
        disconnect();
    }

    bool remoteDispatch(OperationOn<Y> *op) {
        RemoteDispatchQueueBase<Y,X> *o = nullptr;
        while ((o = other.exchange(o)) == nullptr) {
        }
        assert(o != nullptr);
        if (o == EMPTY || o == DISCONNECTED) {
            o = other.exchange(o);
            assert(o == nullptr);
            //std::cout << "could not remote dispatch, not connected" << std::endl;
            return false;
        }
        //std::cerr << "[RemoteDispatchQueueBase: " << this << "] remote dispatching " << op << std::endl;
        op->operationOn = o->self;
        o->queue.dispatch(op, priority);
        o = other.exchange(o);
        checkAssert(o == nullptr);
        return true;
    }
    void connect(RemoteDispatchQueueBase<Y,X> *newOther) {
        //std::cerr << "[RemoteDispatchQueueBase: " << this << "] connect with " << newOther << std::endl;
        if ((void*)newOther > (void*)this) {
            RemoteDispatchQueueBase<Y,X> *o = nullptr;
            while ((o = other.exchange(o)) == nullptr) {
            }
            checkAssert(o == EMPTY || o == DISCONNECTED);
            RemoteDispatchQueueBase<X,Y> *o2 = nullptr;
            while ((o2 = newOther->other.exchange(o2)) == nullptr) {
            }
            checkAssert((o2 == RemoteDispatchQueueBase<Y,X>::EMPTY));

            other.exchange(newOther);
            newOther->other.exchange(this);
        } else {
            RemoteDispatchQueueBase<Y,X> *o = nullptr;
            while ((o = other.exchange(o)) == nullptr) {
            }
            checkAssert(o == EMPTY || o == DISCONNECTED);
            RemoteDispatchQueueBase<X,Y> *o2 = nullptr;
            while ((o2 = newOther->other.exchange(o2)) == nullptr) {
            }
            checkAssert((o2 == RemoteDispatchQueueBase<Y,X>::EMPTY));

            other.exchange(newOther);
            newOther->other.exchange(this);
        }
    }
    void disconnect() {
        //std::cerr << "[RemoteDispatchQueueBase: " << this << "] disconnect" << std::endl;
restart:
        RemoteDispatchQueueBase<Y,X> *o = nullptr;
        while ((o = other.exchange(o)) == nullptr) {
        }
        if (o == EMPTY || o == DISCONNECTED) {
            o = other.exchange(o);
            assert(o == nullptr);
            return;
        }
        //std::cerr << "[RemoteDispatchQueueBase: " << this << "] disconnecting from " << o << std::endl;
        checkAssert(next == nullptr);
        if ((void*)o > (void*)this) {
            RemoteDispatchQueueBase<X,Y> *o2 = nullptr;
            while ((o2 = o->other.exchange(o2)) == nullptr) {
            }
            checkAssert(o2 == this);
            //std::cerr << "[RemoteDispatchQueueBase: " << this << "] next = " << next << std::endl;
            checkAssert(o->next == nullptr);
            //if (o->next != nullptr)
            //    CRASH;

            //std::cerr << "[RemoteDispatchQueueBase: " << this << "] scheduling disconnect operation (1): " << (void*)o << "/" << (void*)o->next << "/" << &o->queue << std::endl;
            // let the other party know it is disconnected
            o->other.exchange(RemoteDispatchQueueBase<Y,X>::DISCONNECTED);
            //o->other.exchange(RemoteDispatchQueueBase<Y,X>::EMPTY);
            other.exchange(EMPTY);
            if (!o->dispatched.exchange(true))
                o->queue.dispatch(o, priority);

            //std::cout << "[RemoteDispatchQueueBase: " << this << "] set own ref to empty (1)" << std::endl;
            //std::cout << "[RemoteDispatchQueueBase: " << this << "] set remote ref to empty (1)" << std::endl;
        } else {
            RemoteDispatchQueueBase<X,Y> *o2 = o->other.exchange(nullptr);
            if (o2 == nullptr) {
                other.exchange(o);
                goto restart;
            }
            checkAssert(o2 == this);
            checkAssert(o->next == nullptr);
            //if (o->next != nullptr)
            //    CRASH;

            //std::cerr << "[RemoteDispatchQueueBase: " << this << "] scheduling disconnect operation (2)" << std::endl;
            // let the other party know it is disconnected
            o->other.exchange(RemoteDispatchQueueBase<Y,X>::DISCONNECTED);
            //o->other.exchange(RemoteDispatchQueueBase<Y,X>::EMPTY);
            other.exchange(EMPTY);
            if (!o->dispatched.exchange(true))
                o->queue.dispatch(o, priority);
        }
    }
    bool isConnected() {
        RemoteDispatchQueueBase<Y,X> *o;
        while ((o = other.exchange(nullptr)) == nullptr) {
        }
        bool retval = o != EMPTY && o != DISCONNECTED;
        other.exchange(o);
        return retval;
    }
    virtual void disconnected() {
    }
};

template <class X, class Y>
RemoteDispatchQueueBase<Y,X>* RemoteDispatchQueueBase<X,Y>::EMPTY = (RemoteDispatchQueueBase<Y,X>*)-1;

template <class X, class Y>
RemoteDispatchQueueBase<Y,X>* RemoteDispatchQueueBase<X,Y>::DISCONNECTED = (RemoteDispatchQueueBase<Y,X>*)-2;


template <class X, class Y, void (X::*disconnectedMethod)()>
class RemoteDispatchQueue : public RemoteDispatchQueueBase<X,Y> {
public:
    RemoteDispatchQueue(X *self, DispatchQueue &queue, Dispatch::Priority priority = Dispatch::PRIORITY_HIGH) : RemoteDispatchQueueBase<X,Y>(self, queue, priority) {
    }
    void disconnected() {
        (RemoteDispatchQueueBase<X,Y>::self->*disconnectedMethod)();
    }
};

class Work {
public:
    concurrent_count refcount;
    shared_object<Work> next;
    Work() : next(nullptr) {
    }
    virtual ~Work() {
    }
    virtual void execute() = 0;
    virtual void aborted(Exception& e) = 0;
};

typedef Queue<shared_object<Work>> WorkQueue;

// executes items in parallel over the availible cores, and automatically deallocates the items by using the shared_object construct
void parallel(WorkQueue& allItems);

}
}

#endif

