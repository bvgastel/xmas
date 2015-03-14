#ifndef XMAS_H
#define XMAS_H

#include "extension.h"
#include "simplestring.h"
//#include "parse-source-expression-parse-result.h"

/*
 * GBO: added setter for name to XMASComponent: necessary for designer or we have no way to
 *      initialize and change name. The first name would be final, no way to change.
 *
 */

/**
 * @brief The SignalType enum type of wires/signals
 */
enum SignalType {IRDY, TRDY, DATA};
/**
 * @brief The PortType enum selector to state which ports you are interested in
 *
 * REMARK: renamed INPUT to INPUT_PORT and OUTPUT to OUTPUT_PORT due to
 *         error for redefining typedef INPUT from windows header winuser.h
 */
enum PortType {ALL, INPUT_PORT, OUTPUT_PORT};

class XMASComponent;

// to each port we should
// extensions to a port should be of type PortExtension
class PortExtension : public bitpowder::lib::Extension<PortExtension>
{
};

class Input;
class Output;


/**
 * @brief The PortVisitor class
 *
 * This class represents a visitor class in accordance with the visitor
 * design pattern.
 *
 * The visitor pattern, an algorithm and the data structure are independent.
 *
 * The algorithm should extend PortVisitor and is called with the data structure
 * elements
 *
 */
class PortVisitor
{
public:
    virtual void visit(Input *) = 0;
    virtual void visit(Output *) = 0;
};

/**
 * @brief The Port class
 *
 * A port on an xMas component.
 *
 */
class Port : bitpowder::lib::ExtensionContainer<PortExtension>
{
    // due to compiler bug in GCC 4.8, can not be std::string
    // to avoid a memory leak (Input has an "o" argument as
    // name, then "o" get constructed and never deallocted)
    // (maybe due to a lack of virtual descrtuctor)
    const char *name;
public:
    /**
     * @brief self pointer to the containing component
     *
     * FIXME: should this pointer be public? if so: write motivation
     * FIXME: why is this called self, while it points to containing component? --> m_owner
     * NOTE: Changed self to m_owner
     */
    XMASComponent *m_owner;
    /**
     * @brief Port
     *
     * @param self pointer to self
     * @param name the name of the port
     */
    Port(XMASComponent *owner, const char *name) : ExtensionContainer<PortExtension>(), name(name), m_owner(owner)
    {
    }

    virtual ~Port();

    /**
     * @brief getComponent
     *
     * @return the pointer to this component of which this port is part
     */
    XMASComponent *getComponent() const
    {
        return m_owner;
    }

    // the name of the component, e.g. "a", "b", "i", "o"
    const char *getName() const
    {
        return name;
    }

    // this port is valid (is part of a component)
    /**
     * @brief valid true if the port is valid
     *
     * @return true, if the port is part of a component
     */
    virtual bool valid()
    {
        return m_owner != nullptr;
    }
    // abstract methods getting the target port/component or initiator port/component
    /**
     * @brief getTargetPort
     *
     * (abstract method)
     *
     * @return the target port
     */
    virtual Input *getTargetPort() = 0;
    /**
     * @brief getInitiatorPort
     *
     * (abstract method)
     *
     * @return the initiator port
     */
    virtual Output *getInitiatorPort() = 0;
    /**
     * @brief getTarget
     *
     * @return the XMASComponent that owns the target port
     */
    XMASComponent *getTarget();
    /**
     * @brief getInitiator
     *
     * @return the XMASComponent that owns the initiator port
     */
    XMASComponent *getInitiator();

    /**
     * @brief isConnected
     *
     * @return true if the port is connected to a channel
     */
    virtual bool isConnected() = 0;
    /**
     * @brief connectedTo
     *
     * @param c an XMASComponent
     * @return true if this port is connected to the
     *          specified XMASComponent
     */
    virtual bool connectedTo(XMASComponent *c) = 0;

    // support for the visitor pattern
    /**
     * @brief accept support for the visitor pattern
     *
     * (abstract method)
     *
     * FIXME: why is this method abstract? There is no reason,
     *      as all sub classes have the same contents.
     *
     * @param t the port visitor
     */
    virtual void accept(PortVisitor &t) = 0;

    /**
     * @brief getPortExtension get an extension of a specific type, allocates if not exists
     *
     *  (template on class PortExtensionType).
     *
     * @tparam PortExtensionType
     *
     * @param create boolean indication whether to create
     *
     * @return a pointer to the PortExtensionType
     */
    template <class PortExtensionType>
    PortExtensionType* getPortExtension(bool create = true)
    {
        PortExtensionType *ext = getExtension<PortExtensionType*>();
        if (ext == nullptr && create) {
            ext = new PortExtensionType();
            addExtension(ext);
        }
        return ext;
    }

    /**
     * @brief clearPortExtension clear an extension of a specific type
     *
     *  (template on class PortExtensionType).
     *
     * @tparam PortExtensionType
     *
     */
    template <class PortExtensionType>
    void clearPortExtension()
    {
        PortExtensionType *ext = removeExtension<PortExtensionType*>();
        if (ext)
            delete ext;
    }

    /**
     * @brief clearExtensions deletes all port extensions
     */
    void clearExtensions();
};

//
/**
 * @brief operator <<
 *
 * ability to print a port to std::cout, etc
 *
 * @param out The output stream
 * @param c the port
 * @return a reference to the output stream
 */
std::ostream &operator <<(std::ostream &out, const Port &c);

/**
 * @brief The Output port
 *
 * This class represents an output port.
 *
 *
 */
class Output : public Port
{
    friend class Input;
    friend void connect(Output &o, Input &i);
    /**
     * @brief output This is a pointer to the output port that is connected
     *          to this input port.
     */
    Input *output;
public:
    Output(XMASComponent *owner, const char *name) : Port(owner, name), output(nullptr)
    {
    }
    Input *getTargetPort();
    Output *getInitiatorPort();
    bool isConnected();
    bool connectedTo(XMASComponent *c);
    bool valid();

    void accept(PortVisitor &t)
    {
        t.visit(this);
    }
};

class Input : public Port
{
    friend class Output;
    friend void connect(Output &o, Input &i);
    // FIXME: why is this Output pointer named input ?
    Output *input;
public:
    Input(XMASComponent *self, const char *name) : Port(self, name), input(nullptr)
    {
    }
    Output *getInitiatorPort();
    Input *getTargetPort();
    bool isConnected();
    bool connectedTo(XMASComponent *c);
    bool valid();

    void accept(PortVisitor &t)
    {
        t.visit(this);
    }
};

// connect an Output to an Input
/**
 * @brief connect
 *
 * Connects an output to an input
 *
 * @param o the output port
 * @param i the input port
 */
void connect(Output &o, Input &i);

/**
 * @brief The XMASComponentExtension class
 *
 * This class represents  .....
 *
 * FIXME: describe the XMASComponentExtension class properly: the class contents is empty
 *
 */
class XMASComponentExtension : public bitpowder::lib::Extension<XMASComponentExtension>
{
};

/*
 * forward declarations of primitives.
 *
 */
class XMASSink;
class XMASSource;
class XMASQueue;
class XMASFunction;
class XMASSwitch;
class XMASFork;
class XMASMerge;
class XMASJoin;

/**
 * @brief The XMASComponentVisitor class
 *
 * A class to support the visitor pattern for XMASComponent.
 *
 */
class XMASComponentVisitor
{
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
/************************ XMASComponent *******************************************/

struct ExpressionResult {
    bool m_success;
    int m_pos;
    bitpowder::lib::String m_errMsg;

    ExpressionResult() : m_success(false), m_pos(0), m_errMsg() {}
    ExpressionResult(bool success, int pos, bitpowder::lib::String errMsg) :
        m_success(success), m_pos(pos), m_errMsg(errMsg) {}
};

/**
 * @brief The XMASComponent class
 *
 * FIXME: describe the XMASComponent class properly
 *
 */
class XMASComponent : public bitpowder::lib::ExtensionContainer<XMASComponentExtension>
{
private:
    std::string m_name;
public:

    XMASComponent(const bitpowder::lib::String& name) : m_name(name.stl())
    {
    }

    virtual ~XMASComponent();

    /**
     * @brief name the name of the component
     * @return a value initialized copy of name (std::string)
     */
    std::string getStdName() const {
        return this->m_name;
    }

    /**
     * @brief getName getter for name
     *
     * This getter is specifically for use in a memory sensitive environment.
     * The bitpowder::lib::String class reuses memory among strings.
     *
     * NOTE: Check reason for returning bitpowder::lib::String with Bernard.
     *
     * @return a bitpowder::lib::String for name of the XMASComponent
     */
    bitpowder::lib::String getName() const;

    /**
     * @brief name A function to (re)set the name of the component.
     *
     * This is function was primarily created to support direct updates
     * from qml using getters and setters. Also, the human designer uses
     * the designer to change the name, if appropriate.
     *
     * @param name The new name of the component.
     */
    void name(std::string name) {
        this->m_name = name;
    }

    /**
     * @brief canvasData getter for the canvas properties
     * @return a tuple containing the canvas related data.
     */
    std::tuple<int, int, int, float> canvasData();

    /**
     * @brief canvasData setter for the canvas related data
     * @param x the x coordinate of the component
     * @param y the y coordinate of the component
     * @param orientation the orientation of the component on the board
     *        in degrees
     * @param scale the scale of the component in relation to other components
     *              the default scale = 1.0f
     */
    void canvasData(int x, int y, int orientation, float scale);

    /**
     * @brief valid
     * @return true if the XMASComponent is valid
     */
    bool valid();

    /**
     * @brief The PortIterators struct
     *
     * @tparam typename Iterator
     */
    template <typename Iterator>
    struct PortIterators {
        Iterator _begin;
        Iterator _end;
        PortIterators(Iterator begin, Iterator end) : _begin(begin), _end(end)
        {
        }
        Iterator begin()
        {
            return _begin;
        }
        Iterator end()
        {
            return _end;
        }
    };

    /**
     * @brief beginPort
     *
     * The first port of this XMASComponent
     *
     * @param type the PortType
     *
     * @return an array of pointer to port: a pointer to the first port
     */
    virtual Port** beginPort(PortType type) = 0;
    /**
     * @brief endPort
     *
     * @param type the PortType
     *
     * @return an array of pointern to port: one past the last port.
     */
    virtual Port** endPort(PortType type) = 0;

    /**
     * @brief ports
     *
     * The struct containing the ports of all types.
     *
     * @param type the port type (default: ALL)
     *
     * @return a PortIterators struct
     */
    PortIterators<Port**> ports(PortType type = PortType::ALL);
    /**
     * @brief inputPorts
     *
     * The struct containing the ports of input types.
     *
     * @return a PortIterators struct
     */
    PortIterators<Input**> inputPorts();
    /**
     * @brief outputPorts
     *
     * The struct containing the ports of output types.
     *
     * @return a PortIterators struct
     */
    PortIterators<Output**> outputPorts();

    /**
     * @brief clearExtensions
     */
    void clearExtensions();

    /**
     * @brief accept support for the visitor pattern
     *
     * (abstract method)
     *
     * FIXME: why is this method abstract? All subclasses do the same.
     *
     * @param v the XMASComponentVisitor
     *
     */
    virtual void accept(XMASComponentVisitor &v) = 0;

    /**
     * @brief getComponentExtension
     *
     * @tparam class ComponentExtensionType
     *
     * @param create indication whether to create (default = true).
     *
     * @return the ComponentExtionsionType
     */
    template <class ComponentExtensionType>
    ComponentExtensionType* getComponentExtension(bool create = true)
    {
        ComponentExtensionType *ext = getExtension<ComponentExtensionType*>();
        if (ext == nullptr && create) {
            ext = new ComponentExtensionType();
            addExtension(ext);
        }
        return ext;
    }

    /**
     * @brief clearComponentExtension
     *
     * clears and deletes the extension
     *
     * @tparam class ComponentExtentionType
     *
     */
    template <class ComponentExtensionType>
    void clearComponentExtension()
    {
        ComponentExtensionType *ext = removeExtension<ComponentExtensionType*>();
        if (ext)
            delete ext;
    }
}; // class XMASComponent

/**
 * @brief operator <<
 *
 * output operator for serializing an XMASComponent to the output stream.
 *
 * @param out reference to the output stream
 * @param c reference to the XMASComponent
 * @return reference to the output stream
 */
std::ostream &operator <<(std::ostream &out, const XMASComponent &c);

/*********************** Primitives ************************************/

/**
 * @brief The XMASSink class
 *
 * The class that represents the primitive definition of a sink.
 * This class is a subclass of XMASComponent.
 *
 * It has only one input port.
 *
 */
class XMASSink : public XMASComponent
{
public:
    Input i;
    Port* p[1];


    /**
     * @brief XMASSink Constructor
     *
     * @param name the name of the sink
     */
    XMASSink(const bitpowder::lib::String& name)
        : XMASComponent(name), i(this, "i")
    {
        p[0] = &i;
    }

    void accept(XMASComponentVisitor &v)
    {
        v.visit(this);
    }

    /**
     * @brief beginPort Pointer to the first port of type requested
     *
     * @param type the type of the port requested
     * @return one past the last port for the type requested, or nullptr if no
     *          ports of that type are available
     */
    Port** beginPort(PortType type)
    {
        return type == PortType::OUTPUT_PORT ? nullptr : &p[0];
    }
    /**
     * @brief endPort pointer to one past the last port of type requested
     *
     * @param type the type of the port requested
     * @return one past the last port for the type requested, or nullptr if no
     *          ports of that type are available
     */
    Port** endPort(PortType type)
    {
        return type == PortType::OUTPUT_PORT ? nullptr : &p[1];
    }
};

/**
 * @brief The XMASSource class
 *
 * The class that represents the primitive definition of a source.
 * This class is a subclass of XMASComponent.
 *
 * It has only one output port.
 */
class XMASSource : public XMASComponent
{
public:
    Output o;
    Port* p[1];

    XMASSource(const bitpowder::lib::String& name) : XMASComponent(name), o(this, "o")
    {
        p[0] = &o;
    }

    std::string getSourceExpression(bitpowder::lib::MemoryPool &mp);

    ExpressionResult setSourceExpression(bitpowder::lib::String &expr,
                                         bitpowder::lib::MemoryPool &mp);
    ExpressionResult setSourceExpression(std::string &expr,
                                         bitpowder::lib::MemoryPool &mp);

    void accept(XMASComponentVisitor &v)
    {
        v.visit(this);
    }
    /**
     * @brief beginPort Pointer to the first port of type requested
     *
     * @param type the type of the port requested
     * @return one past the last port for the type requested, or nullptr if no
     *          ports of that type are available
     */
    Port** beginPort(PortType type)
    {
        return type == PortType::INPUT_PORT ? nullptr : &p[0];
    }
    /**
     * @brief endPort pointer to one past the last port of type requested
     *
     * @param type the type of the port requested
     * @return one past the last port for the type requested, or nullptr if no
     *          ports of that type are available
     */
    Port** endPort(PortType type)
    {
        return type == PortType::INPUT_PORT ? nullptr : &p[1];
    }
};

/**
 * @brief The XMASQueue class
 *
 * The class that represents the primitive definition of a queue.
 * This class is a subclass of XMASComponent.
 *
 * It has one input port and one output port and a storage with specified capacity.
 */
class XMASQueue : public XMASComponent
{
public:
    Input i;
    Output o;
    Port* p[2];
    size_t c; // capacity

    /**
     * @brief XMASQueue constructor with the correct ports defined
     * @param name the name of the queue.
     * @param capacity the capacity of the queue.
     */
    XMASQueue(const bitpowder::lib::String& name, size_t capacity = 1)
        : XMASComponent(name), i(this,"i"), o(this,"o"), c(capacity)
    {
        p[0] = &i;
        p[1] = &o;
    }

    void accept(XMASComponentVisitor &v)
    {
        v.visit(this);
    }
    /**
     * @brief beginPort Pointer to the first port of type requested
     *
     * @param type the type of the port requested
     * @return the correct port for each type
     */
    Port** beginPort(PortType type)
    {
        return type == PortType::OUTPUT_PORT ? &p[1] : &p[0];
    }
    /**
     * @brief endPort pointer to one past the last port of type requested
     *
     * @param type the type of the port requested
     * @return one past the last port for the type requested, or nullptr if no
     *          ports of that type are available
     */
    Port** endPort(PortType type)
    {
        return type == PortType::INPUT_PORT ? &p[1] : &p[2];
    }
};

/**
 * @brief The XMASFunction class
 *
 * The class that represents the primitive definition of a function.
 * This class is a subclass of XMASComponent.
 *
 * It has one input port and one output port and a string specification of its
 * function.
 */
class XMASFunction : public XMASComponent
{
public:
    Input i;
    Output o;
    Port* p[2];

    XMASFunction(const bitpowder::lib::String& name)
        : XMASComponent(name), i(this, "i"), o(this, "o")
    {
        p[0] = &i;
        p[1] = &o;
    }


    const bitpowder::lib::String getFunctionExpression(bitpowder::lib::MemoryPool &mp);

    ExpressionResult setFunctionExpression(std::string &str_expr,
                                           bitpowder::lib::MemoryPool &mp);
    ExpressionResult setFunctionExpression(bitpowder::lib::String &expr,
                                           bitpowder::lib::MemoryPool &mp);


    void accept(XMASComponentVisitor &v)
    {
        v.visit(this);
    }

    /**
     * @brief beginPort Pointer to the first port of type requested
     *
     * @param type the type of the port requested
     * @return the correct port for each type
     */
    Port** beginPort(PortType type)
    {
        return type == PortType::OUTPUT_PORT ? &p[1] : &p[0];
    }
    /**
     * @brief endPort pointer to one past the last port of type requested
     *
     * @param type the type of the port requested
     * @return one past the last port for the type requested, or nullptr if no
     *          ports of that type are available
     */
    Port** endPort(PortType type)
    {
        return type == PortType::INPUT_PORT ? &p[1] : &p[2];
    }
};

/**
 * @brief The XMASSwitch class
 *
 * The class that represents the primitive definition of a switch.
 * This class is a subclass of XMASComponent.
 *
 * It has one input port and two output ports and
 * logic to decide which output channel to chose for a package
 * NOTE: the logic is not part of this class
 */
class XMASSwitch : public XMASComponent
{
public:
    Input i;
    Output a;
    Output b;
    Port* p[3];

    XMASSwitch(const bitpowder::lib::String& name) : XMASComponent(name), i(this, "i"), a(this,"a"), b(this,"b")
    {
        p[0] = &i;
        p[1] = &a;
        p[2] = &b;
    }

    void accept(XMASComponentVisitor &v)
    {
        v.visit(this);
    }

    /**
     * @brief beginPort Pointer to the first port of type requested
     *
     * @param type the type of the port requested
     * @return the correct port for each type
     */
    Port** beginPort(PortType type)
    {
        return type == PortType::OUTPUT_PORT ? &p[1] : &p[0];
    }
    /**
     * @brief endPort pointer to one past the last port of type requested
     *
     * @param type the type of the port requested
     * @return one past the last port for the type requested, or nullptr if no
     *          ports of that type are available
     */
    Port** endPort(PortType type)
    {
        return type == PortType::INPUT_PORT ? &p[1] : &p[3];
    }
};

/**
 * @brief The XMASFork class
 *
 * The class that represents the primitive definition of a fork.
 * This class is a subclass of XMASComponent.
 *
 * It has one input port and two output ports.
 * In the network execution it copies input to both output ports.
 *
 */
class XMASFork : public XMASComponent
{
public:
    Input i;
    Output a;
    Output b;
    Port* p[3];

    XMASFork(const bitpowder::lib::String& name) : XMASComponent(name), i(this,"i"), a(this,"a"), b(this,"b")
    {
        p[0] = &i;
        p[1] = &a;
        p[2] = &b;
    }

    void accept(XMASComponentVisitor &v)
    {
        v.visit(this);
    }
    /**
     * @brief beginPort Pointer to the first port of type requested
     *
     * @param type the type of the port requested
     * @return the correct port for each type
     */
    Port** beginPort(PortType type)
    {
        return type == PortType::OUTPUT_PORT ? &p[1] : &p[0];
    }
    /**
     * @brief endPort pointer to one past the last port of type requested
     *
     * @param type the type of the port requested
     * @return one past the last port for the type requested, or nullptr if no
     *          ports of that type are available
     */
    Port** endPort(PortType type)
    {
        return type == PortType::INPUT_PORT ? &p[1] : &p[3];
    }
};

/**
 * @brief The XMASMerge class
 *
 * The class that represents the primitive definition of a merge.
 * This class is a subclass of XMASComponent.
 *
 * It has one input port and two output ports and
 * logic to decide how to combine the input into output
 * Remark: the logic is not part of this class
 *
 * FIXME: in one of the papers a merge could have any number of output channels: true?
 *
 *
 */
class XMASMerge : public XMASComponent
{
public:
    Input a;
    Input b;
    Output o;
    Port* p[3];

    XMASMerge(const bitpowder::lib::String& name) : XMASComponent(name), a(this,"a"), b(this,"b"), o(this,"o")
    {
        p[0] = &a;
        p[1] = &b;
        p[2] = &o;
    }

    void accept(XMASComponentVisitor &v)
    {
        v.visit(this);
    }
    /**
     * @brief beginPort Pointer to the first port of type requested
     *
     * @param type the type of the port requested
     * @return the correct port for each type
     */
    Port** beginPort(PortType type)
    {
        return type == PortType::OUTPUT_PORT ? &p[2] : &p[0];
    }
    /**
     * @brief endPort pointer to one past the last port of type requested
     *
     * @param type the type of the port requested
     * @return one past the last port for the type requested, or nullptr if no
     *      ports of that type are available
     */
    Port** endPort(PortType type)
    {
        return type == PortType::INPUT_PORT ? &p[2] : &p[3];
    }
};

/**
 * @brief The XMASJoin class
 *
 * The class that represents the primitive definition of a join.
 * This class is a subclass of XMASComponent.
 *
 * It has two input ports and one output port and
 * logic to decide how to send input into output
 * Remark: the logic is not part of this class
 *
 */
class XMASJoin : public XMASComponent
{
public:
    Input a;
    Input b;
    Output o;
    Port* p[3];

    XMASJoin(const bitpowder::lib::String& name) : XMASComponent(name), a(this,"a"), b(this,"b"), o(this,"o")
    {
        p[0] = &a;
        p[1] = &b;
        p[2] = &o;
    }

    void accept(XMASComponentVisitor &v)
    {
        v.visit(this);
    }
    /**
     * @brief beginPort Pointer to the first port of type requested
     *
     * @param type the type of the port requested
     * @return the correct port for each type
     */
    Port** beginPort(PortType type)
    {
        return type == PortType::OUTPUT_PORT ? &p[2] : &p[0];
    }
    /**
     * @brief endPort pointer to one past the last port of type requested
     *
     * @param type the type of the port requested
     * @return one past the last port for the type requested, or nullptr if no
     *      ports of that type are available
     */
    Port** endPort(PortType type)
    {
        return type == PortType::INPUT_PORT ? &p[2] : &p[3];
    }
};

#endif // XMAS_H
