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
along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _COMMON_H_
#define _COMMON_H_

#define BP_MAX(a, b) ((a) > (b) ? (a) : (b))
#define BP_MIN(a, b) ((a) < (b) ? (a) : (b))

#ifndef MAX
#define MAX BP_MAX
#endif
#ifndef MIN
#define MIN BP_MIN
#endif

extern "C" {
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <strings.h>
}
#include <memory>
#include <functional>
#include <utility>
#include <type_traits>
#include <algorithm>

namespace bitpowder {
namespace lib {

#if (__cplusplus <= 199711L) && !defined(__GXX_EXPERIMENTAL_CXX0X__)
#define nullptr 0
#endif

#undef offsetof
//#define offsetof offset_of
#define offsetof(type, attribute) ((size_t)&(((type*)0x10)->attribute) - 0x10)

//#define EXTENDED_DEBUG
#ifdef EXTENDED_DEBUG
//#define eprint(msg) ::fprintf(stderr, "[%s:%i] %s\n", __FILE__, __LINE__, msg)
#define eprintf(format, ...) ::fprintf(stderr, "[%s:%i] " format "\n", __FILE__, __LINE__, ## __VA_ARGS__)
#define CRASH {*((int*)0x02) = 1;}
#else
//#define eprint(...)
#define eprintf(...)
#define CRASH
#endif

//#if (__cplusplus <= 199711L) && !defined(__GXX_EXPERIMENTAL_CXX0X__)
#define override
//#endif

// invoke is alternative to mem_fun, because this is statically, so it interfaces better with C (e.g. thread.cpp / pthread_create).
// For the same reason uses less memory (8 bytes instead of 16 bytes of a std::mem_fn)
template <class T, void (T::*method)()>
inline void invoke() {
    T::method();
}

template <class T, void (T::*method)()>
inline void invoke(T *object) {
    (object->*method)();
}

template <class T, class A, void (T::*method)(A)>
inline void invoke(T *object, A arg) {
    (object->*method)(arg);
}

template <class T, class A, class B, void (T::*method)(A, B)>
inline void invoke(T *object, A arg, B arg2) {
    (object->*method)(arg, arg2);
}

template <class T, class A, class B, class C, void (T::*method)(A, B, C)>
inline void invoke(T object, A arg, B arg2, C arg3) {
    (object->*method)(arg, arg2, arg3);
}

template <class T>
inline void destroy(T *object) {
    object->~T();
}

template <class T>
inline void performDelete(T *object) {
    delete object;
}

template <class T>
inline void nothing(T) {
}

template <class T, class A>
inline void nothing(T, A) {
}

template <class T, class A, class B>
inline void nothing(T, A, B) {
}

template <class T, class A, class B, class C>
inline void nothing(T, A, B, C) {
}

template <class T, class A>
inline void deleteSecond(T, A *snd) {
    delete snd;
}

#define NO_COPY(Class) \
    Class(const Class&) = delete; \
    Class& operator=(const Class&) = delete

// usage for in container classes
// usage: Test *bla = _new(mp->allocator<Test>(), Test(42));
template <class Alloc, class T> inline T *_new(Alloc a, const T &val = T()) {
    T *obj = a.allocate(1);
    a.construct(obj, val);
    return obj;
}

template <class T> inline T *_new(const T& val = T()) {
    std::allocator<T> a;
    T *obj = a.allocate(1);
    a.construct(obj, val);
    return obj;
}

template <class T>
class LinkedList {
    T content;
public:
    LinkedList<T> *next;

    LinkedList() : content(), next(NULL) {
    }
    LinkedList(const T &c) : content(c), next(NULL) {
    }
    /*
    LinkedList(const LinkedList<T> &c) : content(c.content), next(nullptr) {
        //next = c.next ? new LinkedList<T>(*c.next) : nullptr;
    }
    */
    LinkedList(const LinkedList<T> &c) {
        content = c ? c->content : T();
        next = nullptr;
        //next = c && c->next ? new LinkedList<T>(c->next) : nullptr;
    }
    //LinkedList(T c) : next(NULL), content(c) {};
    virtual ~LinkedList() {
        /*
        // can be used to minimise stack usage
        LinkedList<T> *current = next;
        while (current) {
            LinkedList<T> *next = current->next;
            current->next = NULL;
            delete current;
            current = next;
        }
        */
    }
    inline T *operator->() {
        return &content;
    }
    inline T &operator*() {
        return content;
    }
    inline T *get() {
        return &content;
    }
    template <void (*f)(T*)>
    inline void execute() {
        f(&content);
    }
    template <void (T::*f)()>
    inline void execute() {
        ((&content)->*f)();
    }

    template <void (*f)(T*)>
    inline void forEach() {
        LinkedList<T> *e = this;
        while (e) {
            f(e->get());
            e = e->next;
        }
    }
    template <void (T::*f)()>
    void forEach() {
        forEach< &invoke<T,f> >();
    }

    template <void (*f)(T*)>
    static void forEachIn(T *e) {
        while (e) {
            f(e);
            e = e->next;
        }
    }
    template <void (T::*f)()>
    static void forEachIn(T *e) {
        forEachIn<&invoke<T,f> >(e);
    }
    static void forEachIn(T *e, void (*f)(T*)) {
        while (e) {
            f(e);
            e = e->next;
        }
    }
};

struct evaluateHelper {
    evaluateHelper(std::initializer_list<int>) {
    }
};
#define evaluate(x) evaluateHelper{(x,1)...}

// with help from http://stackoverflow.com/questions/7858817/unpacking-a-tuple-to-call-a-matching-function-pointer
template <int...>
struct CountDown { };
template <int current, int... partialCountDown>
struct GenerateCountDown : GenerateCountDown<current-1, current-1, partialCountDown...> {
};
template <int... partialCountDown>
struct GenerateCountDown<0, partialCountDown...> {
    typedef CountDown<partialCountDown...> countDown;
};

template <typename Func, typename... Args, int... countdown>
void callFunctionHelper(const Func &f, const std::tuple<Args...> &tuple, CountDown<countdown...>) {
    f(std::get<countdown>(tuple)...);
}
template <typename Func, typename... Args>
void callFunction(const Func &f, const std::tuple<Args...> &args) {
    callFunctionHelper(f, args, typename GenerateCountDown<sizeof...(Args)>::countDown());
}

template <typename Func, typename Retval, typename... Args, int... countdown>
Retval evaluateFunctionHelper(const Func &f, const std::tuple<Args...> &tuple, CountDown<countdown...>) {
    return f(std::get<countdown>(tuple)...);
}
template <typename Func, typename Retval, typename... Args>
Retval evaluteFunction(const Func &f, const std::tuple<Args...> &args) {
    return evaluateFunctionHelper<Func, Retval, Args...>(f, args, typename GenerateCountDown<sizeof...(Args)>::countDown());
}

/**
  Usage:
        template <typename In>
        static typename In::type getArg() {
            return Type<typename In::type>::from(L, In::index);
        }
        template <typename... Args>
        static std::tuple<In...> convert(TypeWrapper<Args...> &&type) {
            return std::tuple<In...>(getArg<Args>()...);
        }

        convert(typename Zip<In>::type());
*/

template <int current, typename T>
struct ZippedNumber {
    typedef T type;
    static const int index = current;
};
template <typename... Args>
struct TypeWrapper {
};

template <typename T, typename... Args>
struct Unpack {
};
template <typename T, typename Tail, typename... Args>
struct Unpack<TypeWrapper<T,Tail>, Args...> : Unpack<Tail, T, Args...> {
};
template <typename... Args>
struct Unpack<TypeWrapper<>, Args...> {
    typedef TypeWrapper<Args...> type;
};

template <int current, typename Acc, typename... Args>
struct ZipNumbers {
};
template <int current, typename Acc, typename First, typename... Args>
struct ZipNumbers<current, Acc, First, Args...> : ZipNumbers<current+1, TypeWrapper<ZippedNumber<current, First>, Acc>, Args...> {
};
template <int current, typename Acc, typename First>
struct ZipNumbers<current, Acc, First> {
    //typedef Reverse<Acc> type;
    typedef typename Unpack<TypeWrapper<ZippedNumber<current, First>, Acc>>::type type;
};

template <typename... Args>
struct Zip : ZipNumbers<0, TypeWrapper<>, Args...> {
};

template <>
struct Zip<> {
    typedef typename Unpack<TypeWrapper<>>::type type;
};




///// select only one value of the arguments and throw away the others
template <size_t N, class Arg, class... Args>
struct nth_wrap {
    static auto get(Arg a, Args... args) -> decltype(nth_wrap<N-1, Args...>::get(std::forward<Args>(args)...)) {
        return nth_wrap<N-1, Args...>::get(std::forward<Args>(args)...);
    }
};

template <class Arg, class... Args>
struct nth_wrap<0, Arg, Args...> {
    static Arg get(Arg a, Args... args) {
        return std::forward<Arg>(a);
    }
};

template <class Arg>
struct nth_wrap<0, Arg> {
    static Arg get(Arg a) {
        return std::forward<Arg>(a);
    }
};

template <size_t N, class... Args>
auto nth(Args... a) -> decltype(nth_wrap<N,Args...>::get(std::forward<Args>(a)...)) {
    return nth_wrap<N,Args...>::get(std::forward<Args>(a)...);
}

}
}

#endif

