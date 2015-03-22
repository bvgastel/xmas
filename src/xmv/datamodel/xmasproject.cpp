#include "xmasproject.h"

#include "parser_json.h"
#include "parse.h"
#include "simplestring.h"

#include <sys/stat.h>
#include <fcntl.h>

#include <sstream>

#include <boost/filesystem.hpp>

using namespace bitpowder::lib;

JSONParseResult read_json_from_file(const std::string &filename, MemoryPool &mp);

XMASProject::XMASProject(const std::string& filename)
{
    boost::filesystem::path path { filename };
    root = loadNetwork(path);
}

XMASProject::~XMASProject()
{

}

XMASNetwork* XMASProject::loadNetwork(const boost::filesystem::path& filePath)
{
    const std::string filename = filePath.filename().string();

    boost::filesystem::path basePath { filePath };
    basePath.remove_filename();

    auto jsonResult = read_json_from_file(filePath.string(), mp);

    if (!jsonResult)
        throw Exception("Unable to read JSON data from file");

    auto json = jsonResult.result();

    // add the current networks key and a dummy network value to the networks map
    // this prevents infinite recursion of loadNetwork with a circular dependency
    XMASNetwork* dummy = new XMASNetwork {"dummy"};
    networks.insert(std::make_pair(filename, std::unique_ptr<XMASNetwork>(dummy)));

    // load all composite networks used by this network
    for (auto &jsonComponent : json["COMPOSITE_OBJECTS"]) {
        String compositeFilename = jsonComponent.asString();
        std::cout << compositeFilename << std::endl;

        // load the composite network, if not already loaded
        if (networks.count(compositeFilename.stl()) == 0) {
            boost::filesystem::path compositePath { basePath / compositeFilename.stl() };
            std::cout << compositePath << std::endl;
            loadNetwork(compositePath);
        }
    }

    // now load the network itself
    auto componentsAndGlobals = generate_xmas_from_parse_result(jsonResult, mp, networks);
    auto components = componentsAndGlobals.first;
    networks.erase(filename);

    XMASNetwork* result = new XMASNetwork {filename, std::move(components)};
    networks.insert(std::make_pair(filename, std::unique_ptr<XMASNetwork>(result)));

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
