#ifndef XMASPROJECT_H
#define XMASPROJECT_H

#include "string.h"
#include "xmas.h"

#include <boost/filesystem.hpp>

class XMASProject
{
public:
    XMASProject(const std::string& filename);
    ~XMASProject();

private:
    bitpowder::lib::MemoryPool mp;
    std::map<std::string, XMASNetwork>  networks;

    void loadNetwork(const boost::filesystem::path& filePath);
};

#endif // XMASPROJECT_H
