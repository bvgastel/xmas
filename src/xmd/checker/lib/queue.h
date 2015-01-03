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
    NO_COPY(Queue);
public:
    typedef T value_type;
    typedef typename std::remove_pointer<T>::type X;
    friend class Stack<T, PtrContainer, ptr>;
    typedef Queue Container;
private:
    T head = nullptr;
    T tail = nullptr;
public:
    inline Queue(std::initializer_list<T> l) {
        for (auto &e : l)
            push(e);
    }
    inline Queue() {
    }
    inline ~Queue() {
        clear();
    }
    inline Queue(Queue &&q) : head(std::move(q.head)), tail(std::move(q.tail)) {
        q.head = nullptr;
        q.tail = nullptr;
    }

    inline void clear() {
        head = nullptr;
        tail = nullptr;
    }

    inline void push(const T& e) {
        push_back(e);
    }

    inline T pop() {
        return pop_front();
    }

    inline void unpop(const T& e) {
        push_front(e);
    }

    inline void push_front(const T& e) {
        if (!e)
            return;
        e->*ptr = head;
        if (empty())
            tail = e;
        head = e;
    }

    inline void push_back(const T& e) {
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

    inline T pop_front() {
        T e = nullptr;
        std::swap(e, head);
        if (e) {
            std::swap(head, (T)e->*ptr);
            if (!head)
                tail = nullptr;
        }
        return e;
    }

    inline Queue pop_all() {
        return std::move(*this);
    }

    inline T front() {
        return head;
    }
    inline T back() {
        return tail;
    }
    inline T front() const {
        return head;
    }
    inline T back() const {
        return tail;
    }
    inline bool splice(const T& first, const T& last) {
        if (head == first || !first || !last)
            return false;
        (empty() ? head : tail->*ptr) = first;
        tail = last;
        return true;
    }
    inline bool splice(T&& first, T&& last) {
        if (head == first || !first || !last)
            return false;
        (empty() ? head : tail->*ptr) = std::move(first);
        tail = std::move(last);
        return true;
    }

    inline bool splice(Queue &c) {
        bool retval = splice(std::move(c.head), std::move(c.tail));
        c.head = nullptr;
        c.tail = nullptr;
        return retval;
    }
    template <class C>
    inline bool splice(C &c) {
        bool retval = splice(c.front(), c.back());
        if (retval)
            c.clear();
        return retval;
    }

    inline void erase(const T& e) {
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
        if (current->*ptr != nullptr) { // therefore we know current->*ptr == e, but this check is cheaper
            current->*ptr = std::move(e->*ptr);
            e->*ptr = nullptr;
            if (tail == e)
                tail = std::move(current);
        }
    }

    // information
    inline bool empty() const {
        return !(bool)head;
    }

    inline bool multipleItems() const {
        return !empty() && head != tail;
    }

/*
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
*/
    // iterators
    /*
    template <void (T::*func)()>
    inline void forEach() const {
        forEach< &invoke<T,func> >();
    }

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
    */
    template <class P>
    inline int erase_if(P &&func) {
        int retval = 0;
        for (auto it = select(std::ref(func)).begin(); it != end(); it.erase(), ++retval);
        return retval;
    }

    template <class P>
    inline T first(P &&func) const {
        for (auto e : select(std::ref(func)))
            return e;
        return nullptr;
    }

    template <class Self, class PtrType>
    class iterator_base : public std::iterator<std::bidirectional_iterator_tag, X, unsigned int, PtrType> {
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
            return value ? *value : nullptr;
        }
        inline T operator->() {
            return value ? *value : nullptr;
        }
        inline Self operator+(int i) {
            Self retval = *static_cast<Self*>(this);
            while (retval && i-- > 0)
                ++retval;
            return retval;
        }
        inline bool operator!=(const iterator_base& b) const {
            return !(*this == b);
        }
        inline bool operator==(const iterator_base& b) const {
            if (value == b.value)
                return true;
            X *thisPointsTo = value && *value ? &**value : nullptr;
            X *bPointsTo = b.value && *b.value ? &**b.value : nullptr;
            return thisPointsTo == bPointsTo;
        }
        inline operator bool() const {
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
        // insert before current item, but also progressing, so current item stays the same
        inline void insert(const T& a) {
            if (!this->value || !*this->value)
                return;
            if (*this->value == nullptr) {
                q.tail = a;
            }
            a->*ptr = std::move(*this->value);
            *this->value = a;
            operator++();
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
/*
template<class Type>
struct iterator_traits<typename bitpowder::lib::Queue<Type>::iterator> {
    typedef unsigned int difference_type;
    typedef typename bitpowder::lib::Queue<Type>::X value_type;
    typedef Type pointer;
    typedef typename bitpowder::lib::Queue<Type>::X& reference;
    typedef std::forward_iterator_tag iterator_category;
};
*/
}

#endif

