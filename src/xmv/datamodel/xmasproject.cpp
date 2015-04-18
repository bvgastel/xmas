#include "xmasproject.h"

#include "parse.h"
#include "simplestring.h"
#include "export.h"
#include "composite-network-extension.h"
#include "canvas-network-extension.h"

#include <sys/stat.h>
#include <fcntl.h>

#include <sstream>
#include <fstream>

using namespace bitpowder::lib;

XMASProject::XMASProject()
{
    allocate_initial_project();
}

XMASProject::XMASProject(const std::string &json, std::string &name, std::string &basePath) {

    bitpowder::lib::String bjson = bitpowder::lib::String(json)(*m_mp);
    auto parseResult = ParseJSON(bjson, *m_mp);
    if (!parseResult) {
        std::stringstream errStr;
        errStr << "error in parsing json : " << parseResult.error()
               << " at " << parseResult.position() << std::endl;
        throw Exception(errStr.str());
    }

    root = loadNetwork(parseResult, name, basePath);

}

XMASProject::XMASProject(const std::string &filename)
{
    root = loadNetwork(filename);
}

XMASProject::~XMASProject()
{
    deallocate_project();
}

void XMASProject::clear() {
    deallocate_project();
    allocate_initial_project();
}

void XMASProject::allocate_initial_project() {
    std::string name = m_initial_name;
    root = new XMASNetwork(name, &m_mp);
    networks.insert(std::make_pair(name, std::shared_ptr<XMASNetwork> {root} ));
}

void XMASProject::deallocate_project() {
    networks.clear();
    root = nullptr;
    m_mp.vacuum();
    m_mp.clear();
}

XMASNetwork* XMASProject::getRootNetwork() const {
    return root;
}

XMASNetwork* XMASProject::getNetwork(const std::string name) const {
    auto it = networks.find(name);
    return (it != networks.end()) ? it->second.get() : nullptr;
}

std::map<std::string,std::shared_ptr<XMASNetwork>> XMASProject::getNetworks() const {
    return networks;
}

// If network is nullptr: use root.
void XMASProject::saveNetwork(const std::string &filename, XMASNetwork* network)
{
    MemoryPool mp;

    // convert network to jsonString
    String jsonStr = network2jsonString(mp, network);

    std::ofstream file { filename };
    file << jsonStr;

}

// if network is nullptr: use root.
bitpowder::lib::String XMASProject::network2jsonString(bitpowder::lib::MemoryPool &mp, XMASNetwork *network) {

    JSONData::Map globals = JSONData::AllocateMap(mp);

    network = network ? network : root;

    auto cne = network->getNetworkExtension<CompositeNetworkExtension>(false);
    if (cne) {
        JSONData::Map jsonComposite = JSONData::AllocateMap(mp);
        jsonComposite["alias"] = String(cne->alias);
        jsonComposite["image-name"] = String(cne->imageName);
        jsonComposite["packet"] = String(cne->packet);
        jsonComposite["boxed-image"] = cne->boxedImage ? 1 : 0;
        globals["COMPOSITE_NETWORK"] = jsonComposite;
    }

    auto canvas_ext = network->getNetworkExtension<CanvasNetworkExtension>(false);
    if (cne) {
        JSONData::Map jsonCanvas = JSONData::AllocateMap(mp);
        jsonCanvas["width"] = canvas_ext->width;
        jsonCanvas["height"] = canvas_ext->height;
        globals["CANVAS"] = jsonCanvas;
    }

    //FIXME packet_type is (string,value) list , need to parse first!
    JSONData::Map jsonPacketType = JSONData::AllocateMap(mp);
    jsonPacketType["val "] = String(network->m_packet_type);
    globals["PACKET_TYPE"] = jsonPacketType;

    auto componentSet = network->getComponentSet();
    String jsonStr = Export(componentSet, globals, mp);
    return jsonStr;
}

/**
 * @brief XMASProject::loadNetwork
 *
 * Beware. This is a recursively called network loading utility.
 * Each level of new composites leads to a new load network for
 * each composite.
 *
 * Beware: indirect recursion through loadNetwork(parseResult).
 *
 * @param filename
 * @return
 */

XMASNetwork* XMASProject::loadNetwork(const std::string& filename)
{
    size_t lastSlash = filename.find_last_of("/");
    std::string basePath = filename.substr(0, lastSlash);
    std::string name = filename.substr(lastSlash + 1);

    if (networks.count(name) > 0) {
        throw Exception("Network already loaded");
    }
    std::cout << "Loading: " << name << std::endl;

    auto jsonResult = read_json_from_file(filename, *m_mp);

    if (!jsonResult)
        throw Exception("Unable to read JSON data from file");

    return loadNetwork(jsonResult, name, basePath);

}

XMASNetwork *XMASProject::loadNetwork(bitpowder::lib::JSONParseResult &jsonResult, std::string &name, std::string &basePath) {
    auto json = jsonResult.result();

    // add the current networks key and a dummy network value to the networks map
    // this prevents infinite recursion of loadNetwork with a circular dependency
    networks.insert(std::make_pair(name, std::shared_ptr<XMASNetwork> {new XMASNetwork {"dummy", &m_mp}} ));

    // load the network
    auto componentsAndGlobals = generate_xmas_from_parse_result(jsonResult, m_mp, [this, basePath](std::string name) -> XMASNetwork* {
        auto network_it = networks.find(name);
        if (network_it != networks.end()) {
            return network_it->second.get();
        }
        // Network was not retrieved yet, retrieve it now
        std::string compositeFilename = basePath + '/' + name;
        return loadNetwork(compositeFilename);
    });
    auto components = componentsAndGlobals.first;

    XMASNetwork* result = new XMASNetwork {name, std::move(components), &m_mp};
    networks.erase(name);
    networks.insert(std::make_pair(name, std::shared_ptr<XMASNetwork> {result}));

    auto jsonPacketType = json["PACKET_TYPE"];
    if (!jsonPacketType.isNull()) {
        auto jsonValue = jsonPacketType["val "];
        if (jsonValue.isString()) {
            result->packetType(jsonValue.asString().stl());
        } else if (jsonValue.isNumber()) {
            result->packetType(std::to_string(jsonValue.asNumber()));
        }
    }

    auto jsonComposite = json["COMPOSITE_NETWORK"];
    if (!jsonComposite.isNull()) {
        auto cn_ext = result->getNetworkExtension<CompositeNetworkExtension>(true);
        cn_ext->alias      = jsonComposite["alias"].asString().stl();
        cn_ext->imageName  = jsonComposite["image-name"].asString().stl();
        cn_ext->packet     = jsonComposite["packet"].asString().stl();
        cn_ext->boxedImage = jsonComposite["boxed-image"].asNumber() > 0;
    }

    auto jsonCanvas = json["CANVAS"];
    if (!jsonCanvas.isNull()) {
        auto canvas_ext = result->getNetworkExtension<CanvasNetworkExtension>(false);
        canvas_ext->width   = jsonCanvas["width"].asNumber();
        canvas_ext->height  = jsonCanvas["height"].asNumber();
    }

    return result;
}

bool XMASProject::unloadNetwork(std::string name)
{
    auto toUnload_it = networks.find(name);
    if (toUnload_it == networks.end())
        return false;                       // network not found

    XMASNetwork* toUnload = toUnload_it->second.get();

    // check if the network is still used by a composite
    auto checkComposites = [toUnload](const XMASNetwork* n) -> bool {
        auto composites = n->componentsOfType<XMASComposite>();
        for (auto x : composites) {
            if (&x->getNetwork() == toUnload)
                return false;               // network still in use
        }
        return true;
    };

    // check all networks for composites referring to the network to unload
    for (auto x : networks) {
        XMASNetwork* n = x.second.get();
        if (n != toUnload) {                // don't need to check toUnload itself
            if (!checkComposites(n))
                return false;
        }
    }

    // ok, unload the network
    networks.erase(toUnload_it);

    return true;
}


bool XMASProject::changeComponentName(std::string oldName, std::string newName)
{
    /* ensure memory is permanent enough */
    bitpowder::lib::String b_oldName = bitpowder::lib::String(oldName);
    b_oldName = b_oldName(*m_mp);
    bitpowder::lib::String b_newName = bitpowder::lib::String(newName);
    b_newName = b_newName(*m_mp);

    /* change the name and position in the component map */
    bool result = root->changeComponentName(b_oldName, b_newName);
    return result;
}

bool XMASProject::removeComponent(std::string name)
{
    bitpowder::lib::String bname = bitpowder::lib::String(name)(mp());
    bool result = root->removeComponent(bname);
    return result;
}

XMASComponent *XMASProject::insertSource(const bitpowder::lib::String &name, bool external) {
    return insert<XMASSource>(name, external);
}

XMASComponent *XMASProject::insertSink(const bitpowder::lib::String &name, bool external) {
    return insert<XMASSink>(name, external);
}

XMASComponent *XMASProject::insertFunction(const bitpowder::lib::String &name) {
    return insert<XMASFunction>(name);
}

XMASComponent *XMASProject::insertQueue(const bitpowder::lib::String &name, size_t capacity) {
    return insert<XMASQueue>(name, capacity);
}

XMASComponent *XMASProject::insertJoin(const bitpowder::lib::String &name) {
    return insert<XMASJoin>(name);
}

XMASComponent *XMASProject::insertMerge(const bitpowder::lib::String &name) {
    return insert<XMASMerge>(name);
}

XMASComponent *XMASProject::insertSwitch(const bitpowder::lib::String &name) {
    return insert<XMASSwitch>(name);
}

XMASComponent *XMASProject::insertFork(const bitpowder::lib::String &name) {
    return insert<XMASFork>(name);
}

XMASComponent *XMASProject::insertComposite(const bitpowder::lib::String &name, std::string filename) {
    XMASNetwork* network = getNetwork(filename);
    if(network){
        return root->insert<XMASComposite>(name,std::ref(*network));
    }
    return nullptr;
}

