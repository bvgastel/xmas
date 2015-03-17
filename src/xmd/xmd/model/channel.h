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

class XPort;
class Component;

class Channel : public QQuickItem
{
    Q_OBJECT

    Q_PROPERTY(Component firstComponent READ getFirstComponent WRITE setFirstComponent NOTIFY firstComponentChanged)
    Q_PROPERTY(Component secondComponent READ getSecondComponent WRITE setSecondComponent NOTIFY secondComponentChanged)
    Q_PROPERTY(XPort firstPort READ getFirstPort WRITE setFirstPort NOTIFY firstPortChanged)
    Q_PROPERTY(XPort secondPort READ getSecondPort WRITE setSecondPort NOTIFY secondPortChanged)

signals:

    void firstComponentChanged();
    void secondComponentChanged();
    void firstPortChanged();
    void secondPortChanged();

public slots:


public:
    explicit Channel(QQuickItem *parent = 0);
    ~Channel();

    virtual void componentComplete();

    Component * getFirstComponent(){return m_component1;}
    Component * getSecondComponent(){return m_component2;}
    XPort * getFirstPort(){return m_port1;}
    XPort * getSecondPort(){return m_port2;}

    void setFirstComponent(Component firstComponent) {
        m_component1 = &firstComponent;
        emit firstComponentChanged();
    }
    void setSecondComponent(Component secondComponent){
        m_component2 = &secondComponent;
        emit secondComponentChanged();
    }
    void setFirstPort(XPort firstPort){
        m_port1 = &firstPort;
        emit firstPortChanged();
    }
    void setSecondPort(XPort secondPort){
        m_port2 = &secondPort;
        emit secondPortChanged();
    }

private:

    XPort *m_port1;
    XPort *m_port2;
    Component *m_component1;
    Component *m_component2;
};

} // namespace model

#endif // CHANNEL_H
