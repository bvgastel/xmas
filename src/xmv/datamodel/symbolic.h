#ifndef SYMBOLIC_H
#define SYMBOLIC_H

#include <string>
#include <vector>
#include <algorithm>
#include <set>
#include <map>
#include <functional>
#include <iostream>
#include <unordered_set>
#include <memory>
#include <atomic>

#include "xmas.h"
#include "simplestring.h"
#include "type_hash.h"

#ifdef CONCURRENCY
#include "lock.h"
#include "spinlock.h"
#endif


class SymbolicFieldVisitor;

class SymbolicPacketField {
public:
    bitpowder::lib::FastTypeT type;
    SymbolicPacketField(bitpowder::lib::FastTypeT type) : type(type) {
    }
    virtual std::vector<std::shared_ptr<SymbolicPacketField>> getIntersection(const std::shared_ptr<SymbolicPacketField> & a) const = 0;
    virtual void getDifference(const std::shared_ptr<SymbolicPacketField> & a, std::function<void (std::shared_ptr<SymbolicPacketField> &&)> &&f) const = 0;
    virtual bool operator==(const SymbolicPacketField &b) const = 0;
    bool operator!=(const SymbolicPacketField &b) const {
        return !(*this == b);
    }
    virtual bool valid() const = 0;
    virtual bool empty() const = 0;
    virtual bool contains(const std::shared_ptr<SymbolicPacketField> &f) const = 0;
    virtual bool isCombinable(const std::shared_ptr<SymbolicPacketField> &f) const = 0;
    virtual std::shared_ptr<SymbolicPacketField> combine(const std::shared_ptr<SymbolicPacketField> &f) const = 0;
    virtual std::vector<std::shared_ptr<SymbolicPacketField>> negate() const = 0;
    virtual void print(std::ostream &out) const = 0;
    virtual int hash() const = 0;
    virtual void updateHash() = 0;

        // support for the visitor pattern
    virtual void accept(const bitpowder::lib::String& field, SymbolicFieldVisitor &t) = 0;
    virtual void printOldCSyntax(std::ostream& out,
                                 std::map<bitpowder::lib::String, int>& enumMap,
                                 const bitpowder::lib::String& key) const = 0;
};

std::ostream &operator <<(std::ostream &out, const SymbolicPacketField &c);
bool operator<(const SymbolicPacketField &lhs, const SymbolicPacketField &rhs);
bool operator==(const std::shared_ptr<SymbolicPacketField>& lhs, const std::shared_ptr<SymbolicPacketField>& rhs);

class SymbolicPacket {
    friend bool operator<(const SymbolicPacket &lhs, const SymbolicPacket &rhs);
    void setHash();
public:
    //typedef std::string key_type;
    typedef bitpowder::lib::String key_type;
    std::map<key_type,std::shared_ptr<SymbolicPacketField>> fields;
    bool propagated;
    int _hash;
    SymbolicPacket(const std::map<key_type,std::shared_ptr<SymbolicPacketField>> &fields) : fields(fields), propagated(false) {
        setHash();
    }
    SymbolicPacket(std::map<key_type,std::shared_ptr<SymbolicPacketField>> &&fields) : fields(std::move(fields)), propagated(false) {
        setHash();
    }
    SymbolicPacket(std::initializer_list<std::pair<const key_type,std::shared_ptr<SymbolicPacketField>>> &&fields) : fields(fields), propagated(false) {
        setHash();
    }
    SymbolicPacket() : fields(), propagated(false), _hash(0) {
    }
    SymbolicPacket(const SymbolicPacket &c) : fields(c.fields), propagated(false), _hash(c._hash) {
    }
    SymbolicPacket(SymbolicPacket &&copy) : fields(copy.fields), propagated(false), _hash(copy._hash) {
        //std::cout << "move of SymbolicPacket" << std::endl;
    }
    std::vector<SymbolicPacket> getIntersection(const SymbolicPacket & a) const;
    void getDifference(const SymbolicPacket & a, std::function<void(SymbolicPacket&&)> &&f) const;
    std::vector<SymbolicPacket> negate() const;
    bool operator==(const SymbolicPacket &p) const;
    bool valid() const {
        return std::all_of(fields.begin(), fields.end(), [](const decltype(fields)::value_type &it) {
            return it.second->valid();
        });
    }
    bool contains(const SymbolicPacket &p) const;
    bool isCombinable(const SymbolicPacket &p) const;
    SymbolicPacket combine(const SymbolicPacket &p) const;
    SymbolicPacket& operator=(SymbolicPacket&& other) {
        if (&other != this) {
            std::swap(fields, other.fields);
            std::swap(propagated, other.propagated);
            std::swap(_hash, other._hash);
        }
        return *this;
    }
    SymbolicPacket& operator=(const SymbolicPacket& other) {
        if (&other != this) {
            fields = other.fields;
            propagated = other.propagated;
            _hash = other._hash;
        }
        return *this;
    }
    int hash() const {
        return _hash;
    }
    void updateHash();
    void accept(SymbolicFieldVisitor &visitor) const;
    void printOldCSyntax(std::ostream& out, std::map<bitpowder::lib::String, int>& enumMap) const;
};


std::ostream &operator <<(std::ostream &out, const SymbolicPacket &c);
bool operator<(const SymbolicPacket &lhs, const SymbolicPacket &rhs) ;

class SymbolicSwitchingFunctionExtension : public XMASComponentExtension {
public:
    std::set<SymbolicPacket> availablePackets;
    SymbolicSwitchingFunctionExtension() : availablePackets() {
    }
    void addSymbolicPacket(const SymbolicPacket &p);
};

template <class T>
class SymbolicFunctionExtension : public XMASComponentExtension {
public:
    std::function<T> function;
    SymbolicFunctionExtension() : function() {
    }
    SymbolicFunctionExtension(std::function<T> &function) : function(function) {
    }
};

template <class T>
class SymbolicInverseFunctionExtension : public XMASComponentExtension {
public:
    std::function<T> function;
    SymbolicInverseFunctionExtension() : function() {
    }
    SymbolicInverseFunctionExtension(std::function<T> &function) : function(function) {
    }
};

class TypeWorker;
class TypeWorkerItem;

class SymbolicTypesExtension : public PortExtension {
public:
    std::vector<SymbolicPacket> availablePackets;
    std::unordered_set<int> availablePacketsHashes;

#ifdef CONCURRENCY
    SpinLock lock;
#endif
    std::vector<SymbolicPacket> candidates;

    TypeWorkerItem* workerItem;

    SymbolicTypesExtension() : PortExtension(), availablePackets(), availablePacketsHashes(), candidates(), workerItem(nullptr) {
    }
    // returns true if packet is not yet in here
    void addSymbolicPacket(SymbolicPacket &&p);
    void simplify(const std::string &desc, bool full = false);
};


void removeTypes(Port *port);

void attach(Input *port, SymbolicPacket &&a);
void attach(Output *port, SymbolicPacket &&a);

void attachPackets(Input *port, std::vector<SymbolicPacket> &&a);
void attachPackets(Output *port, std::vector<SymbolicPacket> &&a);

void attachPackets(Output *port, const std::vector<SymbolicPacket> &a);

void attachPackets(const std::vector<Input *> &ports, const std::vector<SymbolicPacket> &p);
void attachPackets(const std::vector<Output *> &ports, const std::vector<SymbolicPacket> &p);
void attachSwitchingFunction(XMASSwitch *c, SymbolicPacket &&a);
void attachSwitchingFunction(XMASSwitch *c, const SymbolicPacket &a);


inline void attach(Input *port, const SymbolicPacket &a) {
    attach(port, SymbolicPacket(a));
}
inline void attach(Output *port, const SymbolicPacket &a) {
    attach(port, SymbolicPacket(a));
}

template <class T>
void attachFunction(XMASFunction *c, T &&f)
{
    SymbolicFunctionExtension<std::vector<SymbolicPacket> (const std::vector<SymbolicPacket>&)> *ext = c->getComponentExtension<SymbolicFunctionExtension<std::vector<SymbolicPacket> (const std::vector<SymbolicPacket>&)>>();
    ext->function = f;
}

template <class T>
void attachInverseFunction(XMASFunction *c, T &&f) {
  auto *ext = c->getComponentExtension<
      SymbolicInverseFunctionExtension<SymbolicPacket(const SymbolicPacket &)>>();
  ext->function = f;
}

template <class T>
void attachFunction(XMASJoin *c, T &&f)
{
    SymbolicFunctionExtension<SymbolicPacket (const SymbolicPacket&, const SymbolicPacket&)> *ext = c->getComponentExtension<SymbolicFunctionExtension<SymbolicPacket (const SymbolicPacket&, const SymbolicPacket&)>>();
    ext->function = f;
}

void printSymbolicTypeInfo(std::ostream &out, Port *port);

void SymbolicTypes(std::set<XMASComponent *> allComponents);
void ClearSymbolicTypes(std::set<XMASComponent *> allComponents);
void ClearSymbolicTypes(XMASComponent *c);



class SymbolicAnyField : public SymbolicPacketField {
public:
    SymbolicAnyField() : SymbolicPacketField(bitpowder::lib::FastType<SymbolicAnyField>::value) {
    }
    virtual std::vector<std::shared_ptr<SymbolicPacketField>> getIntersection(const std::shared_ptr<SymbolicPacketField> & a) const;
    virtual void getDifference(const std::shared_ptr<SymbolicPacketField> & a, std::function<void (std::shared_ptr<SymbolicPacketField> &&)> &&f) const;
    virtual bool operator==(const SymbolicPacketField &b) const;
    virtual bool valid() const;
    virtual bool empty() const;
    virtual bool contains(const std::shared_ptr<SymbolicPacketField> &f) const;
    virtual bool isCombinable(const std::shared_ptr<SymbolicPacketField> &f) const;
    virtual std::shared_ptr<SymbolicPacketField> combine(const std::shared_ptr<SymbolicPacketField> &f) const;
    virtual void print(std::ostream &out) const;
    virtual int hash() const;
    virtual void updateHash();
    std::vector<std::shared_ptr<SymbolicPacketField>> negate() const;
    void accept(const bitpowder::lib::String& field, SymbolicFieldVisitor &v);
    void printOldCSyntax(std::ostream& out, std::map<bitpowder::lib::String, int>& enumMap, const bitpowder::lib::String& key) const;
};

namespace std {
template<>
    struct hash<SymbolicPacket> : public unary_function<SymbolicPacket, size_t>
    {
        size_t operator()(SymbolicPacket const& s) const
        {
            return s.hash();
        }
    };
}

#endif // SYMBOLIC_H
