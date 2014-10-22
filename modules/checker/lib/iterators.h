
/*
 *  stack.h
 *  SpeedHTTPd
 *
 *  Created by Bernard van Gastel on 08-05-10.
 *  Copyright 2010 Bit Powder. All rights reserved.
 *
 */
#ifndef ITERATORS_H
#define ITERATORS_H

#include "common.h"
#include "exception.h"
#include "assert.h"

namespace bitpowder {
namespace lib {

template <class T, class P, class iterator>
    class select_iterator {
        iterator it;
        P func;
        void progress() {
            while (it && !func(*it)) {
                ++it;
            }
        }
    public:
        select_iterator(const iterator & it, const P &func) : it(it), func(func) {
            progress();
        }
        select_iterator(const iterator & it, P &&func) : it(it), func(std::move(func)) {
            progress();
        }
        ~select_iterator() {
        }
        inline void select(P &func) {
            this->func = func;
        }
        inline T operator*() {
            return static_cast<T>(*it);
        }
        inline T operator->() {
            return static_cast<T>(*it);
        }
        inline void operator++() {
            ++it;
            progress();
        }
        inline bool operator!=(const select_iterator& b) {
            return it != b.it;
        }
        inline bool operator==(const select_iterator& b) {
            return it == b.it;
        }
        inline operator bool() {
            return it;
        }
        inline void insert(const T& a) {
            it.insert(a);
        }
        inline T erase() {
            T retval = it.erase();
            this->progress();
            return retval;
        }
    };

    template <class T, class S, class P>
    struct select_container {
        S &value;
        P func;
        select_container(S &value, P &&func) : value(value), func(std::move(func)) {
        }
        select_iterator<T, P, decltype(value.begin())> begin() {
            return {value.begin(), func};
        }
        select_iterator<T, P, decltype(value.end())> end() {
            return {value.end(), func};
        }
    };

}
}

#endif // ITERATORS_H
