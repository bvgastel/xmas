/*
 *  thread.cpp
 *  SpeedHTTPd
 *
 *  Created by Bernard van Gastel on 02-05-10.
 *  Copyright 2010 Bit Powder. All rights reserved.
 *
 */

#include "thread.h"
#include <thread>

namespace bitpowder {
namespace lib {


void Thread::start(void (*func)(void *), void *object) {
    std::thread t(func, object);
    t.detach();
}

}
}
