#ifndef SymbolicRangeField_H
#define SymbolicRangeField_H

#include <iostream>
#include <vector>
#include <algorithm>
#include "symbolic-visitor.h"

class SymbolicRangeField : public SymbolicPacketField {
    friend bool operator<(const SymbolicRangeField &lhs, const SymbolicRangeField &rhs);
    friend std::ostream &operator <<(std::ostream &out, const SymbolicRangeField &c);
    int min;
    int max; // not including this value
    bool complement;
public:
    SymbolicRangeField(const SymbolicRangeField &copy)
        : SymbolicPacketField(bitpowder::lib::FastType<SymbolicRangeField>::value),
          min(copy.min),
          max(copy.max),
          complement(copy.complement) {
        //std::cout << "copy of SymbolicRangeField" << std::endl;
    }
    SymbolicRangeField(SymbolicRangeField &&copy)
        : SymbolicPacketField(bitpowder::lib::FastType<SymbolicRangeField>::value),
          min(copy.min),
          max(copy.max),
          complement(copy.complement) {
        //std::cout << "move of SymbolicRangeField" << std::endl;
    }
    SymbolicRangeField()
        : SymbolicPacketField(bitpowder::lib::FastType<SymbolicRangeField>::value),
          min(0),
          max(0),
          complement(false) {
    }
    SymbolicRangeField(int min, int max, bool complement = false)
        : SymbolicPacketField(bitpowder::lib::FastType<SymbolicRangeField>::value),
          min(min),
          max(max),
          complement(complement) {
    }

    std::vector<std::shared_ptr<SymbolicPacketField>> getIntersection(const std::shared_ptr<SymbolicPacketField> & a) const;
    void getDifference(const std::shared_ptr<SymbolicPacketField> & a, std::function<void (std::shared_ptr<SymbolicPacketField>&&)> &&f) const;
    bool operator==(const SymbolicRangeField &b) const {
        return min == b.min && max == b.max && complement == b.complement;
    }
    bool operator==(const SymbolicPacketField &sb) const {
        const SymbolicRangeField *b = dynamic_cast<const SymbolicRangeField*>(&sb);
        //SymbolicRangeField *b = (SymbolicRangeField*)sb.get();
        return b && min == b->min && max == b->max && complement == b->complement;
    }
    bool valid() const {
        return max > min;
    }
    bool empty() const {
        return min == max;
    }
    bool hasValue(int value) const;
    bool contains(const std::shared_ptr<SymbolicPacketField> &f) const;
    bool isCombinable(const std::shared_ptr<SymbolicPacketField> &sa) const {
        //SymbolicRangeField *a = sa.get() != nullptr ? dynamic_cast<SymbolicRangeField*>(sa.get()) : nullptr;
        SymbolicRangeField *a = (SymbolicRangeField*)sa.get();
        //std::cout << "check field combining " << *(SymbolicPacketField*)this << " with " << *sa << std::endl;
        //return a && ((a->min <= min && min <= a->max) || (a->max >= max && max >= a->min)) && complement == a->complement;
        return a && ((min <= a->min && a->min <= max) || (min <= a->max && a->max <= max)) && complement == a->complement;
    }
    std::shared_ptr<SymbolicPacketField> combine(const std::shared_ptr<SymbolicPacketField> &f) const;
    SymbolicRangeField& operator=(SymbolicRangeField&& other) {
        if (&other != this) {
            min = other.min;
            max = other.max;
            complement = other.complement;
        }
        return *this;
    }
    SymbolicRangeField& operator=(const SymbolicRangeField& other) {
        if (&other != this) {
            min = other.min;
            max = other.max;
            complement = other.complement;
        }
        return *this;
    }
    virtual void print(std::ostream &out) const;
    virtual int hash() const {
        return min ^ ((max-min) << 16);
    }
    std::vector<std::shared_ptr<SymbolicPacketField>> negate() const;
    void updateHash();
    void accept(SymbolicFieldVisitor &v) {
        v.visit(this);
    }
};

inline bool operator<(const SymbolicRangeField &lhs, const SymbolicRangeField &rhs) {
    return std::tie(lhs.min, lhs.max) < std::tie(rhs.min, rhs.max);
}

template <typename... Args>
std::shared_ptr<SymbolicPacketField> RANGE(Args &&... args) {
    return std::shared_ptr<SymbolicPacketField>(new SymbolicRangeField({args...}));
}

inline std::shared_ptr<SymbolicPacketField> MAKE_RANGE(SymbolicRangeField && a) {
    return std::shared_ptr<SymbolicPacketField>(new SymbolicRangeField(std::move(a)));
}


#endif // SymbolicRangeField_H
