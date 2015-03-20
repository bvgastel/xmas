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
    Q_PROPERTY(bool connected READ getConnected NOTIFY connectedChanged)

//    property int id: 0 (komt van toepassing in composites , uniek volgnummer ipv naam)

signals:
    void nameChanged();
    void typeChanged();
    void connectedChanged();

    void writeLog(QString message, QColor color = Qt::blue);

public slots:

public:
    explicit XPort(QQuickItem *parent = 0);
    ~XPort();
    enum PortType {INPORT, OUTPORT};


    virtual void classBegin();
    virtual void componentComplete();

    PortType getType() const {
        return m_type;
    }

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
        Port *port = this->getPort();
        if (port) {
            return port->isConnected();
        }
        return false;
    }

    /**
     * @brief getPort
     *
     * Returns the companion xmas port.
     *
     * @return Port with the same name.
     */
    Port *getPort();

private:
    QString m_name;
    PortType m_type;

    Component *m_component;

};

} // namespace model

#endif // XPORT_H
