#include "xmasproject.h"

#include "parser_json.h"
#include "parse.h"
#include "simplestring.h"

#include <sys/stat.h>
#include <fcntl.h>

#include <sstream>

using namespace bitpowder::lib;

JSONParseResult read_json_from_file(const std::string &filename, MemoryPool &mp);

XMASProject::XMASProject(const std::string& filename)
{
    root = loadNetwork(filename);
}

XMASProject::~XMASProject()
{

}

XMASNetwork* XMASProject::loadNetwork(const std::string& filename)
{
    size_t lastSlash = filename.find_last_of("/");
    std::string basePath = filename.substr(0, lastSlash);
    std::string name = filename.substr(lastSlash + 1);

    std::cout << "Loading: " << name << std::endl;

    auto jsonResult = read_json_from_file(filename, mp);

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
    auto componentsAndGlobals = generate_xmas_from_parse_result(jsonResult, mp, networks);
    auto components = componentsAndGlobals.first;

    XMASNetwork* result = new XMASNetwork {name, std::move(components)};
    networks.erase(name);
    networks.insert(std::make_pair(name, std::unique_ptr<XMASNetwork>(result)));

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
