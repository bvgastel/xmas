#include "xmasproject.h"

#include "parser_json.h"
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

JSONParseResult read_json_from_file(const std::string &filename, MemoryPool &mp);

XMASProject::XMASProject()
  : m_mp {new bitpowder::lib::MemoryPool}
{
    allocate_initial_project();
}

XMASProject::XMASProject(const std::string& filename)
    : m_mp {new bitpowder::lib::MemoryPool}
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
    std::string name = "?.xmas";
    root = new XMASNetwork {name};
    networks.insert(std::make_pair(name, root));
}

void XMASProject::deallocate_project() {
    networks.clear();
    delete root;
    m_mp->clear();
}


XMASNetwork* XMASProject::getRootNetwork() const {
    return root;
}

XMASNetwork* XMASProject::getNetwork(const std::string name) const {
    auto it = networks.find(name);
    return (it != networks.end()) ? it->second : nullptr;
}

void XMASProject::saveNetwork(const std::string &filename, XMASNetwork* network)
{
    MemoryPool mp;
    std::set<XMASComponent*> allComponents;

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

    JSONData::Map jsonPacketType = JSONData::AllocateMap(mp);
    jsonPacketType["val "] = bitpowder::lib::String(network->packetType());
    globals["PACKET_TYPE"] = JSONData::AllocateMap(mp);


    for (auto& it : network->getComponentMap()) {
        allComponents.insert(it.second);
    }

    String jsonStr = Export(allComponents, globals, mp);

    std::ofstream file { filename };
    file << jsonStr;

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

XMASComponent *XMASProject::insertComposite(const bitpowder::lib::String &name, XMASNetwork &network) {
    return root->insert<XMASComposite>(name, std::ref(network));
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

/**
 * @brief XMASProject::loadNetwork
 *
 * Beware. This is a recursively called network loading utility.
 * Each level of new composites leads to a new load network for
 * each composite.
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

    auto json = jsonResult.result();

    // add the current networks key and a dummy network value to the networks map
    // this prevents infinite recursion of loadNetwork with a circular dependency
    XMASNetwork* dummy = new XMASNetwork {"dummy"};
    networks.insert(std::make_pair(name, dummy));

    // gbo: unnecessary to store COMPOSITE OBJECTS: better retrieve dynamically
    // load all composite networks used by this network
//    for (auto &jsonComponent : json["COMPOSITE_OBJECTS"]) {
//        String compositeName = jsonComponent.asString();
//        std::cout << "Depends on: " << compositeName << std::endl;

//        // load the composite network, if not already loaded
//        if (networks.count(compositeName.stl()) == 0) {
//            std::string compositeFilename = basePath + '/' + compositeName.stl();
//            loadNetwork(compositeFilename);
//        }
//    }


    // now load the network itself
    auto componentsAndGlobals = generate_xmas_from_parse_result(jsonResult, *m_mp, [this, basePath](std::string name) -> XMASNetwork* {
            auto network_it = networks.find(name);
            if (network_it != networks.end()) {
                return network_it->second;
            }
            // Network was not retrieved yet, retrieve it now
            std::string compositeFilename = basePath + '/' + name;
            XMASNetwork *network = loadNetwork(compositeFilename);
            if (network) {
                networks.insert(std::make_pair(name, network));
            }
            return network;
            });
    auto components = componentsAndGlobals.first;

    XMASNetwork* result = new XMASNetwork {name, std::move(components), m_mp};
    networks.erase(name);
    delete dummy;
    networks.insert(std::make_pair(name, result));

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


JSONParseResult read_json_from_file(const std::string &filename, MemoryPool &mp)
{
    struct stat st;
    if (stat(filename.c_str(), &st)) {
        throw Exception("Unable to read file: " + filename);
    }
#ifdef __MINGW32__
    int fd = open(filename.c_str(), O_RDONLY | O_BINARY);
#else
    int fd = open(filename.c_str(), O_RDONLY);
#endif
    if (!fd) {
        throw Exception("Unable to read file: " + filename);
    }
    char *buffer = (char*)mp.alloc(st.st_size);
    int size = read(fd, buffer, st.st_size);
    if (size != st.st_size) {
        std::stringstream errStr;
        errStr << "wrong number of bytes read: " << size << " instead of " << st.st_size;
        throw Exception(errStr.str());
    }
    close(fd);

    String current(buffer, size);
    auto parseResult = ParseJSON(current, mp);
    if (!parseResult) {
        std::stringstream errStr;
        errStr << "error in parsing json file " << filename << ": " << parseResult.error() << " at " << parseResult.position() << std::endl;
        throw Exception(errStr.str());
    }
    return parseResult;
}
