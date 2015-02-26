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

    Logger *m_logger = new Logger("parse flat json");

    std::string filename =
            fileUrl.isLocalFile() ? fileUrl.toLocalFile().toStdString()
                                  : fileUrl.fileName().toStdString();

    m_logger->log("Opening file " + filename, QColor("black"));

    m_logger->log("Started process validate network for file '"
                  + filename);


    std::tie(componentMap, std::ignore) = parse_xmas_from_file(filename, mp);

    if (componentMap.empty()) {
        m_logger->log("[Component.cpp/fileOpen(fileUrl)] File "+ filename + " was parsed as empty. Maybe the file is invalid json input.",Qt::red);
        return false;
    }

    return true;

}
