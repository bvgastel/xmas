#ifndef XMASPROJECT_H
#define XMASPROJECT_H

#include "parser_json.h"
#include "string.h"
#include "xmas.h"

class XMASProject
{
public:
    XMASProject();
    XMASProject(const std::string& filename);
    XMASProject(const std::string &json, std::string &name, std::string &basePath);
    ~XMASProject();

    void allocate_initial_project();
    void deallocate_project();
    void clear();

    bitpowder::lib::MemoryPool& mp() { return *m_mp; }

    XMASNetwork* getRootNetwork() const;
    XMASNetwork* getNetwork(const std::string name) const;

    bitpowder::lib::String network2jsonString( bitpowder::lib::MemoryPool &mp, XMASNetwork *network = nullptr);

    XMASNetwork* loadNetwork(const std::string& filename);
    XMASNetwork *loadNetwork(bitpowder::lib::JSONParseResult &jsonResult,
                             std::string &name,
                             std::string &basePath);
    void saveNetwork(const std::string& filename, XMASNetwork* network = nullptr);

    template <class T, typename... Args>
    T *insert(const bitpowder::lib::String& name, Args... args) {
        return root->insert<T>(name, args...);
    }

    XMASComponent *insertSource(const bitpowder::lib::String& name, bool external = false);
    XMASComponent *insertSink(const bitpowder::lib::String& name, bool external = false);
    XMASComponent *insertFunction(const bitpowder::lib::String& name);
    XMASComponent *insertQueue(const bitpowder::lib::String& name, size_t capacity = 1);
    XMASComponent *insertJoin(const bitpowder::lib::String& name);
    XMASComponent *insertMerge(const bitpowder::lib::String& name);
    XMASComponent *insertSwitch(const bitpowder::lib::String& name);
    XMASComponent *insertFork(const bitpowder::lib::String& name);
    XMASComponent *insertComposite(const bitpowder::lib::String& name, std::string filename = "");

    bool changeComponentName(std::string oldName, std::string newName);

    bool removeComponent(std::string name);

private:
    const std::string m_initial_name = "?.json";
    bitpowder::lib::MemoryPool m_mp;
    std::map<std::string, XMASNetwork *>networks;
    XMASNetwork* root;

};

#endif // XMASPROJECT_H
