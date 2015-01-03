#ifndef TYPE_HASH_H
#define TYPE_HASH_H

#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <typeinfo>
#include <cstdlib>

/** Fast manner of testing types of certain classes, type should be stored in an integer.
 * Suggestions for classes with a virtual method: DECLARE_TYPE_ID for the type, so it is really quick
 * Typical use case: virtual base class with templated derived classes, and to check if a derived class is of a certain type.
 * If no virtual base class is used, one can still use this, as you can declare the hash of derived classes with DECLARE_TYPE_HASH
 *
 * WARNING: use DECLARE_TYPE_HASH only if all class definitions are in one file, otherwise wrong type information is used
 */

namespace bitpowder {
namespace lib {

typedef const ssize_t* FastTypeT;
#define FastTypeEmpty nullptr

template <class T>
struct FastType {
    const static ssize_t value_addr;
    constexpr static FastTypeT value = &value_addr;

    template <class A, FastTypeT A::*type = &A::type>
    static bool pointer_of_type(A *a) {
        //fprintf(stdout, "expected %s (%i), got %s (%i)\n", typeid(T).name(), hash_code, typeid(*a).name(), a->*type);
        return a != nullptr && a->*type == value; // && typeid(*a) == typeid(T);
    }

    template <class A, FastTypeT A::*type = &A::type>
    static T* cast(A *a) {
        return pointer_of_type(a) ? static_cast<T*>(a) : nullptr;
    }

    template <class A, FastTypeT A::*type = &A::type>
    static const T* cast(const A *a) {
        return pointer_of_type(a) ? static_cast<const T*>(a) : nullptr;
    }

};

template <class T> const ssize_t FastType<T>::value_addr = typeid(T).hash_code();

}
}

#endif // TYPE_HASH_H
