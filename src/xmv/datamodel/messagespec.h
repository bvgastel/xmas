#ifndef MESSAGESPEC_H
#define MESSAGESPEC_H

#include <string>
#include <vector>
#include <algorithm>
#include <set>
#include <functional>
#include <iostream>
#include <memory>
#include <tuple>

#include "xmas.h"
#include "symbolic.h"

#include "shared_object.h"

#define USE_REFCOUNT

class MessageSpec {
public:
    bitpowder::lib::FastTypeT type;
#ifdef USE_REFCOUNT
    int refcount;
    typedef bitpowder::lib::shared_object<MessageSpec> Ref;
    MessageSpec(bitpowder::lib::FastTypeT type) : type(type), refcount(0) {
    }
    template <class T, class... Args>
    static Ref make_shared(Args &&... args) {
        return Ref(new T(std::forward<Args>(args)...));
    }
#else
    typedef std::shared_ptr<MessageSpec> Ref;
    template <class T, class... Args>
    static Ref make_shared(Args &&... args) {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }
#endif
    virtual ~MessageSpec() {
    }
    virtual void print(std::ostream &out) const = 0;
    virtual bool operator==(const Ref &b) const = 0;
    virtual void lookup(std::map<bitpowder::lib::String,XMASComponent*> mapping) = 0;
};

std::ostream &operator <<(std::ostream &out, const MessageSpec &c);

class MessageSpecEndLookup : public MessageSpec {
    bitpowder::lib::String endpointName;
    XMASComponent *endpoint;
    std::vector<SymbolicPacket> packets;
public:
    MessageSpecEndLookup(bitpowder::lib::String endpointName, const std::vector<SymbolicPacket> &packets)
        : MessageSpec(bitpowder::lib::FastType<MessageSpecEndLookup>::value),
          endpointName(endpointName),
          endpoint(nullptr),
          packets(packets) {
    }
    void print(std::ostream &out) const;
    bool operator==(const Ref &b) const;
    void lookup(std::map<bitpowder::lib::String,XMASComponent*> mapping);
};

class MessageSpecEnd : public MessageSpec {
    XMASComponent *endpoint;
    std::vector<SymbolicPacket> packets;
public:
    MessageSpecEnd(XMASComponent *endpoint, const std::vector<SymbolicPacket> &packets)
        : MessageSpec(bitpowder::lib::FastType<MessageSpecEnd>::value),
          endpoint(endpoint),
          packets(packets) {
    }
    void print(std::ostream &out) const;
    bool operator==(const Ref &b) const;
    void lookup(std::map<bitpowder::lib::String,XMASComponent*> mapping) {
        bitpowder::lib::unused(mapping);
    }
};

class MessageSpecAnd : public MessageSpec {
    Ref a;
    Ref b;
public:
    MessageSpecAnd(Ref a, Ref b)
        : MessageSpec(bitpowder::lib::FastType<MessageSpecAnd>::value), a(a), b(b) {
    }
    void print(std::ostream &out) const;
    bool operator==(const Ref &b) const;
    void lookup(std::map<bitpowder::lib::String,XMASComponent*> mapping);
};

class MessageSpecOr : public MessageSpec {
    Ref a;
    Ref b;
public:
    MessageSpecOr(Ref a, Ref b)
        : MessageSpec(bitpowder::lib::FastType<MessageSpecOr>::value), a(a), b(b) {
    }
    void print(std::ostream &out) const;
    bool operator==(const Ref &b) const;
    void lookup(std::map<bitpowder::lib::String,XMASComponent*> mapping);
};

class MessageSpecError : public MessageSpec {
    std::string description;
public:
    MessageSpecError(const std::string &description)
        : MessageSpec(bitpowder::lib::FastType<MessageSpecError>::value), description(description) {
    }
    void print(std::ostream &out) const;
    bool operator==(const Ref &b) const;
    void lookup(std::map<bitpowder::lib::String,XMASComponent*> mapping) {
        bitpowder::lib::unused(mapping);
    }
};

inline MessageSpec::Ref operator|(MessageSpec::Ref a, MessageSpec::Ref b) {
    return new MessageSpecOr(a, b);
}

inline MessageSpec::Ref operator&(MessageSpec::Ref a, MessageSpec::Ref b) {
    return new MessageSpecAnd(a, b);
}

inline MessageSpec::Ref S(XMASComponent *endpoint, const std::vector<SymbolicPacket> &content) {
    return new MessageSpecEnd(endpoint, content);
}

inline MessageSpec::Ref S(bitpowder::lib::String endpoint, const std::vector<SymbolicPacket> &content) {
    return new MessageSpecEndLookup(endpoint, content);
}

class MessageSpecExtension : public PortExtension {
public:
    std::vector<std::tuple<std::vector<SymbolicPacket>, MessageSpec::Ref>> specs;

    MessageSpecExtension() : PortExtension(), specs() {
    }
    void addMessageSpec(const std::vector<SymbolicPacket> &p, MessageSpec::Ref s) {
        specs.emplace_back(p, s);
    }
};

// returns true on error
bool CheckMessageSpec(std::set<XMASComponent *> allComponents);

void attachMessageSpec(Output *port, const std::vector<SymbolicPacket> &a, MessageSpec::Ref spec);

void ClearMessageSpec(std::set<XMASComponent *> allComponents);
void ClearMessageSpec(XMASComponent *c);
void clearMessageSpec(Output *port);

#endif // MESSAGESPEC_H
