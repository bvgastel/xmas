/*
 *  thread.h
 *  SpeedHTTPd
 *
 *  Created by Bernard van Gastel on 02-05-10.
 *  Copyright 2010 Bit Powder. All rights reserved.
 *
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

