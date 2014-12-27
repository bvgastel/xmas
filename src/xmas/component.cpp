#include "component.h"


Component::Component(QString name) : m_name(name)
{
}

const QString Component::name() const {
    return m_name;
}
