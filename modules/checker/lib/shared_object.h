/*
 *  shared_object.h
 *  SpeedHTTPd
 *
 *  Created by Bernard van Gastel on 06-03-14.
 *  Copyright 2014 Bit Powder. All rights reserved.
 *
 */

#ifndef SHARED_OBJECT_H
#define SHARED_OBJECT_H

#include "common.h"
#include <atomic>
#include <iostream>

namespace bitpowder {
namespace lib {

class concurrent_count {
    std::atomic<int> refcount;
public:
    concurrent_count(int refcount = 0) : refcount(refcount) {
    }

    int operator++(int) {
        return refcount.fetch_add(1, std::memory_order_relaxed);
        //return refcount++;
    }

    int operator--() {
        int count = refcount.fetch_sub(1, std::memory_order_relaxed);
        return count-1;
        //return --refcount;
    }

    operator int() {
        return refcount;
    }
};

template <class Action>
class ref_count_with_action {
    std::atomic<int> refcount;
    Action action;
public:
    ref_count_with_action(Action &&action) : refcount(0), action(std::forward<Action>(action)) {
    }

    void operator++(int) {
        refcount++;
    }

    int operator--() {
        int count = --refcount;
        if (count == 0)
            action();
        return count;
    }

    operator int() {
        return refcount;
    }
};

template <class Type>
class count_proxy {
    Type& refcount;
public:
    count_proxy(Type& refcount) : refcount(refcount) {
    }

    int operator++(int) {
        return refcount++;
    }

    int operator--() {
        return --refcount;
    }

    operator int() {
        return refcount;
    }
};

/**
 * shared_object is a smart pointer using storage of the object pointed to, so it has minimal overhead.
 *
 * std::shared_ptr uses a indirection, so it can lead to memory fragmentation. The default manner of std::shared_ptr
 * to use std::make_shared to allocate at once is at odds with the ability of MemoryPool to reserve memory for a
 * class (as more bytes are needed). Also there is no choice of type of counter used (concurrent or single threaded),
 * and there is no ability to redirect to another counter (useful with multiple inheritance).
 */
template <class Object, class Base = Object, class RefCountType = decltype(Base::refcount), RefCountType Base::*ptr = &Base::refcount, void (*deleter)(Base*) = &performDelete<Base>>
class shared_object {
    Object *object;
public:
    /*
    shared_object(T &&o) : object(new T(std::forward<T>(o))) {
        std::cout << "emplace" << std::endl;
        (object->*ptr)++;
    }
    */
    shared_object() : object(nullptr) {
    }
    shared_object(Object *object) : object(object) {
        //std::cout << "pointer" << std::endl;
        if (object != nullptr)
            (object->*ptr)++;
    }
    ~shared_object() {
        //std::cout << "dtor" << std::endl;
        if (object != nullptr && --(object->*ptr) == 0) {
            deleter(object);
            object = nullptr;
        }
    }
    shared_object(const shared_object &o) : object(o.object) {
        //std::cout << "copy" << std::endl;
        if (object != nullptr)
            (object->*ptr)++;
    }
    shared_object(shared_object &&o) : object(nullptr) {
        //std::cout << "move constructor" << std::endl;
        std::swap(object, o.object);
    }

    shared_object& operator=(shared_object &&o) {
        //std::cout << "move assignment" << std::endl;
        std::swap(object, o.object);
        return *this;
    }
    shared_object& operator=(const shared_object &o) {
        //std::cout << "assignment" << std::endl;
        if (object && --(object->*ptr) == 0)
            deleter(object);
        object = o.object;
        if (object)
            (object->*ptr)++;
        return *this;
    }
    /*
    shared_object& operator=(T &&o) {
        std::cout << "move assignment with alloc" << std::endl;
        if (object != nullptr && --(object->*ptr) == 0)
            delete object;
        object = new T(std::move(o));
        (object->*ptr)++;
        return *this;
    }
    */
    operator Object*() const {
        return object;
    }
    Object& operator*() const {
        return *object;
    }
    Object* operator->() const {
        return object;
    }
    explicit operator bool() const {
        return object != nullptr;
    }
    bool operator==(const shared_object &o) const {
        return object == o.object;
    }
    bool operator==(const std::nullptr_t &o) const {
       return object == nullptr;
    }

    bool operator!=(const shared_object &o) const {
        return !(*this == o);
    }
    bool operator!=(const std::nullptr_t &o) const {
        return object != nullptr;
    }
    bool empty() const {
        return object == nullptr;
    }
    Object* get() const {
        return object;
    }
};

}
}

namespace std {

template <class Object, class Base, class RefCountType, RefCountType Base::*ptr, void (*deleter)(Base*)>
void swap(bitpowder::lib::shared_object<Object, Base, RefCountType, *ptr, *deleter>& lhs, bitpowder::lib::shared_object<Object, Base, RefCountType, *ptr, *deleter>& rhs) {
    std::swap(lhs.object, rhs.object);
}

template <class Object, class Base, class RefCountType, RefCountType Base::*ptr, void (*deleter)(Base*)>
class remove_pointer<bitpowder::lib::shared_object<Object,Base,RefCountType,ptr,deleter>> {
public:
    typedef Object type;
};

}

#endif // SHARED_OBJECT_H
