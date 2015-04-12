#include <string>
#include <sstream>
#include <cstring>
#include <strings.h>

#include "xmas.h"
#include "canvascomponentextension.h"
#include "composite-network-extension.h"
#include "canvas-network-extension.h"
#include "simplestring.h"
#include "parse.h"
#include "messagespec.h"
#include "memorypool.h"
#include "export.h"
#include "symbolic.h"

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
 * @brief disconnect disconnect a channel starting from an output
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

/**
 * @brief disconnect disconnect a channel starting from an input
 *
 * Provided a channel is valid i.e. o.valid(), both pointers
 * from output to input port and back are nullified.
 *
 * This function is necessary for the graphical designer, where
 * a designing person may delete a channel.
 *
 * @param i the input port of the channel.
 */
void disconnect(Input &i) {
    if (i.valid()) {
        Output *o = i.input;
        o->output = nullptr;
        i.input = nullptr;
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
    clearExtensions();          // FIXME: why was this call commented out?
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


XMASSink::~XMASSink() {
    for (Input* i : inputPorts()) {
        if (i->isConnected()) {
            disconnect(*i);
        }
    }
    for (Output *o : outputPorts()) {
        if (o->isConnected()) {
            disconnect(*o);
        }
    }
}


XMASSource::~XMASSource() {
    for (Input* i : inputPorts()) {
        if (i->isConnected()) {
            disconnect(*i);
        }
    }
    for (Output *o : outputPorts()) {
        if (o->isConnected()) {
            disconnect(*o);
        }
    }
}


XMASQueue::~XMASQueue() {
    for (Input* i : inputPorts()) {
        if (i->isConnected()) {
            disconnect(*i);
        }
    }
    for (Output *o : outputPorts()) {
        if (o->isConnected()) {
            disconnect(*o);
        }
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

XMASFunction::~XMASFunction() {
    for (Input* i : inputPorts()) {
        if (i->isConnected()) {
            disconnect(*i);
        }
    }
    for (Output *o : outputPorts()) {
        if (o->isConnected()) {
            disconnect(*o);
        }
    }
}

const std::string XMASFunction::getFunctionExpression(bitpowder::lib::MemoryPool &mp) {
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
        removeExtensionOfBaseType<ParsedXMASFunctionExtension *>();
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

XMASSwitch::~XMASSwitch() {
    for (Input* i : inputPorts()) {
        if (i->isConnected()) {
            disconnect(*i);
        }
    }
    for (Output *o : outputPorts()) {
        if (o->isConnected()) {
            disconnect(*o);
        }
    }
}

const bitpowder::lib::String XMASSwitch::getSwitchExpression(bitpowder::lib::MemoryPool &mp) {
    bitpowder::lib::String expression = Export(this, mp);
    expression = expression(mp);
    return std::move(expression);
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
        removeExtensionOfBaseType<SymbolicSwitchingFunctionExtension *>();
        for (auto &packet : result.result().values) {
            attachSwitchingFunction(this, packet);
        }
    } else {
        std::cerr << "[XMASSwitch::setFunctionExpression] parsing " << expr << " containing an error." << std::endl;
        std::cerr << "[XMASSwitch::setFunctionExpression] error parsing at position " << result.position() << " is " << result.error() << std::endl;
    }

    return ExpressionResult(result, result.position(), result.error());

}

XMASFork::~XMASFork() {
    for (Input* i : inputPorts()) {
        if (i->isConnected()) {
            disconnect(*i);
        }
    }
    for (Output *o : outputPorts()) {
        if (o->isConnected()) {
            disconnect(*o);
        }
    }
}

XMASMerge::~XMASMerge() {
    for (Input* i : inputPorts()) {
        if (i->isConnected()) {
            disconnect(*i);
        }
    }
    for (Output *o : outputPorts()) {
        if (o->isConnected()) {
            disconnect(*o);
        }
    }
}

XMASJoin::~XMASJoin() {
    for (Input* i : inputPorts()) {
        if (i->isConnected()) {
            disconnect(*i);
        }
    }
    for (Output *o : outputPorts()) {
        if (o->isConnected()) {
            disconnect(*o);
        }
    }
}

const bitpowder::lib::String XMASJoin::getJoinExpression(bitpowder::lib::MemoryPool &mp) {
    bitpowder::lib::String port;

    ParsedXMASRestrictedJoin* restricted = this->getComponentExtension<ParsedXMASRestrictedJoin>(false);
    if (restricted) {
        std::string port_str = std::to_string(restricted->function);
        port = bitpowder::lib::String(port_str.c_str());
        port = port(mp);
    }
    return std::move(port);
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

    removeExtensionOfBaseType<ParsedXMASRestrictedJoin *>();
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

XMASNetwork::XMASNetwork(std::string name, bitpowder::lib::MemoryPool *mp)
    : name(name),
      m_packet_type(),
      m_mp(mp ? mp
              : new bitpowder::lib::MemoryPool()),
      m_mp_self_created(mp ? false : true)
{
    setCompositeNetworkData("",1000,500,"",true);
}

XMASNetwork::XMASNetwork(std::string name, std::map<bitpowder::lib::String, XMASComponent*>&& components, bitpowder::lib::MemoryPool *mp)
    : name(name),
      m_packet_type(),
      components(components),
      m_mp(mp ? mp
              : new bitpowder::lib::MemoryPool()),
      m_mp_self_created(mp ? false : true)
{
    setCompositeNetworkData("",1000,500,"",true);
}

XMASNetwork::XMASNetwork(XMASNetwork &&tempNetwork)
    : ExtensionContainer<XMASNetworkExtension>(std::move(tempNetwork)),
      name(std::move(tempNetwork.name)),
      m_packet_type(std::move(tempNetwork.m_packet_type)),
      components(std::move(tempNetwork.components)),
      m_mp(tempNetwork.m_mp),
      m_mp_self_created(tempNetwork.m_mp_self_created)
{
    tempNetwork.m_mp = nullptr;
    tempNetwork.m_mp_self_created = false;
}

XMASNetwork::~XMASNetwork()
{
    clearExtensions();

    if (m_mp_self_created) {
        delete m_mp;
    }
}

void XMASNetwork::clearExtensions() {
    auto extensions = ExtensionContainer<XMASNetworkExtension>::clearExtensions();
    for (auto it = extensions.begin(); it != extensions.end(); )
        delete it.erase();
}


const std::string XMASNetwork::getStdName() const {
    return this->name;
}

const std::string XMASNetwork::packetType() const {
    return m_packet_type;
}

void XMASNetwork::packetType(const std::string packet_type) {
    m_packet_type = packet_type;
}



const std::map<bitpowder::lib::String, XMASComponent*> &XMASNetwork::getComponentMap() const {
    return components;
}

std::set<XMASComponent *> XMASNetwork::getComponentSet() const {
    std::set<XMASComponent*> xset;

    auto xmap = getComponentMap();
    for (auto entry : xmap) {
        XMASComponent *c = entry.second;
        xset.insert(c);
    }

    return xset;
}

XMASComponent *XMASNetwork::getComponent(std::string name) {
    if (components.count(name)) {
        auto c = components.at(name);
        return c;
    }
    return nullptr;
}

void XMASNetwork::setCompositeNetworkData(std::string alias, int width, int height, std::string imageName, bool boxedImage)
{
    auto cn_ext = getNetworkExtension<CompositeNetworkExtension>(true);
    cn_ext->alias = alias;
    cn_ext->imageName = imageName;
    cn_ext->boxedImage = boxedImage;
    // FIXME: what about packet??

    auto canvas_ext = getNetworkExtension<CanvasNetworkExtension>(true);
    canvas_ext->width = width;
    canvas_ext->height = height;
}

// FIXME: needs implementation
bitpowder::lib::String XMASNetwork::getPacketType() {
    return "";
}

// FIXME: needs implementation
bitpowder::lib::String XMASNetwork::getVars() {
    return "yet to be defined";
}

/**
  * @brief XMASNetwork::changeComponentName
  *
  * Only use this function of newName is on a permanent memory location.
  * Because the way bitpowder::lib::String works is it copies the pointer
  * to the string, not the string itself. Beware!
  *
  * Preferrably use the same function from project. The project function copies the memory
  * to a memorypool it has, before calling this function.
  *
  * @param oldName
  * @param newName
  * @return
  */
bool XMASNetwork::changeComponentName(bitpowder::lib::String oldName, bitpowder::lib::String &newName)
{
    auto it_old = components.find(oldName);
    auto it_new = components.find(newName);
    if (it_old != components.end() && it_new == components.end()) {
        XMASComponent *c = components.at(oldName);
        components.erase(oldName);
        c->name(newName.stl());
        bool success = false;

        std::tie(std::ignore, success) = components.insert(std::make_pair(c->getName(), c));
        return success;
    }
    return false;
}

bool XMASNetwork::removeComponent(bitpowder::lib::String name)
{
    auto c = getComponent(name.stl());
    if (c) {
        for (Input* i : c->inputPorts()) {
            if (i->isConnected()) {
                disconnect(*i);
            }
        }
        for (Output *o : c->outputPorts()) {
            if (o->isConnected()) {
                disconnect(*o);
            }
        }
        auto count = components.erase(name);
        if (count == 1) {
            return true;
        }
    }
    return false;
}


XMASComposite::XMASComposite(const bitpowder::lib::String &name, XMASNetwork &network)
    : XMASComponent(name), network(network) {
    // get in & out gates from network
    auto inGates = network.componentsOfType<XMASSource>();
    auto outGates = network.componentsOfType<XMASSink>();

    int numInGates = inGates.size();
    int numOutGates = outGates.size();

    inputs.reserve(numInGates);
    outputs.reserve(numOutGates);

    // for all in gates, create an input port
    for (XMASSource *c : inGates) {
        inputs.push_back(Input {this, c->getStdName().c_str()} );
    }

    // for all out gates, create an output port
    for (XMASSink *c : outGates) {
        outputs.push_back(Output {this, c->getStdName().c_str()} );
    }

    // fill p
    p.reserve(inputs.size() + outputs.size());

    for (auto& i : inputs)
        p.push_back(&i);
    for (auto& o : outputs)
        p.push_back(&o);
}

XMASComposite::~XMASComposite() {
    for (Input* i : inputPorts()) {
        if (i->isConnected()) {
            disconnect(*i);
        }
    }
    for (Output *o : outputPorts()) {
        if (o->isConnected()) {
            disconnect(*o);
        }
    }
}


