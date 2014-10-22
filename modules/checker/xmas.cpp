#include "xmas.h"

XMASComponent *Port::getTarget() {
    Port *targetPort = getTargetPort();
    return targetPort == nullptr ? nullptr : targetPort->self;
}

XMASComponent *Port::getInitiator() {
    Port *initPort = getInitiatorPort();
    return initPort == nullptr ? nullptr : initPort->self;
}

std::ostream &operator <<(std::ostream &out, const Port &p) {
    out << *p.getComponent() << "." << p.getName();
    return out;
}

std::ostream &operator <<(std::ostream &out, const XMASComponent &c) {
    out << c.getName();
    return out;
}

Input *Output::getTargetPort() {
    return output;
}

Output *Output::getInitiatorPort() {
    return this;
}

bool Output::isConnected() {
    return output != nullptr;
}

bool Output::connectedTo(XMASComponent *c) {
    return isConnected() && getTarget() == c;
}

bool Output::valid() {
    return Port::valid() && isConnected() && output->input == this;
}

Output *Input::getInitiatorPort() {
    return input;
}

bool Input::valid() {
    return Port::valid() && isConnected() && input->output == this;
}

void connect(Output &o, Input &i) {
    i.input = &o;
    o.output = &i;
}

XMASComponent::~XMASComponent()
{
    clearExtensions();
}

Port::~Port() {
    //clearExtensions();
}

bool XMASComponent::valid()
{
    //for (Port *p : getPorts())
    for (Port* p : ports())
        if (!p->valid())
            return false;
    return true;
}

void Port::clearExtensions() {
    auto extensions = ExtensionContainer<PortExtension>::clearExtensions();
    for (auto it = extensions.begin(); it != extensions.end(); )
        delete it.erase();
}

void XMASComponent::clearExtensions() {
    auto extensions = ExtensionContainer<XMASComponentExtension>::clearExtensions();
    for (auto it = extensions.begin(); it != extensions.end(); )
        delete it.erase();
}
