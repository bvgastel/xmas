#ifndef NETWORK_H
#define NETWORK_H

#include <sstream>
#include <map>

#include <QUrl>

#include "controller.h"
#include "simplestring.h"
#include "xmas.h"

class Network
{
public:
    explicit Network(Controller *controller, QUrl fileUrl);
    explicit Network(Controller *controller, std::stringstream &sstr);
    ~Network();

    size_t size();
private:
    Controller *m_controller;
    std::map<bitpowder::lib::String, XMASComponent *> m_componentMap;
};

#endif // NETWORK_H
