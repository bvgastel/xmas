#ifndef COMPONENT_H
#define COMPONENT_H

#include <QString>

#include "xmas_global.h"

class XMASSHARED_EXPORT Component
{

public:
    Component(QString name);
    virtual ~Component() {

    }

    const QString name() const;

private:

    QString m_name;


};

#endif // COMPONENT_H
