#ifndef MESSAGESPEC_H
#define MESSAGESPEC_H

#include <string>
#include <vector>
#include <algorithm>
#include <set>
#include <functional>
#include "xmas.h"
#include <iostream>
#include <memory>
#include "symbolic.h"
#include <tuple>

#include "lib/shared_object.h"

using namespace bitpowder::lib;

#define USE_REFCOUNT

class MessageSpec {
public:
    FastTypeT type;
#ifdef USE_REFCOUNT
    int refcount;
    typedef shared_object<MessageSpec> Ref;
    MessageSpec(FastTypeT type) : type(type), refcount(0) {
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
    virtual void lookup(std::map<String,XMASComponent*> mapping) = 0;
};

std::ostream &operator <<(std::ostream &out, const MessageSpec &c);

class MessageSpecEndLookup : public MessageSpec {
    String endpointName;
    XMASComponent *endpoint;
    std::vector<SymbolicPacket> packets;
public:
    MessageSpecEndLookup(String endpointName, const std::vector<SymbolicPacket> &packets) : MessageSpec(FastType<MessageSpecEndLookup>::value), endpointName(endpointName), endpoint(nullptr), packets(packets) {
    }
    void print(std::ostream &out) const;
    bool operator==(const Ref &b) const;
    void lookup(std::map<String,XMASComponent*> mapping);
};

class MessageSpecEnd : public MessageSpec {
    XMASComponent *endpoint;
    std::vector<SymbolicPacket> packets;
public:
    MessageSpecEnd(XMASComponent *endpoint, const std::vector<SymbolicPacket> &packets) : MessageSpec(FastType<MessageSpecEnd>::value), endpoint(endpoint), packets(packets) {
    }
    void print(std::ostream &out) const;
    bool operator==(const Ref &b) const;
    void lookup(std::map<String,XMASComponent*> mapping) {}
};

class MessageSpecAnd : public MessageSpec {
    Ref a;
    Ref b;
public:
    MessageSpecAnd(Ref a, Ref b) : MessageSpec(FastType<MessageSpecAnd>::value), a(a), b(b) {
    }
    void print(std::ostream &out) const;
    bool operator==(const Ref &b) const;
    void lookup(std::map<String,XMASComponent*> mapping);
};

class MessageSpecOr : public MessageSpec {
    Ref a;
    Ref b;
public:
    MessageSpecOr(Ref a, Ref b) : MessageSpec(FastType<MessageSpecOr>::value), a(a), b(b) {
    }
    void print(std::ostream &out) const;
    bool operator==(const Ref &b) const;
    void lookup(std::map<String,XMASComponent*> mapping);
};

class MessageSpecError : public MessageSpec {
    std::string description;
public:
    MessageSpecError(const std::string &description) : MessageSpec(FastType<MessageSpecError>::value), description(description) {
    }
    void print(std::ostream &out) const;
    bool operator==(const Ref &b) const;
    void lookup(std::map<String,XMASComponent*> mapping) {}
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

inline MessageSpec::Ref S(String endpoint, const std::vector<SymbolicPacket> &content) {
    return new MessageSpecEndLookup(endpoint, content);
}

// returns true on error
bool CheckMessageSpec(std::set<XMASComponent *> allComponents);
void ClearMessageSpec(std::set<XMASComponent *> allComponents);

class MessageSpecExtension : public PortExtension {
public:
    std::vector<std::tuple<std::vector<SymbolicPacket>, MessageSpec::Ref>> specs;

    MessageSpecExtension() : PortExtension(), specs() {
    }
    void addMessageSpec(const std::vector<SymbolicPacket> &p, MessageSpec::Ref s) {
        specs.emplace_back(p, s);
    }
};

void attachMessageSpec(Output *port, const std::vector<SymbolicPacket> &a, MessageSpec::Ref spec);
void clearMessageSpec(Output *port);

#endif // MESSAGESPEC_H
