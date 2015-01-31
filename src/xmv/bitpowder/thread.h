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

#ifndef _THREAD_H_
#define _THREAD_H_

#include <pthread.h>
#include "common.h"
#include "stack.h"

#include "spinlock.h"

namespace bitpowder {
namespace lib {

class Thread {
public:
    template <class T>
    inline static void start(T * object) {
        start<T, &T::run>(object);
    }

    template <class T, void (T::*func)()>
    inline static void start(T * object) {
        start((void (*)(T *))&invoke<T,func>, object);
    }

    template <void (*func)()>
    inline static void start() {
        start((void (*)(void *))func, (void*)nullptr);
    }

    template <class T, void (*func)(T *)>
    inline static void start(T *object) {
        start(func, object);
    }
    inline static void start(void (*func)()) {
        start((void (*)(void *))func, (void*)nullptr);
    }
    template <class T>
    inline static void start(void (*func)(T *), T *object) {
        start((void (*)(void *))func, (void*)object);
    }
    static void start(void (*func)(void *), void *object);
};

}
}

#endif

