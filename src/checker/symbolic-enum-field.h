#ifndef SYMBOLICENUMFIELD_H
#define SYMBOLICENUMFIELD_H

#include <iostream>
#include <vector>
#include <algorithm>
#include "symbolic-visitor.h"
#include "lib/simplestring.h"

class SymbolicEnumField : public SymbolicPacketField {
    friend bool operator<(const SymbolicEnumField &lhs, const SymbolicEnumField &rhs);
    friend std::ostream &operator <<(std::ostream &out, const SymbolicEnumField &c);
public:
    // FIXME: use more memory efficient and faster String instead of this container std::string
    typedef String Type;
    //typedef std::string Type;
    std::vector<Type> values;
    //std::set<Type> values;
    bool complement;
private:
    int _hash;
public:
    void updateHash();
    SymbolicEnumField(const SymbolicEnumField &copy) : SymbolicPacketField(bitpowder::lib::FastType<SymbolicEnumField>::value),
        values(copy.values), complement(copy.complement), _hash(copy._hash) {
        //std::cout << "copy of SymbolicEnumField" << std::endl;
    }
    SymbolicEnumField(SymbolicEnumField &&copy) : SymbolicPacketField(bitpowder::lib::FastType<SymbolicEnumField>::value), values(std::move(copy.values)), complement(copy.complement), _hash(copy._hash) {
        //std::cout << "move of SymbolicEnumField" << std::endl;
    }
    SymbolicEnumField() : SymbolicPacketField(bitpowder::lib::FastType<SymbolicEnumField>::value), values(), complement(false) {
    }
    SymbolicEnumField(const std::vector<Type> &values, bool complement = false) :
        SymbolicPacketField(bitpowder::lib::FastType<SymbolicEnumField>::value), values(values), complement(complement) {
        std::sort(this->values.begin(), this->values.end());
        updateHash();
    }
    /*
    SymbolicEnumField(const std::vector<Type> &values, bool complement = false) : SymbolicPacketField(FastType<SymbolicEnumField>::value), values(), complement(complement) {
        // needed for intersection and difference to be efficient
        for (auto& v : values)
            this->values.insert(v);
        //std::sort(this->values.begin(), this->values.end());
        updateHash();
    }
    */
    SymbolicEnumField(std::initializer_list<Type> &&values) : SymbolicEnumField(std::vector<Type>(std::move(values))) {
    }
    SymbolicEnumField(const Type &value) : SymbolicPacketField(bitpowder::lib::FastType<SymbolicEnumField>::value), values(), complement(false) {
        values.push_back(value);
        //values.insert(value);
        updateHash();
    }

    std::vector<std::shared_ptr<SymbolicPacketField>> getIntersection(const std::shared_ptr<SymbolicPacketField> & a) const;
    void getDifference(const std::shared_ptr<SymbolicPacketField> & a, std::function<void (std::shared_ptr<SymbolicPacketField>&&)> &&f) const;
    bool operator==(const SymbolicEnumField &b) const {
        return complement == b.complement && values == b.values;
    }
    bool operator==(const SymbolicPacketField &sb) const {
        const SymbolicEnumField *b = bitpowder::lib::FastType<SymbolicEnumField>::cast(&sb);
        return b && _hash == b->_hash && complement == b->complement && values == b->values;
    }
    bool valid() const {
        return complement || !values.empty();
    }
    bool empty() const {
        return !complement && values.empty();
    }
    bool hasValue(const Type &value) const;
    bool contains(const std::shared_ptr<SymbolicPacketField> &f) const;
    bool isCombinable(const std::shared_ptr<SymbolicPacketField> &sa) const {
        SymbolicEnumField *a = bitpowder::lib::FastType<SymbolicEnumField>::cast(sa.get());
        return a && complement == a->complement;
    }
    std::shared_ptr<SymbolicPacketField> combine(const std::shared_ptr<SymbolicPacketField> &f) const;
    SymbolicEnumField& operator=(SymbolicEnumField&& other) {
        if (&other != this) {
            values = std::move(other.values);
            complement = other.complement;
            _hash = other._hash;
        }
        return *this;
    }
    SymbolicEnumField& operator=(const SymbolicEnumField& other) {
        if (&other != this) {
            values = other.values;
            complement = other.complement;
            _hash = other._hash;
        }
        return *this;
    }
    virtual void print(std::ostream &out) const;
    virtual int hash() const {
        return _hash;
    }
    std::vector<std::shared_ptr<SymbolicPacketField>> negate() const;

    void accept(const String& field, SymbolicFieldVisitor &v) {
        v.visit(field, this);
    }
};

inline bool operator<(const SymbolicEnumField &lhs, const SymbolicEnumField &rhs) {
    return lhs.values < rhs.values;
}

template <typename... Args>
std::shared_ptr<SymbolicPacketField> ENUM(Args &&... args) {
    //return std::make_shared<SymbolicPacketField>(SymbolicEnumField({args...}));
    return std::shared_ptr<SymbolicPacketField>(new SymbolicEnumField({args...}));
}

template <typename... Args>
std::pair<SymbolicPacket::key_type, std::shared_ptr<SymbolicPacketField>> NAMED_ENUM(const SymbolicPacket::key_type &key, Args &&... args) {
    return std::make_pair(key, std::shared_ptr<SymbolicPacketField>(new SymbolicEnumField({args...})));
}

inline std::shared_ptr<SymbolicPacketField> MAKE_ENUM(SymbolicEnumField && a) {
    return std::shared_ptr<SymbolicPacketField>(new SymbolicEnumField(std::move(a)));
}

inline std::pair<SymbolicPacket::key_type, std::shared_ptr<SymbolicPacketField>> MAKE_NAMED_ENUM(const SymbolicPacket::key_type &key, SymbolicEnumField && a) {
    return std::make_pair(key, std::shared_ptr<SymbolicPacketField>(new SymbolicEnumField(std::move(a))));
}


#endif // SYMBOLICENUMFIELD_H
