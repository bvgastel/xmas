#ifndef Constraint_H
#define Constraint_H

#include <string>
#include <vector>
#include <algorithm>
#include <set>
#include <functional>
#include "xmas.h"
#include <iostream>

class ConstraintPacketField {
    friend bool operator<(const ConstraintPacketField &lhs, const ConstraintPacketField &rhs);
    friend std::ostream &operator <<(std::ostream &out, const ConstraintPacketField &c);
    std::string constraint;
public:
    ConstraintPacketField(const ConstraintPacketField &copy) : constraint(copy.constraint) {
        //std::cout << "copy of ConstraintPacketField" << std::endl;
    }
    ConstraintPacketField(ConstraintPacketField &&copy) : constraint(std::move(copy.constraint)) {
        //std::cout << "move of ConstraintPacketField" << std::endl;
    }
    ConstraintPacketField() : constraint() {
    }
    ConstraintPacketField(const std::string &constraint) : constraint(constraint) {
    }

    ConstraintPacketField getIntersection(const ConstraintPacketField & a) const;
    ConstraintPacketField getDifference(const ConstraintPacketField & a) const;
    bool operator==(const ConstraintPacketField &b) const {
        return constraint == b.constraint;
    }
    bool valid() const {
        return constraint != "false";
    }
    bool empty() const {
        return false;
    }
    bool contains(const ConstraintPacketField &f) const;
    bool isCombinable(const ConstraintPacketField &f) const {
        return true; //false; //complement == f.complement;
    }
    ConstraintPacketField combine(const ConstraintPacketField &f) const;
    ConstraintPacketField& operator=(ConstraintPacketField&& other) {
        if (&other != this) {
            constraint = std::move(other.constraint);
        }
        return *this;
    }
    ConstraintPacketField& operator=(const ConstraintPacketField& other) {
        if (&other != this) {
            constraint = other.constraint;
        }
        return *this;
    }
};

std::ostream &operator <<(std::ostream &out, const ConstraintPacketField &c);

inline bool operator<(const ConstraintPacketField &lhs, const ConstraintPacketField &rhs) {
    return lhs.constraint < rhs.constraint;
}

class ConstraintPacket {
    friend bool operator<(const ConstraintPacket &lhs, const ConstraintPacket &rhs);
public:
    std::vector<ConstraintPacketField> fields;
    bool propagated;
    ConstraintPacket(const std::vector<ConstraintPacketField> &fields) : fields(fields), propagated(false) {
    }
    ConstraintPacket(std::vector<ConstraintPacketField> &&fields) : fields(std::move(fields)), propagated(false) {
    }
    ConstraintPacket(std::initializer_list<ConstraintPacketField> &&fields) : fields(std::move(fields)), propagated(false) {
    }
    ConstraintPacket() : fields(), propagated(false) {
    }
    ConstraintPacket(const ConstraintPacket &c) : fields(c.fields), propagated(false) {
    }
    ConstraintPacket(ConstraintPacket &&copy) : fields(std::move(copy.fields)), propagated(false) {
        std::cout << "move of ConstraintPacket" << std::endl;
    }
    ConstraintPacket getIntersection(const ConstraintPacket & a) const;
    std::vector<ConstraintPacket> getDifference(const ConstraintPacket & a) const;
    bool operator==(const ConstraintPacket &p) const;
    bool valid() const {
        return std::all_of(fields.begin(), fields.end(), std::mem_fun_ref(&ConstraintPacketField::valid));
    }
    bool contains(const ConstraintPacket &p) const;
    bool isCombinable(const ConstraintPacket &p) const;
    ConstraintPacket combine(const ConstraintPacket &p) const;
    ConstraintPacket& operator=(ConstraintPacket&& other) {
        if (&other != this) {
            fields = std::move(other.fields);
            propagated = other.propagated;
        }
        return *this;
    }
    ConstraintPacket& operator=(const ConstraintPacket& other) {
        if (&other != this) {
            fields = other.fields;
            propagated = other.propagated;
        }
        return *this;
    }
};

std::ostream &operator <<(std::ostream &out, const ConstraintPacket &c);

inline bool operator<(const ConstraintPacket &lhs, const ConstraintPacket &rhs) {
    return lhs.fields < rhs.fields;
}


class ConstraintSwitchingFunctionExtension : public XMASComponentExtension {
public:
    std::set<ConstraintPacket> availablePackets;
    ConstraintSwitchingFunctionExtension() : availablePackets() {
    }
    void addConstraintPacket(const ConstraintPacket &p);
};

template <class T>
class ConstraintFunctionExtension : public XMASComponentExtension {
public:
    std::function<T> function;
    ConstraintFunctionExtension() : function() {
    }
    ConstraintFunctionExtension(std::function<T> &function) : function(function) {
    }
};

class ConstraintTypesExtension : public PortExtension {
public:
    std::vector<ConstraintPacket> availablePackets;
    ConstraintTypesExtension() : PortExtension(), availablePackets() {
    }
    // returns true if packet is not yet in here
    ConstraintPacket* addConstraintPacket(const ConstraintPacket &&p);
    void simplify(const std::string &desc);
};

template <class T>
T* getComponentInfo(XMASComponent *c, bool create = true) {
    T *ext = c->getExtension<T>();
    if (ext == nullptr && create) {
        //std::cout << "allocating new XMASComponent extension" << std::endl;
        ext = new T();
        c->addExtension(ext);
    }
    return ext;
}

template <class T>
T* getPortInfo(Port *port, bool create = true) {
    T *ext = port->getPortExtension<T>(create);
    return ext;
}

void attach(Input *port, ConstraintPacket &&a, std::function<void(Input *, ConstraintPacket &)> *callback = nullptr);
void attach(Output *port, ConstraintPacket &&a, std::function<void(Input *, ConstraintPacket &)> *callback = nullptr);

void attachPackets(Input *port, std::vector<ConstraintPacket> &&a, std::function<void(Input *, ConstraintPacket &)> *callback = nullptr);
void attachPackets(Output *port, std::vector<ConstraintPacket> &&a, std::function<void(Input *, ConstraintPacket &)> *callback = nullptr);

void attachPackets(const std::vector<Input *> &ports, const std::vector<ConstraintPacket> &p, std::function<void(Input *, ConstraintPacket &)> *callback = nullptr);
void attachPackets(const std::vector<Output *> &ports, const std::vector<ConstraintPacket> &p, std::function<void(Input *, ConstraintPacket &)> *callback = nullptr);
void attachSwitchingFunction(XMASSwitch *c, ConstraintPacket &&a);

inline void attach(Input *port, const ConstraintPacket &a, std::function<void(Input *, ConstraintPacket &)> *callback = nullptr) {
    attach(port, ConstraintPacket(a), callback);
}
inline void attach(Output *port, const ConstraintPacket &a, std::function<void(Input *, ConstraintPacket &)> *callback = nullptr) {
    attach(port, ConstraintPacket(a), callback);
}

template <class T>
void attachConstraintFunctionPostCondition(XMASFunction *c, T &&f)
{
    ConstraintFunctionExtension<ConstraintPacket (const ConstraintPacket&)> *ext = getComponentInfo<ConstraintFunctionExtension<ConstraintPacket (const ConstraintPacket&)>>(c);
    ext->function = f;
}

template <class T>
void attachConstraintFunction(XMASJoin *c, T &&f)
{
    ConstraintFunctionExtension<ConstraintPacket (const ConstraintPacket&, const ConstraintPacket&)> *ext = getComponentInfo<ConstraintFunctionExtension<ConstraintPacket (const ConstraintPacket&, const ConstraintPacket&)>>(c);
    ext->function = f;
}

void printConstraintTypeInfo(std::ostream &out, Port *input);

void ConstraintTypes(std::set<XMASComponent *> allComponents);

#endif // Constraint_H
