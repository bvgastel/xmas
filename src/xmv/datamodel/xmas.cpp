#include <string>
#include <sstream>
#include <cstring>
#include <strings.h>

#include "xmas.h"
#include "canvascomponentextension.h"
#include "simplestring.h"
#include "parse.h"
#include "messagespec.h"
#include "memorypool.h"
#include "export.h"

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

/*
 * WARNING: does this need any validity checks?
 * Cases to check:
 *  - output port is already connected
 *  - input port is already connected
 *  - type of data on both ports?
 *
 */
void connect(Output &o, Input &i) {
    i.input = &o;
    o.output = &i;
}

/**
 * @brief disconnect disconnect a channel from an output
 *
 * Provided a channel is valid i.e. o.valid(), both pointers
 * from output to input port and back are nullified.
 *
 * This function is necessary for the graphical designer, where
 * a designing person may delete a channel.
 *
 * @param o the output port of the channel.
 */

void disconnect(Output &o) {
    if (o.valid()) {
        Input *i = o.output;
        i->input = nullptr;
        o.output = nullptr;
    }
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


bitpowder::lib::String XMASSource::getSourceExpression(bitpowder::lib::MemoryPool &mp) {
    //bitpowder::lib::StaticMemoryPool<128> mp;
    return Export(this, mp);
}
ExpressionResult XMASSource::setSourceExpression(std::string &expr,
                                                 bitpowder::lib::MemoryPool &mp) {
    bitpowder::lib::String b_expr = expr.c_str();
    return setSourceExpression(b_expr, mp);
}

ExpressionResult XMASSource::setSourceExpression(bitpowder::lib::String &expr,
                                                 bitpowder::lib::MemoryPool &mp) {

    // To prevent local temporary variables from losing memory
    expr = expr(mp);

    std::ostringstream errMsg;

    Output *out = &o;

    if (!out->valid()) {
        errMsg << "[XMASSource::setSourceExpression] parsing expression '"
               << expr
               << "' not attached: port is not connected.";
        std::cout << errMsg.str() << std::endl;
        return ExpressionResult(false, -1, errMsg.str());
    }


    auto result = ParseSourceExpression(expr, mp);
    // note: result has a bool() operator
    if (result) {
        //std::cout << "parsing " << expr << ": " << result.result() << std::endl;
        ClearMessageSpec(this);
        for (auto &packet : result.result().spec) {
            attachMessageSpec(out, std::get<0>(packet).values, std::get<1>(packet));
        }
    }

    return ExpressionResult(result, result.position(), result.error());
}

const bitpowder::lib::String XMASFunction::getFunctionExpression(bitpowder::lib::MemoryPool &mp) {
    return Export(this, mp).stl();
}

ExpressionResult XMASFunction::setFunctionExpression(std::string &str_expr, bitpowder::lib::MemoryPool &mp) {
    bitpowder::lib::String expr(str_expr);
    return setFunctionExpression(expr, mp);
}

ExpressionResult XMASFunction::setFunctionExpression(bitpowder::lib::String &expr, bitpowder::lib::MemoryPool &mp) {

    // The input variable may be a temp
    expr = expr(mp);

    auto result = ParsePacketFunction (expr, mp);
    if (result) {
        //std::cout << "parsing " << expr << ": " << result.result() << std::endl;
        auto func = result.result();
        removeExtensionOfBaseType<ParsedXMASFunctionExtension>();
        this->addExtension(new ParsedXMASFunctionExtension(func));
        attachFunction(this, [func](const std::vector<SymbolicPacket> &p) {
            return (*func)(p);
        });
    } else {
        std::cerr << "[XMASFunction::setFunctionExpression] parsing " << expr << " containing an error." << std::endl;
        std::cerr << "[XMASFunction::setFunctionExpression] error parsing at position " << result.position() << " is " << result.error() << std::endl;
    }

    return ExpressionResult(result, result.position(), result.error());

}

const bitpowder::lib::String XMASSwitch::getSwitchExpression(bitpowder::lib::MemoryPool &mp) {
    return Export(this, mp).stl();
}

ExpressionResult XMASSwitch::setSwitchExpression(std::string &str_expr, bitpowder::lib::MemoryPool &mp) {
    bitpowder::lib::String expr(str_expr);
    return setSwitchExpression(expr, mp);
}

ExpressionResult XMASSwitch::setSwitchExpression(bitpowder::lib::String &expr, bitpowder::lib::MemoryPool &mp) {

    // The input variable may be a temp
    expr = expr(mp);

    auto result = ParsePacketExpression(expr, mp);
    if (result) {
        //std::cout << "parsing " << expr << ": " << result.result() << std::endl;
        removeExtensionOfBaseType<SymbolicSwitchingFunctionExtension>();
        for (auto &packet : result.result().values) {
            attachSwitchingFunction(this, packet);
        }
    } else {
        std::cerr << "[XMASSwitch::setFunctionExpression] parsing " << expr << " containing an error." << std::endl;
        std::cerr << "[XMASSwitch::setFunctionExpression] error parsing at position " << result.position() << " is " << result.error() << std::endl;
    }

    return ExpressionResult(result, result.position(), result.error());

}

const bitpowder::lib::String XMASJoin::getJoinExpression(bitpowder::lib::MemoryPool &mp) {
    bitpowder::lib::unused(mp);
    bitpowder::lib::String port;

    ParsedXMASRestrictedJoin* restricted = this->getComponentExtension<ParsedXMASRestrictedJoin>(false);
    if (restricted) {
        std::string port_str = std::to_string(restricted->function);
        port = bitpowder::lib::String(port_str.c_str());
    }
    return std::move(port);
    //return Export(this, mp).stl();  ---> does not exist yet
}

ExpressionResult XMASJoin::setRestrictedJoinPort(std::string &str_expr, bitpowder::lib::MemoryPool &mp) {
    bitpowder::lib::String expr(str_expr);
    return setRestrictedJoinPort(expr, mp);
}

ExpressionResult XMASJoin::setUnrestrictedJoinExpression(std::string &str_expr, bitpowder::lib::MemoryPool &mp) {
    bitpowder::lib::String expr(str_expr);
    return setUnrestrictedJoinExpression(expr, mp);
}

ExpressionResult XMASJoin::setRestrictedJoinPort(bitpowder::lib::String &expr, bitpowder::lib::MemoryPool &mp) {

    bitpowder::lib::String errMsg;

    // The input variable may be a temp
    expr = expr(mp);

    int port;
    try {
        port = std::stoi(expr.stl());
    } catch (std::invalid_argument) {
        errMsg = "[XMASJoin::setRestrictedJoinPort] Invalid argument.";
        errMsg = errMsg(mp);
        return ExpressionResult(false, 0, errMsg);
    } catch (std::out_of_range) {
        errMsg = "[XMASJoin::setRestrictedJoinPort] Argument out of range.";
        errMsg = errMsg(mp);
        return ExpressionResult(false, 0, errMsg);
    }

    removeExtensionOfBaseType<ParsedXMASRestrictedJoin>();
    auto restrictedJoin = new ParsedXMASRestrictedJoin(port);
    this->addExtension(restrictedJoin);

    return ExpressionResult(true, 0, "");

}

ExpressionResult XMASJoin::setUnrestrictedJoinExpression(bitpowder::lib::String &expr, bitpowder::lib::MemoryPool &mp) {

    // FIXME: parser only knows restricted join with function (=portnr). Unrestricted has no function.
    auto result = ParsePacketExpression(expr, mp);
    if (result) {
        //std::cout << "parsing " << expr << ": " << result.result() << std::endl;
        //removeExtensionOfBaseType<SymbolicJoinFunctionExtension>(); // does not exist yet
        for (auto &packet : result.result().values) {
            bitpowder::lib::unused(packet);
            //attachjoinFunction(this, packet); // does not exist yet.
        }
    } else {
        std::cerr << "[XMASJoin::setFunctionExpression] parsing " << expr << " containing an error." << std::endl;
        std::cerr << "[XMASJoin::setFunctionExpression] error parsing at position " << result.position() << " is " << result.error() << std::endl;
    }

    return ExpressionResult(result, result.position(), result.error());

}

