#ifndef FLATTENGATES
#define FLATTENGATES

#include "xmas.h"

using bitpowder::lib::Exception;

class XMASInGate : public XMASComponent
{
public:
    Output o;
    Input i_ext;    // external interface port
    Port* p[1];     // publish only the output port

    XMASInGate(const bitpowder::lib::String& name) : XMASComponent(name), o(this,"o"), i_ext(this,"i_ext")
    {
        p[0] = &o;
    }

    void accept(XMASComponentVisitor &) override
    {
        throw Exception("Unable to visit XMASInGate.");
    }

    Port** beginPort(PortType type) override
    {
        return type == PortType::INPUT_PORT ? nullptr : &p[0];
    }

    Port** endPort(PortType type) override
    {
        return type == PortType::INPUT_PORT ? nullptr : &p[1];
    }
};

class XMASOutGate : public XMASComponent
{
public:
    Input i;
    Output o_ext;   // external interface port
    Port* p[1];     // publish only the input port

    XMASOutGate(const bitpowder::lib::String& name) : XMASComponent(name), i(this,"i"), o_ext(this,"o_ext")
    {
        p[0] = &i;
    }

    void accept(XMASComponentVisitor &) override
    {
        throw Exception("Unable to visit XMASOutGate.");
    }

    Port** beginPort(PortType type) override
    {
        return type == PortType::OUTPUT_PORT ? nullptr : &p[0];
    }

    Port** endPort(PortType type) override
    {
        return type == PortType::OUTPUT_PORT ? nullptr : &p[1];
    }
};

#endif // FLATTENGATES

