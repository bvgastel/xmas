/*
 *  queue.h
 *  SpeedHTTPd
 *
 *  Created by Bernard van Gastel on 08-05-10.
 *  Copyright 2010 Bit Powder. All rights reserved.
 *
 */

#ifndef _QUEUE_H_
#define _QUEUE_H_

#include "common.h"
#include "exception.h"
#include <memory>
#include <iterator>
#include <type_traits>
#include "iterators.h"

namespace bitpowder {
namespace lib {

template <class T, class PtrContainer, T PtrContainer::*ptr> class Stack;

template <class T, class PtrContainer = typename std::remove_pointer<T>::type, T PtrContainer::*ptr = &PtrContainer::next >
class Queue {
public:
    typedef typename std::remove_pointer<T>::type X;
    friend class Stack<T, PtrContainer, ptr>;
    typedef Queue Container;
private:
    T head;
    T tail;

    Queue(const Queue &q);
    Queue &operator=(const Queue &q);
public:
    inline Queue(T head = nullptr, T tail = nullptr) : head(head), tail(tail) {
        if (!tail)
            tail = head;
        if (tail)
            while (tail->*ptr)
                tail = (T)tail->*ptr;
    }
    inline ~Queue() {
        clear();
    }
    inline Queue(Queue &&q) : head(q.head), tail(q.tail) {
        q.head = q.tail = nullptr;
    }
    inline Queue& operator=(Queue &&q) {
        // as this is
        //std::swap(head, q.head);
        //std::swap(tail, q.tail);
        head = q.head;
        tail = q.tail;
        q.head = q.tail = nullptr;
        return *this;
    }
    inline void clear() {
        head = nullptr;
        tail = nullptr;
    }
    inline size_t count() {
        size_t cnt = 0;
        T *current = head;
        while (current) {
            current = current->*ptr;
            cnt++;
        }
        return cnt;
    }
    inline bool splice(T first, T last) {
        if (head == first || !first || !last)
            return false;
        (empty() ? head : tail->*ptr) = first;
        tail = last;
        return true;
    }
    inline bool splice(Queue &c) {
        if (&c == this || c.empty())
            return false;
        splice(c.front(), c.back());
        c.head = c.tail = nullptr;
        return true;
    }
    inline void push_back(T e) {
        push(e);
    }
    inline void push_front(T e) {
        unpop(e);
    }
    inline void push(T e) {
        if (!e)
            return;
        e->*ptr = nullptr;
        if (empty()) {
            head = e;
        } else {
            tail->*ptr = e;
        }
        tail = e;
    }

    inline T pop() {
        T e = nullptr;
        std::swap(e, head);
        if (e) {
            std::swap(head, (T)e->*ptr);
            if (!head)
                tail = nullptr;
        }
        return e;
    }
    inline T pop_front() {
        return pop();
    }

    inline Queue pop_all() {
        Queue retval;
        retval.splice(*this);
        return retval;
    }

    inline void unpop(T e) {
        if (!e)
            return;
        e->*ptr = head;
        if (empty())
            tail = e;
        head = e;
    }

    inline void erase(T e) {
        if (!e || empty())
            return;
        if (head == e) {
            pop();
            return;
        }
        // it is not the first item, and there are items
        T current = head;
        while (current->*ptr && current->*ptr != e) {
            current = current->*ptr;
        }
        if (current->*ptr == e) {
            current->*ptr = e->*ptr;
            if (tail == e)
                tail = current;
        }
        e->*ptr = nullptr;
    }

    inline T front() {
        return head;
    }
    inline T back() {
        return tail;
    }
    inline const T front() const {
        return head;
    }
    inline const T back() const {
        return tail;
    }

    // information
    inline bool empty() const {
        return !(bool)head;
    }

    inline bool multipleItems() const {
        return !empty() && head != tail;
    }

    template <class Func>
    void insert(T object, Func &&f) {
        T *current = &head;
        while (*current) {
            if (f(*current)) {
                T next = (*current)->*ptr;
                (*current)->*ptr = object;
                object->*ptr = next;
                return;
            }
            current = &((*current)->*ptr);
        }
        push(object);
    }

    // iterators
    /*
    template <void (T::*func)()>
    inline void forEach() const {
        forEach< &invoke<T,func> >();
    }
    */
    template <void (*func)(T)>
    inline void forEach() const {
        T current = head;
        while (current) {
            T next = (T)current->*ptr;
            func(current);
            current = next;
        }
    }

    inline void forEach(void (*func)(T)) const {
        T current = head;
        while (current) {
            T next = (T)current->*ptr;
            func(current);
            current = next;
        }
    }

    template <class Self, class PtrType>
    class iterator_base {
    protected:
        PtrType* value;
    public:
        iterator_base(PtrType *value) : value(value) {
        }
        iterator_base(const iterator_base &a) : value(a.value) {
        }
        iterator_base(iterator_base &&a) : value(nullptr) {
            std::swap(a.value, value);
        }
        ~iterator_base() {
        }
        inline T operator*() {
            return *value;
        }
        inline T operator->() {
            return *value;
        }
        inline Self operator+(int i) {
            Self retval = *static_cast<Self*>(this);
            while (retval && i-- > 0)
                ++retval;
            return retval;
        }
        inline bool operator!=(const iterator_base& b) {
            return !(*this == b);
        }
        inline bool operator==(const iterator_base& b) {
            if (value == b.value)
                return true;
            X *thisPointsTo = value && *value ? &**value : nullptr;
            X *bPointsTo = b.value && *b.value ? &**b.value : nullptr;
            return thisPointsTo == bPointsTo;
        }
        inline operator bool() {
            return value && *value;
        }
    };
    class const_iterator : public iterator_base<const_iterator,const T> {
    public:
        const_iterator(const T *value) : iterator_base<const_iterator,const T>(value) {
        }
        const_iterator(const const_iterator &a) : iterator_base<const_iterator,const T>(a) {
        }
        const_iterator(const_iterator &&a) : iterator_base<const_iterator,const T>(std::move(a)) {
        }
        inline void operator++() {
            this->value = this->value && *this->value ? &((*this->value)->*ptr) : nullptr;
        }
    };

    class iterator : public iterator_base<iterator,T> {
        Queue &q;
        T prevItem; // needed for setting 'tail' to the correct value
    public:
        iterator(T *value, Queue &q, T prevItem = nullptr) : iterator_base<iterator,T>(value), q(q), prevItem(prevItem) {
        }
        iterator(const iterator &a) : iterator_base<iterator,T>(a), q(a.q), prevItem(a.prevItem) {
        }
        iterator(iterator &&a) : iterator_base<iterator,T>(std::move(a)), q(a.q), prevItem(nullptr) {
            std::swap(prevItem, a.prevItem);
        }
        inline void operator++() {
            prevItem = this->value && *this->value ? *this->value : nullptr;
            this->value = this->value && *this->value ? &((*this->value)->*ptr) : nullptr;
        }
        inline T erase() {
            if (!this->value || !*this->value)
                return nullptr;
            T retval = std::move(*this->value);
            *this->value = std::move(bool(retval) ? retval->*ptr : nullptr);
            if (*this->value == nullptr) { // alternative way of specifying q.tail == retval (but more defensive)
                q.tail = std::move(prevItem);
                prevItem = nullptr;
            }
            return retval;
        }
        inline void insert(const T & a) {
            if (!this->value || !*this->value)
                return;
            if (*this->value == nullptr) {
                q.tail = a;
            }
            a->*ptr = std::move(*this->value);
            *this->value = a;
        }
    };


    inline iterator begin() {
        return {&head, *this};
    }
    inline iterator end() {
        return {nullptr, *this};
    }

    inline const_iterator begin() const {
        return {&head};
    }
    inline const_iterator end() const {
        return {nullptr};
    }

    // suggested usage: std::ref(arg)
    template <class P>
    inline select_container<T, const Container, P> select(P&& func) const {
        return {*this, std::forward<P>(func)};
    }
    // suggested usage: std::ref(arg)
    template <class P>
    inline select_container<T, Container, P> select(P&& func) {
        return {*this, std::forward<P>(func)};
    }
    // suggested usage: std::ref(arg)
    template <class Ptr, class P>
    inline select_container<Ptr, const Container, P> selectTo(P&& func) const {
        return {*this, std::forward<P>(func)};
    }
    // suggested usage: std::ref(arg)
    template <class Ptr, class P>
    inline select_container<Ptr, Container, P> selectTo(P&& func) {
        return {*this, std::forward<P>(func)};
    }
    template <class P>
    inline auto sbegin(P &&func) const -> select_iterator<T,P,decltype(this->begin())> {
        return {begin(), std::forward<P>(func)};
    }
    template <class P>
    inline auto sbegin(P &&func) -> select_iterator<T,P,decltype(this->begin())> {
        return {begin(), std::forward<P>(func)};
    }
};

}
}

namespace std {

template <class T, class PtrContainer, T PtrContainer::*nextPtr>
void swap(bitpowder::lib::Queue<T, PtrContainer, nextPtr>& lhs, bitpowder::lib::Queue<T, PtrContainer, nextPtr>& rhs) {
    std::swap(lhs.head, rhs.head);
    std::swap(lhs.tail, rhs.tail);
}

}

#endif

