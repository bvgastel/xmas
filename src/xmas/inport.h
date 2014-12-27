#ifndef INPORT_H
#define INPORT_H

#include "port.h"

class InPort : public Port
{
public:
    InPort(QString name, std::shared_ptr<Component> comp);
    ~InPort();
};

#endif // INPORT_H
