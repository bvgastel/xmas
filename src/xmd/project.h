#ifndef PROJECT_H
#define PROJECT_H

#include "network.h"

class Project
{
public:
    Project();

    std::string name() const { return m_name; }
    void setName(std::string name) { m_name = name; }

    Network& network() { return m_network; }

private:
    std::string m_name;
    Network m_network;
};

#endif // PROJECT_H
