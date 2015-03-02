#include <map>

#include "xmas.h"
#include "simplestring.h"
#include "parse.h"

#include "parseflatjsonfile.h"

ParseFlatJsonFile::ParseFlatJsonFile()
{

}

ParseFlatJsonFile::~ParseFlatJsonFile()
{

}

bool ParseFlatJsonFile::parseJsonFile (const QUrl &fileUrl,
                                       std::map<bitpowder::lib::String, XMASComponent *> &componentMap,
                                       bitpowder::lib::MemoryPool &mp) {


    std::string filename =
            fileUrl.isLocalFile() ? fileUrl.toLocalFile().toStdString()
                                  : fileUrl.fileName().toStdString();

    std::tie(componentMap, std::ignore) = parse_xmas_from_file(filename, mp);

    if (componentMap.empty()) {
        std::cout << "Empty file offered for parsing: "+fileUrl.fileName().toStdString() << std::endl;
        return false;
    }

    return true;

}
