#include "xmas.h"
#include <string.h>
#include <cstring>
#include <strings.h>

XMASComponent *Port::getTarget() {
    Port *targetPort = getTargetPort();
    return targetPort == nullptr ? nullptr : targetPort->m_owner;
}

XMASComponent *Port::getInitiator() {
    Port *initPort = getInitiatorPort();
    return initPort == nullptr ? nullptr : initPort->m_owner;
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

Input *Input::getTargetPort() {
    return this;
}

bool Input::isConnected() {
    return input != nullptr;
}

bool Input::connectedTo(XMASComponent *c) {
    return isConnected() && getInitiator() == c;
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

bitpowder::lib::String XMASComponent::getName() const {
    return name;
}

Port::~Port() {
    //clearExtensions();
}

bool XMASComponent::valid()
{
    for (Port* p : ports())
        if (!p->valid())
            return false;
    return true;
}

XMASComponent::PortIterators<Port **> XMASComponent::ports(PortType type) {
    return {beginPort(type), endPort(type)};
}

XMASComponent::PortIterators<Input **> XMASComponent::inputPorts() {
    return {(Input**)beginPort(PortType::INPUT_PORT), (Input**)endPort(PortType::INPUT_PORT)};
}

XMASComponent::PortIterators<Output **> XMASComponent::outputPorts() {
    return {(Output**)beginPort(PortType::OUTPUT_PORT), (Output**)endPort(PortType::OUTPUT_PORT)};
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



XMASComposite::XMASComposite(const bitpowder::lib::String& name, XMASNetwork& network) : XMASComponent(name), network(network)
{
    // get in & out gates from network
    auto inGates = network.componentsOfType<XMASSource>();
    auto outGates = network.componentsOfType<XMASSink>();

    int numInGates = inGates.size();
    int numOutGates = outGates.size();

    inputs.reserve(numInGates);
    outputs.reserve(numOutGates);

    // for all in gates, create an input port
    for (auto c : inGates)
        if (c->external)
            inputs.push_back(Input {this, c->getStdName().c_str()} );

    // for all out gates, create an output port
    for (auto c : outGates)
        if (c->external)
            outputs.push_back(Output {this, c->getStdName().c_str()} );

    // fill p
    p.reserve(inputs.size() + outputs.size());

    for (auto& i : inputs)
        p.push_back(&i);
    for (auto& o : outputs)
        p.push_back(&o);
}
