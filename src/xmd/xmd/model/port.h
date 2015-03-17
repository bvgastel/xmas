/*********************************************************************
  *
  * Copyright (C) <name>, <year>
  *
  * This file is part of the xmas-design tool.
  *
  * The xmas-design tool is free software: you can redistribute it
  * and/or modify it under the terms of the GNU General Public License
  * as published by the Free Software Foundation, either version 3 of
  * the License, or (at your option) any later version.
  *
  * The xmas-design tool is distributed in the hope that it will be
  * useful,  but WITHOUT ANY WARRANTY; without even the implied warranty
  * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  * GNU General Public License for more details.
  *
  * You should have received a copy of the GNU General Public License
  * along with the xmas-design tool.  If not, see
  * <http://www.gnu.org/licenses/>.
  *
  **********************************************************************/

#ifndef XPORT_H
#define XPORT_H

#include <QQuickItem>


#include "xmas.h"

namespace model {

class Component;

class XPort : public QQuickItem
{
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)
    Q_ENUMS(PortType)
    Q_PROPERTY(PortType type READ getType WRITE setType NOTIFY typeChanged)
    Q_PROPERTY(QString name READ getName WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(bool connected READ getConnected WRITE setConnected NOTIFY connectedChanged)
    Q_PROPERTY(model::Component *owner READ owner WRITE owner NOTIFY ownerChanged)

//    property int id: 0 (komt van toepassing in composites , uniek volgnummer ipv naam)

signals:
    void nameChanged();
    void typeChanged();
    void connectedChanged();
    void ownerChanged();

public slots:

public:
    explicit XPort(QQuickItem *parent = 0);
    ~XPort();
    enum PortType {INPORT, OUTPORT};


    virtual void classBegin();
    virtual void componentComplete();

    PortType getType() const {return m_type;}
    void setType(PortType type) {
        m_type = type;
        emit typeChanged();
    }

    QString getName() {
        return m_name;
    }

    void setName(QString name) {
        if (name != m_name) {
            m_name = name;
        }
        emit nameChanged();
    }

    bool getConnected() {
        return m_port->isConnected();
    }

    void setConnected(bool) {
        emit connectedChanged();  // just signal, don't change anything
    }

    Component *owner() {
        return this->m_owner;
    }

    void owner(Component *owner) {
        this->m_owner = owner;
    }

    Port *getPort() {
        return m_port;
    }


private:
    QString m_name;
    PortType m_type;
    //bool m_connected;       // useless ---> isConnected is part of Output Port.
                            // but how to implement?
    Component *m_owner;
    Port *m_port;           // Only one port is allowed: either input or output
};

} // namespace model

#endif // XPORT_H
