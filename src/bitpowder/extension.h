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

#ifndef EXTENSION_H
#define EXTENSION_H

#ifdef _MSC_VER         // type index causes less problems on VC (JK)
#   define EXTENSION_USING_CPP_TYPE_INDEX
#endif

#include "stack.h"
//#include "shared_object.h"

#ifdef EXTENSION_USING_CPP_TYPE_INDEX
#include <typeindex>
#else
#include "type_hash.h"
#endif

namespace bitpowder {
namespace lib {

template <class E>
class Extension {
public:
#ifdef EXTENSION_USING_CPP_TYPE_INDEX
    std::type_index type;
#else
    FastTypeT type;
#endif
    //lib::concurrent_count refcount;
    //shared_object<E, Extension<E>> next = nullptr;
    E* next = nullptr;
#ifdef EXTENSION_USING_CPP_TYPE_INDEX
    Extension() : type(typeid(E)) {}
#else
    Extension() : type(FastTypeEmpty) {}
#endif
    // one virtual method to make dynamic_cast work
    virtual ~Extension() {}
};

template <class E>
class ExtensionContainer {
public:
    //typedef shared_object<E, Extension<E>> Element;
    typedef E* Element;
    typedef Stack<Element, Extension<E>> ExtensionStack;
private:
    template <class T>
    class Match {
#ifdef EXTENSION_USING_CPP_TYPE_INDEX
        std::type_index targetType;
#endif
    public:
#ifdef EXTENSION_USING_CPP_TYPE_INDEX
        Match() : targetType(typeid(std::remove_pointer<T>::type)) {
#else
        Match() {
#endif
        }
        bool operator()(Extension<E>* e) {
#ifdef EXTENSION_USING_CPP_TYPE_INDEX
            return e->type == targetType;
#else
            return FastType<T>::pointer_of_type(e);
#endif
        }
    };
    template <class T>
    class MatchDynamic {
    public:
        T* retval;
        bool operator()(const Element& e) {
            retval = dynamic_cast<T*>(&*e); // extra &* is used to get rid of smart-containers like lib::shared_object
            return retval != nullptr;
        }
    };
protected:
    ExtensionStack extensions;
public:
    ExtensionStack& getAllExtensions() {
        return extensions;
    }

    template <class T>
    T getExtension() const {
        Match<typename std::remove_pointer<T>::type> m;
        return static_cast<T>(extensions.first(std::ref(m)));
    }

    template <class T>
    T* getExtensionOfBaseType() const {
        MatchDynamic<T> m;
        return extensions.first(std::ref(m)) ? m.retval : nullptr;
    }

    template <class T>
    auto selectExtensions() const -> decltype(extensions.template selectTo<T*>(Match<T>())) {
        return extensions.template selectTo<T*>(Match<T>());
    }

    template <class T>
    auto selectExtensionsOfBaseType() const -> decltype(extensions.template selectTo<T*>(MatchDynamic<T>())) {
        return extensions.template selectTo<T*>(MatchDynamic<T>());
    }

    template <class T>
    void addExtension(const T& extension) {
#ifdef EXTENSION_USING_CPP_TYPE_INDEX
        extension->type = std::type_index(typeid(typename std::remove_pointer<T>::type));
#else
        extension->type = FastType<typename std::remove_pointer<T>::type>::value;
#endif
        extensions.push_back(extension);
    }

    template <class T>
    T removeExtension() {
        Match<typename std::remove_pointer<T>::type> m;
        for (auto it = extensions.select(std::ref(m)).begin(); it != extensions.end(); )
            return static_cast<T>(it.erase());
        return nullptr;
    }

    template <class T>
    T* removeExtensionOfBaseType() {
        MatchDynamic<T> m;
        for (auto it = extensions.select(std::ref(m)).begin(); it != extensions.end(); )
            return static_cast<T*>(it.erase());
        return nullptr;
    }

    ExtensionStack clearExtensions() {
        return extensions.pop_all();
    }
};

}
}

#endif // EXTENSION_H
