#include "outport.h"

OutPort::OutPort(QString name, std::shared_ptr<Component> comp) : Port(name, comp)
{

}

OutPort::~OutPort()
{

}

