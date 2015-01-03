#ifndef XMAS_H
#define XMAS_H

#include <deque>
#include <vector>
#include "lib/extension.h"
#include "lib/exception.h"
#include "lib/simplestring.h"

using namespace bitpowder::lib;

// type of wires/signals
enum SignalType {IRDY, TRDY, DATA};
// selector to state which ports you are interested in
enum PortType {ALL, INPUT, OUTPUT};

class XMASComponent;

// to each port we should
// extensions to a port should be of type PortExtension
class PortExtension : public Extension<PortExtension> {
};

class Input;
class Output;

// Using the visitor pattern, an algorithm and the data structure can be split. The algorithm should extend PortVisitor and is called with the data structure elements
class PortVisitor {
public:
    virtual void visit(Input *) = 0;
    virtual void visit(Output *) = 0;
};

// a port on an xMAS component
class Port : ExtensionContainer<PortExtension> {
    const char *name; // due to compiler bug in GCC 4.8, can not be std::string to avoid a memory leak (Input has an "o" argument as name, then "o" get constructed and never deallocted) (maybe due to a lack of virtual descrtuctor)
public:
    XMASComponent *self;
    Port(XMASComponent *self, const char *name) : ExtensionContainer<PortExtension>(), name(name), self(self) {
    }
    virtual ~Port();
    // the component this port is part of
    XMASComponent *getComponent() const {
        return self;
    }
    // the name of the component, e.g. "a", "b", "i", "o"
    const char *getName() const {
        return name;
    }
    // this port is valid (is part of a component)
    virtual bool valid() {
        return self != nullptr;
    }
    // abstract methods getting the target port/component or initiator port/component
    virtual Input *getTargetPort() = 0;
    virtual Output *getInitiatorPort() = 0;
    XMASComponent *getTarget();
    XMASComponent *getInitiator();

    virtual bool isConnected() = 0;
    virtual bool connectedTo(XMASComponent *c) = 0;

    // support for the visitor pattern
    virtual void accept(PortVisitor &t) = 0;

    // get an extension of a specific type, allocates if not exists
    template <class PortExtensionType>
    PortExtensionType* getPortExtension(bool create = true) {
        PortExtensionType *ext = getExtension<PortExtensionType*>();
        if (ext == nullptr && create) {
            ext = new PortExtensionType();
            addExtension(ext);
        }
        return ext;
    }

    // clear an extension of a specific type
    template <class PortExtensionType>
    void clearPortExtension() {
        PortExtensionType *ext = removeExtension<PortExtensionType*>();
        if (ext)
            delete ext;
    }

    // deletes all port extensions
    void clearExtensions();
};

// ability to print a port to std::cout, etc
std::ostream &operator <<(std::ostream &out, const Port &c);

class Output : public Port {
    friend class Input;
    friend void connect(Output &o, Input &i);
    Input *output;
public:
    Output(XMASComponent *self, const char *name) : Port(self, name), output(nullptr) {
    }
    Input *getTargetPort();
    Output *getInitiatorPort();
    bool isConnected();
    bool connectedTo(XMASComponent *c);
    bool valid();

    void accept(PortVisitor &t) {
        t.visit(this);
    }
};

class Input : public Port {
    friend class Output;
    friend void connect(Output &o, Input &i);
    Output *input;
public:
    Input(XMASComponent *self, const char *name) : Port(self, name), input(nullptr) {
    }
    Output *getInitiatorPort();
    Input *getTargetPort();
    bool isConnected();
    bool connectedTo(XMASComponent *c);
    bool valid();

    void accept(PortVisitor &t) {
        t.visit(this);
    }
};

// connect an Output to an Input
void connect(Output &o, Input &i);

class XMASComponentExtension : public Extension<XMASComponentExtension> {
};

class XMASSink;
class XMASSource;
class XMASQueue;
class XMASFunction;
class XMASSwitch;
class XMASFork;
class XMASMerge;
class XMASJoin;

class XMASComponentVisitor {
public:
    virtual void visit(XMASSink *) = 0;
    virtual void visit(XMASSource *) = 0;
    virtual void visit(XMASQueue *) = 0;
    virtual void visit(XMASFunction *) = 0;
    virtual void visit(XMASSwitch *) = 0;
    virtual void visit(XMASFork *) = 0;
    virtual void visit(XMASMerge *) = 0;
    virtual void visit(XMASJoin *) = 0;
};

class XMASComponent : public ExtensionContainer<XMASComponentExtension> {
    std::string name;
public:
    XMASComponent(const String& name) : name(name.stl()) {
    }
    virtual ~XMASComponent();
    String getName() const;
    bool valid();

    template <typename Iterator>
    struct PortIterators {
        Iterator _begin;
        Iterator _end;
        PortIterators(Iterator begin, Iterator end) : _begin(begin), _end(end) {
        }
        Iterator begin() {
            return _begin;
        }
        Iterator end() {
            return _end;
        }
    };

    virtual Port** beginPort(PortType type) = 0;
    virtual Port** endPort(PortType type) = 0;

    PortIterators<Port**> ports(PortType type = PortType::ALL);
    PortIterators<Input**> inputPorts();
    PortIterators<Output**> outputPorts();

    void clearExtensions();

    // support for the visitor pattern
    virtual void accept(XMASComponentVisitor &v) = 0;

    template <class ComponentExtensionType>
    ComponentExtensionType* getComponentExtension(bool create = true) {
        ComponentExtensionType *ext = getExtension<ComponentExtensionType*>();
        if (ext == nullptr && create) {
            ext = new ComponentExtensionType();
            addExtension(ext);
        }
        return ext;
    }

    template <class ComponentExtensionType>
    void clearComponentExtension() {
        ComponentExtensionType *ext = removeExtension<ComponentExtensionType*>();
        if (ext)
            delete ext;
    }
};

std::ostream &operator <<(std::ostream &out, const XMASComponent &c);

class XMASSink : public XMASComponent {
public:
    Input i;
    Port* p[1];

    XMASSink(const String& name) : XMASComponent(name), i(this, "i") {
        p[0] = &i;
    }

    void accept(XMASComponentVisitor &v) {
        v.visit(this);
    }

    Port** beginPort(PortType type) {
        return type == PortType::OUTPUT ? nullptr : &p[0];
    }
    Port** endPort(PortType type) {
        return type == PortType::OUTPUT ? nullptr : &p[1];
    }
};

class XMASSource : public XMASComponent {
public:
    Output o;
    Port* p[1];

    XMASSource(const String& name) : XMASComponent(name), o(this, "o") {
        p[0] = &o;
    }

    void accept(XMASComponentVisitor &v) {
        v.visit(this);
    }
    Port** beginPort(PortType type) {
        return type == PortType::INPUT ? nullptr : &p[0];
    }
    Port** endPort(PortType type) {
        return type == PortType::INPUT ? nullptr : &p[1];
    }
};

class XMASQueue : public XMASComponent {
public:
    Input i;
    Output o;
    Port* p[2];
    size_t c; // capacity

    XMASQueue(const String& name, size_t capacity = 1) : XMASComponent(name), i(this,"i"), o(this,"o"), c(capacity) {
        p[0] = &i;
        p[1] = &o;
    }

    void accept(XMASComponentVisitor &v) {
        v.visit(this);
    }
    Port** beginPort(PortType type) {
        return type == PortType::OUTPUT ? &p[1] : &p[0];
    }
    Port** endPort(PortType type) {
        return type == PortType::INPUT ? &p[1] : &p[2];
    }
};

class XMASFunction : public XMASComponent {
public:
    Input i;
    Output o;
    Port* p[2];

    XMASFunction(const String& name) : XMASComponent(name), i(this, "i"), o(this, "o") {
        p[0] = &i;
        p[1] = &o;
    }

    void accept(XMASComponentVisitor &v) {
        v.visit(this);
    }

    Port** beginPort(PortType type) {
        return type == PortType::OUTPUT ? &p[1] : &p[0];
    }
    Port** endPort(PortType type) {
        return type == PortType::INPUT ? &p[1] : &p[2];
    }
};

class XMASSwitch : public XMASComponent {
public:
    Input i;
    Output a;
    Output b;
    Port* p[3];

    XMASSwitch(const String& name) : XMASComponent(name), i(this, "i"), a(this,"a"), b(this,"b") {
        p[0] = &i;
        p[1] = &a;
        p[2] = &b;
    }

    void accept(XMASComponentVisitor &v) {
        v.visit(this);
    }

    Port** beginPort(PortType type) {
        return type == PortType::OUTPUT ? &p[1] : &p[0];
    }
    Port** endPort(PortType type) {
        return type == PortType::INPUT ? &p[1] : &p[3];
    }
};

class XMASFork : public XMASComponent {
public:
    Input i;
    Output a;
    Output b;
    Port* p[3];

    XMASFork(const String& name) : XMASComponent(name), i(this,"i"), a(this,"a"), b(this,"b") {
        p[0] = &i;
        p[1] = &a;
        p[2] = &b;
    }

    void accept(XMASComponentVisitor &v) {
        v.visit(this);
    }
    Port** beginPort(PortType type) {
        return type == PortType::OUTPUT ? &p[1] : &p[0];
    }
    Port** endPort(PortType type) {
        return type == PortType::INPUT ? &p[1] : &p[3];
    }
};

class XMASMerge : public XMASComponent {
public:
    Input a;
    Input b;
    Output o;
    Port* p[3];

    XMASMerge(const String& name) : XMASComponent(name), a(this,"a"), b(this,"b"), o(this,"o") {
        p[0] = &a;
        p[1] = &b;
        p[2] = &o;
    }

    void accept(XMASComponentVisitor &v) {
        v.visit(this);
    }
    Port** beginPort(PortType type) {
        return type == PortType::OUTPUT ? &p[2] : &p[0];
    }
    Port** endPort(PortType type) {
        return type == PortType::INPUT ? &p[2] : &p[3];
    }
};

class XMASJoin : public XMASComponent {
public:
    Input a;
    Input b;
    Output o;
    Port* p[3];

    XMASJoin(const String& name) : XMASComponent(name), a(this,"a"), b(this,"b"), o(this,"o") {
        p[0] = &a;
        p[1] = &b;
        p[2] = &o;
    }

    void accept(XMASComponentVisitor &v) {
        v.visit(this);
    }
    Port** beginPort(PortType type) {
        return type == PortType::OUTPUT ? &p[2] : &p[0];
    }
    Port** endPort(PortType type) {
        return type == PortType::INPUT ? &p[2] : &p[3];
    }
};

#endif // XMAS_H
