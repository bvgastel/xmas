#ifndef SYMBOLICINTERVALFIELD_H
#define SYMBOLICINTERVALFIELD_H

#include <iostream>
#include <vector>
#include <algorithm>
#include "symbolic.h"

class SymbolicIntervalField : public SymbolicPacketField
{
public:
    friend bool operator<(const SymbolicIntervalField &lhs, const SymbolicIntervalField &rhs);
    friend std::ostream &operator <<(std::ostream &out, const SymbolicIntervalField &c);
    typedef long long interval_type;
    //typedef __int128 interval_type;
protected:
    interval_type min;
    interval_type max; // not including this value,
public:
    SymbolicIntervalField(const SymbolicIntervalField &copy) : SymbolicPacketField(FastType<SymbolicIntervalField>::value), min(copy.min), max(copy.max) {
        //std::cout << "copy of SymbolicIntervalField" << std::endl;
    }
    SymbolicIntervalField(SymbolicIntervalField &&copy) : SymbolicPacketField(FastType<SymbolicIntervalField>::value), min(copy.min), max(copy.max) {
        //std::cout << "move of SymbolicIntervalField" << std::endl;
    }
    SymbolicIntervalField() : SymbolicPacketField(FastType<SymbolicIntervalField>::value), min(0), max(0) {
    }
    SymbolicIntervalField(interval_type min, interval_type max) : SymbolicPacketField(FastType<SymbolicIntervalField>::value), min(min), max(max) {
    }

    std::vector<std::shared_ptr<SymbolicPacketField>> getIntersection(const std::shared_ptr<SymbolicPacketField> & a) const;
    void getDifference(const std::shared_ptr<SymbolicPacketField> & a, std::function<void (std::shared_ptr<SymbolicPacketField>&&)> &&f) const;
    bool operator==(const SymbolicIntervalField &b) const {
        std::cout << *this << " == " << b << "?" << std::endl;
        return min == b.min && max == b.max;
    }
    bool operator==(const SymbolicPacketField &sb) const {
        //const SymbolicIntervalField *b = dynamic_cast<const SymbolicIntervalField*>(&sb);
        const SymbolicIntervalField *b = FastType<SymbolicIntervalField>::cast(&sb);
        //SymbolicIntervalField *b = (SymbolicIntervalField*)sb.get();
        //std::cout << *this << " == " << *b << "? (ptr version)" << std::endl;
        return b && min == b->min && max == b->max;
    }
    bool valid() const {
        return max > min;
    }
    bool empty() const {
        return min == max;
    }
    bool hasValue(interval_type value) const;
    bool contains(const std::shared_ptr<SymbolicPacketField> &f) const;
    bool isCombinable(const std::shared_ptr<SymbolicPacketField> &sa) const {
        //SymbolicIntervalField *a = sa.get() != nullptr ? dynamic_cast<SymbolicIntervalField*>(sa.get()) : nullptr;
        //SymbolicIntervalField *a = (SymbolicIntervalField*)sa.get();
        SymbolicIntervalField *a = FastType<SymbolicIntervalField>::cast(sa.get());
        //std::cout << "check field combining " << *(SymbolicPacketField*)this << " with " << *sa << std::endl;
        return a && ((min <= a->min && a->min <= max) || (min <= a->max && a->max <= max));
    }
    std::shared_ptr<SymbolicPacketField> combine(const std::shared_ptr<SymbolicPacketField> &f) const;
    SymbolicIntervalField& operator=(SymbolicIntervalField&& other) {
        if (&other != this) {
            min = other.min;
            max = other.max;
        }
        return *this;
    }
    SymbolicIntervalField& operator=(const SymbolicIntervalField& other) {
        if (&other != this) {
            min = other.min;
            max = other.max;
        }
        return *this;
    }
    virtual void print(std::ostream &out) const;
    virtual int hash() const {
        return min ^ ((max-min) << 16);
    }
    virtual void updateHash();
    void lessAs(interval_type b) {
        max = b;
    }

    void lessEqualAs(interval_type b) {
        max = b+1;
    }

    void greaterAs(interval_type b) {
        min = b+1;
    }
    void greaterEqualAs(interval_type b) {
        min = b;
    }
    void add(interval_type min, interval_type max) {
        if (this->min > std::numeric_limits<interval_type>::min())
            this->min += min;
        this->max += max-1;
        if (this->max < std::numeric_limits<interval_type>::max())
            this->max = (std::numeric_limits<interval_type>::max() - this->max) < max ? std::numeric_limits<interval_type>::max() : this->max + max - 1;
    }
    void add(SymbolicIntervalField &field) {
        add(field.min, field.max);
    }
    void minus(interval_type min, interval_type max) {
        if (this->min > std::numeric_limits<interval_type>::min())
            this->min = (this->min - std::numeric_limits<interval_type>::min()) < max ? std::numeric_limits<interval_type>::min() : this->min - max + 1;
        if (this->max < std::numeric_limits<interval_type>::max())
            this->max -= min;
    }
    void minus(SymbolicIntervalField &field) {
        minus(field.min, field.max);
    }

    std::vector<std::shared_ptr<SymbolicPacketField>> negate() const;
};

std::ostream &operator <<(std::ostream &out, const SymbolicIntervalField &c);

inline bool operator<(const SymbolicIntervalField &lhs, const SymbolicIntervalField &rhs) {
    return std::tie(lhs.min, lhs.max) < std::tie(rhs.min, rhs.max);
}

template <typename... Args>
std::shared_ptr<SymbolicPacketField> INTERVAL(Args &&... args) {
    return std::shared_ptr<SymbolicPacketField>(new SymbolicIntervalField({args...}));
}

template <typename... Args>
std::pair<SymbolicPacket::key_type, std::shared_ptr<SymbolicPacketField>> NAMED_INTERVAL(const SymbolicPacket::key_type &key, Args &&... args) {
    return std::make_pair(key, std::shared_ptr<SymbolicPacketField>(new SymbolicIntervalField({args...})));
}

inline std::shared_ptr<SymbolicPacketField> MAKE_INTERVAL(SymbolicIntervalField && a) {
    return std::shared_ptr<SymbolicPacketField>(new SymbolicIntervalField(std::move(a)));
}


#endif // SYMBOLICINTERVALFIELD_H
