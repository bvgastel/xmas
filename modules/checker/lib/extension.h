#ifndef EXTENSION_H
#define EXTENSION_H

//#define EXTENSION_USING_CPP_TYPE_INDEX

#include "stack.h"
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
    E *next;
#ifdef EXTENSION_USING_CPP_TYPE_INDEX
    Extension() : type(typeid(E)), next(nullptr) {}
#else
    Extension() : type(FastTypeEmpty), next(nullptr) {}
#endif
    // one virtual method to make dynamic_cast work
    virtual ~Extension() {}
};

template <class E>
class ExtensionContainer {
    template <class T>
    class Match {
#ifdef EXTENSION_USING_CPP_TYPE_INDEX
        std::type_index targetType;
#endif
    public:
#ifdef EXTENSION_USING_CPP_TYPE_INDEX
        Match() : targetType(typeid(T)) {
#else
        Match() {
#endif
        }
        bool operator()(Extension<E> *e) {
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
        T *retval;
        bool operator()(E *e) {
            retval = dynamic_cast<T*>(e);
            return retval != nullptr;
        }
    };
protected:
    Stack<E*, Extension<E>> extensions;
public:
    template <class T>
    T *getExtension() const {
        Match<T> m;
        return static_cast<T*>(extensions.first(std::ref(m)));
    }

    template <class T>
    T *getExtensionOfBaseType() const {
        MatchDynamic<T> m;
        return extensions.first(std::ref(m)) ? m.retval : nullptr;
    }

    template <class T>
    auto selectExtensions() const -> decltype(extensions.template selectTo<T*>(MatchDynamic<T>())) {
        return extensions.template selectTo<T*>(MatchDynamic<T>());
    }

    template <class T>
    auto selectExtensionsOfBaseType() const -> decltype(extensions.template selectTo<T*>(MatchDynamic<T>())) {
        return extensions.template selectTo<T*>(MatchDynamic<T>());
    }

    template <class T>
    void addExtension(T *extension) {
#ifdef EXTENSION_USING_CPP_TYPE_INDEX
        extension->type = std::type_index(typeid(T));
#else
        extension->type = FastType<T>::value;
#endif
        extensions.push_back(extension);
    }

    template <class T>
    T *removeExtension() {
        Match<T> m;
        for (auto it = extensions.select(std::ref(m)).begin(); it != extensions.end(); )
            return static_cast<T*>(it.erase());
        return nullptr;
    }

    template <class T>
    T *removeExtensionOfBaseType() {
        MatchDynamic<T> m;
        for (auto it = extensions.select(std::ref(m)).begin(); it != extensions.end(); )
            return static_cast<T*>(it.erase());
        return nullptr;
    }

    Stack<E*, Extension<E>> clearExtensions() {
        return extensions.getAll();
    }
};

}
}

#endif // EXTENSION_H
