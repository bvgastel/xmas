
#include <string>
#include "lib/memorypool.h"
#include "parse.h"
#include "export.h"

using namespace bitpowder::lib;

void ConvertFile(const std::string &filename) {
    MemoryPool mp;

    auto parse = Parse(filename, mp);
    auto& components = parse.first;
    auto& globals = parse.second;

    std::set<XMASComponent*> allComponents;
    for (auto &it : components) {
        checkAssert(it.second->valid());
        if (it.second)
            allComponents.insert(it.second);
    }

    Export(allComponents, globals);
}

int main(int argc, char* argv[]) {
    if (argc >= 2) {
        ConvertFile(argv[1]);
        return 0;
    }
    return 1;
}
