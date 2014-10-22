/*
 *  queue.h
 *  SpeedHTTPd
 *
 *  Created by Bernard van Gastel on 08-05-10.
 *  Copyright 2010 Bit Powder. All rights reserved.
 *
 */

#ifndef _DEQUE_H_
#define _DEQUE_H_

#include "common.h"
#include "exception.h"
#include "queue.h"

#include <memory>
#include <algorithm>
#include <iterator>
#include <assert.h>

namespace bitpowder {
namespace lib {

template <class T, class X = typename std::remove_pointer<T>::type, class PtrContainer = typename std::remove_pointer<T>::type, T PtrContainer::*nextPtr = &PtrContainer::next, T PtrContainer::*prevPtr = &PtrContainer::prev>
class Deque {
    NO_COPY(Deque);
    T head;
    T tail;
public:
    Deque(T head = nullptr, T tail = nullptr) : head(head), tail(tail) {
        if (!tail)
            tail = head;
        if (tail)
            while (tail->*nextPtr)
                tail = (T)tail->*nextPtr;
    }
    ~Deque() {
        clear();
    }
    inline Deque(Deque &&q) : head(q.head), tail(q.tail) {
        q.head = q.tail = nullptr;
    }
    inline Deque& operator=(Deque &&q) {
        head = q.head;
        tail = q.tail;
        q.head = q.tail = nullptr;
        return *this;
    }
    void clear() {
        head = nullptr;
        tail = nullptr;
    }

    void swap(Deque &b) {
        using std::swap;
        swap(head, b.head);
        swap(tail, b.tail);
    }

    void swapReversed(Deque &reversed) {
        using std::swap;
        swap(reversed.head, tail);
        swap(reversed.tail, head);
    }

    void push_front(T e) {
        if (!e)
            return;
        assert(e->*nextPtr == nullptr);
        assert(e->*prevPtr == nullptr);
        if (empty()) {
            tail = e;
            e->*nextPtr = nullptr;
        } else {
            head->*prevPtr = e;
            e->*nextPtr = head;
        }
        head = e;
    }
    void push_front(Deque &d) {
        if (d.empty())
            return;
        if (empty()) {
            swap(d);
            return;
        }
        head->*prevPtr = d.back();
        d.back()->*nextPtr = head;
        head = d.front();
        d.clear();
    }
    void push_back(T e) {
        if (!e)
            return;
        assert(e->*nextPtr == nullptr);
        assert(e->*prevPtr == nullptr);
        if (empty()) {
            head = e;
            e->*prevPtr = nullptr;
        } else {
            tail->*nextPtr = e;
            e->*prevPtr = tail;
        }
        tail = e;
    }
    void push_back(Deque &d) {
        if (d.empty())
            return;
        if (empty()) {
            swap(d);
            return;
        }
        tail->*nextPtr = d.front();
        d.front()->*prevPtr = tail;
        tail = d.back();
        d.clear();
    }
    template <size_t N>
    void push_back_array(T (&d)[N]) {
        for (T &i : d)
            push_back(&i);
    }

    T pop_front() {
        T e = nullptr;
        std::swap(head, e);
        if (e) {
            std::swap(head, (T)e->*nextPtr);
            if (!head) {
                tail = nullptr;
            } else {
                head->*prevPtr = nullptr;
            }
            assert(e->*prevPtr == nullptr);
        }
        return e;
    }
    T pop_back() {
        T e = nullptr;
        std::swap(tail, e);
        if (e) {
            std::swap(tail, (T)e->*prevPtr);
            if (!tail) {
                head = nullptr;
            } else {
                tail->*nextPtr = nullptr;
            }
            assert(e->*nextPtr == nullptr);
        }
        return e;
    }

    inline Deque pop_all() {
        Deque retval;
        swap(retval);
        return retval;
    }

    void erase(T e) {
        //fprintf(stdout, "deque::erase; e=%p e->next=%p e->prev=%p head=%p tail=%p\n", &*e, &*(e->*nextPtr), &*(e->*prevPtr), &*head, &*tail);
        if (!e || (e->*prevPtr == nullptr && e->*nextPtr == nullptr && e != head))
            return;
        if (e == head) {
            assert(e->*prevPtr == nullptr);
            head = e->*nextPtr;
            if (head)
                head->*prevPtr = nullptr;
            else
                tail = nullptr;
        } else if (e == tail) {
            assert(e->*nextPtr == nullptr);
            tail = e->*prevPtr;
            if (tail)
                tail->*nextPtr = nullptr;
            else
                head = nullptr;
        } else {
            assert(e->*prevPtr);
            assert(e->*nextPtr);
            (e->*nextPtr)->*prevPtr = e->*prevPtr;
            //std::swap((e->*nextPtr)->*prevPtr, e->*prevPtr);
            std::swap((e->*prevPtr)->*nextPtr, e->*nextPtr);
        }
        e->*prevPtr = nullptr;
        e->*nextPtr = nullptr;
        //fprintf(stderr, "after -> e: %p; head: %p; tail: %p\n", e, head, tail);
    }

    T front() {
        return head;
    }
    T back() {
        return tail;
    }
    const T front() const {
        return head;
    }
    const T back() const {
        return tail;
    }

    // information
    bool empty() const {
        return !head;
    }

    // iterators
    class _iterator : public std::iterator<std::input_iterator_tag, T> {
        friend class Deque;
        friend class Value;
        Deque *d;
        X *value;
        bool increasing;
        //_iterator(const _iterator &it);
    public:
        _iterator(Deque *d, X *value) : d(d), value(value), increasing(true) {
        }
        _iterator(const _iterator &it) : d(it.d), value(it.value), increasing(it.increasing) {
        }
        X* operator*() {
            return value;
        }
        X* operator->() {
            return value;
        }
        _iterator &operator++() {
            if (value)
                value = &*value->*nextPtr;
            else if (!increasing)
                value = &*d->head;
            increasing = true;
            return *this;
        }
        _iterator &operator--() {
            if (value)
                value = &*value->*prevPtr;
            else if (increasing)
                value = &*d->tail;
            increasing = false;
            return *this;
        }
        bool operator==(const _iterator &b) const {
            return value == b.value;
        }
        bool operator!=(const _iterator &b) const {
            return value != b.value;
        }
        operator bool() const {
            return value;
        }
    };
    //template <class T>
    class _riterator : public _iterator {
    public:
        _riterator(Deque *d, X *value) : _iterator(d, value) {
        }
        _riterator(const _iterator &value) : _iterator(value) {
        }
        _riterator &operator++() {
            _iterator::operator--();
            return *this;
        }
        _riterator &operator--() {
            _iterator::operator++();
            return *this;
        }
    };

    //typedef _iterator<T> iterator;
    //typedef _iterator<const T> const_iterator;
    typedef _iterator iterator;
    /*
    const_iterator begin() const {
        return const_iterator(this, head);
    }
    const_iterator end() const {
        return ++const_iterator(this, tail);
    }
    */
    iterator begin() {
        return iterator(this, &*head);
    }
    iterator end() {
        return ++iterator(this, &*tail);
    }

    //typedef _riterator<T> reverse_iterator;
    //typedef _riterator<const T> const_reverse_iterator;
    typedef _riterator reverse_iterator;
    /*
    const_reverse_iterator rbegin() const {
        return const_reverse_iterator(this, tail);
    }
    const_reverse_iterator rend() const {
        return ++const_reverse_iterator(this, head);
    }
    */
    reverse_iterator rbegin() {
        return reverse_iterator(this, &*tail);
    }
    reverse_iterator rend() {
        return ++reverse_iterator(this, &*head);
    }

    void insert(iterator it, T b) {
        X *c = *it;
        insert(c, b);
    }

    template <class Func>
    void insertAt(X* object, Func &&f) {
        for (auto it : *this)
            if (f(it)) {
                insert(it, object);
                return;
            }
        push_back(object);
    }

    void insert(T pos, T b) {
        if (!b)
            return;
        if (!pos) {
            push_back(b);
            return;
        }

        b->*prevPtr = b;
        std::swap(b->*prevPtr, pos->*prevPtr);

        b->*nextPtr = b;
        if (b->*prevPtr) {
            assert(pos != head);
            // previous exists
            std::swap(b->*nextPtr, (b->*prevPtr)->*nextPtr);
        } else {
            assert(pos == head);
            // previous did not exists
            std::swap(b->*nextPtr, head);
        }
    }

    void insert(iterator it, Deque &&b) {
        T c = *it;
        insert(c, std::move(b));
    }

    void insert(T pos, Deque &&b) {
        if (b.empty())
            return;
        if (!pos) {
            push_back(b);
            return;
        }
        T a = pos->*prevPtr;
        if (a) {
            checkAssert(pos != head);
            a->*nextPtr = b.front();
        } else {
            checkAssert(pos == head);
            head = b.front();
        }
        b.front()->*prevPtr = a;
        b.back()->*nextPtr = pos;
        pos->*prevPtr = b.back();
        b.clear();
    }

    T next(T c) {
        return c ? c->*nextPtr : nullptr;
    }

    T prev(T c) {
        return c ? c->*prevPtr : nullptr;
    }

    // including
    Deque split(T pos) {
        T retvalHead = nullptr;
        T retvalTail = nullptr;
        if (!empty() && pos != nullptr) {
            retvalHead = head;
            head = pos->*nextPtr;
            pos->*nextPtr = nullptr;
            if (head)
                head->*prevPtr = nullptr;
            retvalTail = pos;
        }
        return Deque(retvalHead, retvalTail);
    }
};

template <class T>
class DequeItem {
public:
    T value;
    DequeItem *next;
    DequeItem *prev;
    DequeItem(const T &value) : value(value), next(nullptr), prev(nullptr) {
    }
    bool operator <(const DequeItem& b) const {
        return value < b.value;
    }
    bool operator==(const DequeItem &b) const {
        return b.value == value;
    }
    T operator*() const {
        return value;
    }
    bool operator ==(const T &b) {
        return value == b.value;
    }
};

}
}

namespace std {

template <class T, class X, class PtrContainer, T PtrContainer::*nextPtr, T PtrContainer::*prevPtr>
void swap(bitpowder::lib::Deque<T, X, PtrContainer, nextPtr, prevPtr>& lhs, bitpowder::lib::Deque<T, X, PtrContainer, nextPtr, prevPtr>& rhs) {
    lhs.swap(rhs);
}

}

#endif

