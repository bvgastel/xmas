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

#ifndef CHANNEL_H
#define CHANNEL_H

#include <QQuickItem>
#include "component.h"
#include "port.h"

namespace model {

class Channel : public QQuickItem
{
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)
    Q_PROPERTY(XPort *port1 READ port1 WRITE setPort1 NOTIFY port2Changed)
    Q_PROPERTY(XPort *port2 READ port2 WRITE setPort2 NOTIFY port1Changed)

signals:

    void port2Changed();
    void port1Changed();

    void writeLog(QString msg, QColor color = Qt::blue);

public slots:


public:
    explicit Channel(QQuickItem *parent = 0);
    ~Channel();

    virtual void classBegin();
    virtual void componentComplete();

    XPort *port1() {
        return m_port1;
    }

    void setPort1(XPort *port){
        if (m_port1==port) return;
        m_port1 = port;
        emit port1Changed();
    }

    XPort *port2() {
        return m_port2;
    }

    void setPort2(XPort *port){
        if (m_port2==port) return;
        m_port2 = port;
        emit port2Changed();
    }

private:
    bool portsOk(QString &errMsg);

private:

    XPort *m_port1;
    XPort *m_port2;
};
} // namespace model

#endif // CHANNEL_H
