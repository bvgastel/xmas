#include "port.h"

Port::Port(QString name, std::shared_ptr<Component> comp)
    : m_name(name), m_comp(comp)
{

}

Port::~Port()
{

}

const QString Port::name() const {
    return m_name;
}

