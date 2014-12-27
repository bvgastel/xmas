#ifndef OUTPORT_H
#define OUTPORT_H

#include "port.h"

class OutPort : public Port
{
public:
    OutPort(QString name, std::shared_ptr<Component> comp);
    ~OutPort();
};

#endif // OUTPORT_H
