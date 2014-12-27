#ifndef PORT_H
#define PORT_H

#include <memory>

#include <QString>

#include "component.h"

enum PortType {
    ANY, IN, OUT
};

class Port
{
public:
    Port(QString name, std::shared_ptr<Component> comp);
    ~Port();

    const QString name() const;

private:

    QString m_name;
    std::shared_ptr<Component> m_comp;

};

#endif // PORT_H
