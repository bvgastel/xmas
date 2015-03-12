#include <string>
#include <sstream>
#include <cstring>
#include <strings.h>

#include "xmas.h"
#include "canvascomponentextension.h"
#include "simplestring.h"
#include "parse.h"
#include "messagespec.h"

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
    return m_name;
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


std::tuple<int, int, int, float> XMASComponent::canvasData() {
    CanvasComponentExtension *ext = this->getExtension<CanvasComponentExtension *>();
    if (ext) {
        return ext->canvasData();
    }
    return std::make_tuple(0, 0, 0, 1.0f);
}

void XMASComponent::canvasData(int x, int y, int orientation, float scale) {
    CanvasComponentExtension *ext = this->getExtension<CanvasComponentExtension *>();
    if (!ext) {
        try {
            CanvasComponentExtension* cce = new CanvasComponentExtension {x, y, orientation, scale};
            this->addExtension(cce);
        } catch (bitpowder::lib::Exception e) {
            std::cerr << "Invalid canvas data: ["
                      << x << ", " << y << ", " << orientation << ", " << scale << "]"
                      << std::endl;
            std::cerr << e << std::endl;
            exit(-1);
        }
    }
    if (ext) {
        ext->canvasData(x, y, orientation, scale);
    }

}


bitpowder::lib::String XMASSource::getSourceExpression() {
    SymbolicTypesExtension *ext = this->o.getPortExtension<SymbolicTypesExtension>();
    auto packets = ext->availablePackets;
    std::ostringstream out;
    for (SymbolicPacket p : packets) {
        std::map<bitpowder::lib::String, int> enumMap;
        p.printOldCSyntax(out,enumMap);
    }
    bitpowder::lib::String packetStr(out.str());
    return std::move(packetStr);
}
ExpressionResult XMASSource::setSourceExpression(const std::string &expr) {
    bitpowder::lib::String b_expr = expr.c_str();
    return setSourceExpression(b_expr);
}

ExpressionResult XMASSource::setSourceExpression(const bitpowder::lib::String &expr) {

    std::ostringstream errMsg;

    Output *out = &o;

    if (!out->valid()) {
        std::cout << errMsg.str() << std::endl;
        errMsg << "[XMASSource::setSourceExpression] parsing expression '"
               << expr
               << "' not attached: port is not connected.";
        return ExpressionResult(false, -1, errMsg.str());
    }

    bitpowder::lib::MemoryPool mp;
    auto result = ParseSourceExpression(expr, mp);
    // note: result has a bool() operator
    if (result) {
        std::cout << "parsing " << expr << ": " << result.result() << std::endl;
        // FIXME: Storing the values has a memory problem due to extensive use of MemoryPool (temporary memory).
        for (auto &packet : result.result().spec) {
            std::vector<SymbolicPacket> packetSet = std::get<0>(packet).values;
            MessageSpec::Ref msgRef = std::get<1>(packet);
            attachMessageSpec(out, packetSet, msgRef);
        }
    }

    return ExpressionResult(result, result.position(), result.error());
}

const bitpowder::lib::String XMASFunction::getFunctionExpression() {
    bool createExtension = true;
    ParsedXMASFunctionExtension *ext = this->getComponentExtension<ParsedXMASFunctionExtension>(!createExtension);
    std::map<bitpowder::lib::String,int> enumMap;
    std::ostringstream tmp;
    ext->value->printOldCSyntax(tmp, enumMap);
    bitpowder::lib::String function = bitpowder::lib::String(tmp.str());
    return std::move(function);
}

ExpressionResult XMASFunction::setFunctionExpression(const std::string &str_expr) {
    const bitpowder::lib::String expr(str_expr);
    return setFunctionExpression(expr);
}

ExpressionResult XMASFunction::setFunctionExpression(const bitpowder::lib::String &expr) {

    bitpowder::lib::MemoryPool mp;
    auto result = ParsePacketFunction (expr, mp);
    if (result) {
        std::cout << "parsing " << expr << ": " << result.result() << std::endl;
        auto func = result.result();
        this->addExtension(new ParsedXMASFunctionExtension(func));
        attachFunction(this, [func](const std::vector<SymbolicPacket> &p) {
            return (*func)(p);
        });
    } else {
        std::cerr << "parsing " << expr << std::endl;
        std::cerr << "error parsing at position " << result.position() << " is " << result.error() << std::endl;
        //exit(-1);
    }

    return ExpressionResult(result, result.position(), result.error());

}

