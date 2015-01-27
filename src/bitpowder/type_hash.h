/**
Copyright 2010-2014 Bernard van Gastel, bvgastel@bitpowder.com.
All rights reserved. This file is part of Bit Powder Libraries.

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
