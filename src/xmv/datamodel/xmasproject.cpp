#include "xmasproject.h"

#include "parser_json.h"
#include "parse.h"
#include "simplestring.h"
#include "export.h"
#include "composite-network-extension.h"

#include <sys/stat.h>
#include <fcntl.h>

#include <sstream>
#include <fstream>

using namespace bitpowder::lib;

JSONParseResult read_json_from_file(const std::string &filename, MemoryPool &mp);

XMASProject::XMASProject()
{
    allocate_initial_project();
}

XMASProject::XMASProject(const std::string& filename)
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
    networks.insert(std::make_pair(name, std::unique_ptr<XMASNetwork>(root)));
}

void XMASProject::deallocate_project() {
    m_mp.clear();
    if (root) {
        delete root;
    }
    networks.clear();
}

bitpowder::lib::MemoryPool& XMASProject::mp() {
    return m_mp;
}

XMASNetwork* XMASProject::getRootNetwork() const {
    return root;
}

XMASNetwork* XMASProject::getNetwork(const std::string name) const {
    return networks.find(name)->second.get();
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
        jsonComposite["width"] = cne->width;
        jsonComposite["height"] = cne->height;
        jsonComposite["image-name"] = String(cne->imageName);
        jsonComposite["packet"] = String(cne->packet);
        jsonComposite["boxed-imaged"] = cne->boxedImage ? 1 : 0;
        globals["COMPOSITE_NETWORK"] = jsonComposite;
    }

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
    return root->insert<XMASComposite>(m_mp, name, std::ref(network));
}

bool XMASProject::changeComponentName(std::string oldName, std::string newName)
{
    /* ensure memory is permanent enough */
    bitpowder::lib::String b_oldName = bitpowder::lib::String(oldName);
    b_oldName = b_oldName(m_mp);
    bitpowder::lib::String b_newName = bitpowder::lib::String(newName);
    b_newName = b_newName(m_mp);

    /* change the name and position in the component map */
    bool result = root->changeComponentName(b_oldName, b_newName);
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

    auto jsonResult = read_json_from_file(filename, m_mp);

    if (!jsonResult)
        throw Exception("Unable to read JSON data from file");

    auto json = jsonResult.result();

    // add the current networks key and a dummy network value to the networks map
    // this prevents infinite recursion of loadNetwork with a circular dependency
    XMASNetwork* dummy = new XMASNetwork {"dummy"};
    networks.insert(std::make_pair(name, std::unique_ptr<XMASNetwork>(dummy)));

    // load all composite networks used by this network
    for (auto &jsonComponent : json["COMPOSITE_OBJECTS"]) {
        String compositeName = jsonComponent.asString();
        std::cout << "Depends on: " << compositeName << std::endl;

        // load the composite network, if not already loaded
        if (networks.count(compositeName.stl()) == 0) {
            std::string compositeFilename = basePath + '/' + compositeName.stl();
            loadNetwork(compositeFilename);
        }
    }

    // now load the network itself
    auto componentsAndGlobals = generate_xmas_from_parse_result(jsonResult, m_mp, networks);
    auto components = componentsAndGlobals.first;

    XMASNetwork* result = new XMASNetwork {name, std::move(components)};
    networks.erase(name);
    networks.insert(std::make_pair(name, std::unique_ptr<XMASNetwork>(result)));

    auto jsonComposite = json["COMPOSITE_NETWORK"];
    if (!jsonComposite.isNull()) {
        auto cn_ext = result->getNetworkExtension<CompositeNetworkExtension>(true);
        cn_ext->alias      = jsonComposite["alias"].asString().stl();
        cn_ext->width      = jsonComposite["width"].asNumber();
        cn_ext->height     = jsonComposite["height"].asNumber();
        cn_ext->imageName  = jsonComposite["image-name"].asString().stl();
        cn_ext->packet     = jsonComposite["packet"].asString().stl();
        cn_ext->boxedImage = jsonComposite["boxed-image"].asNumber() > 0;
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
