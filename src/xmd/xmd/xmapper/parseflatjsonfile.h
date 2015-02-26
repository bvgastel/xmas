#ifndef PARSEFLATJSONFILE_H
#define PARSEFLATJSONFILE_H

#include <QUrl>

#include "memorypool.h"
#include "xmas.h"
#include "logger.h"

class ParseFlatJsonFile
{
public:
    ParseFlatJsonFile();
    ~ParseFlatJsonFile();

    static bool parseJsonFile(const QUrl &fileUrl,
                              std::map<bitpowder::lib::String, XMASComponent *> &componentMap,
                              bitpowder::lib::MemoryPool &mp
                              );

};

#endif // PARSEFLATJSONFILE_H
