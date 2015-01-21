/*********************************************************************
  *
  * Copyright (C) Guus Bonnema, 2014
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

#ifndef CHIPCOMPONENT_H
#define CHIPCOMPONENT_H

#include <QObject>
#include <QPoint>
#include <QQmlListProperty>

#include "port.h"

// TODO: are we using todo plugin? Yes: see Help / About plugins ...

namespace model {

/**
 * @brief The Component class
 *
 * The component on a network. It contains ports (both
 * in ports and out ports.
 *
 * Ports are tightly coupled with Components: no existance outside components.
 *
 */
class  ChipComponent : public QObject
{
    Q_OBJECT

    Q_ENUMS(Orientation)
    Q_PROPERTY(QString name READ name WRITE name NOTIFY nameChanged)
    Q_PROPERTY(QString network READ network WRITE network NOTIFY networkChanged)
    Q_PROPERTY(int x READ x WRITE x NOTIFY xChanged)
    Q_PROPERTY(int y READ y WRITE y NOTIFY yChanged)
    Q_PROPERTY(Orientation orientation READ orientation WRITE orientation NOTIFY orientationChanged)
    Q_PROPERTY(QQmlListProperty<model::Port> connectors READ connectors NOTIFY connectorsChanged)
    Q_PROPERTY(QString func READ func WRITE func NOTIFY funcChanged)

public:
    enum Orientation { Up, Down, Left, Right };

    explicit ChipComponent(QObject *parent = 0);
    ~ChipComponent();

    QString name() const {return m_name;}
    void name(const QString &name) {m_name = name;}

    QString network() const { return m_network; }
    void network(QString &network);

    int x() { return m_x; }
    void x(int &x) { m_x = x; }

    int y() { return m_y; }
    void y(int &y) { m_y = y; }

    Orientation orientation() {return m_orientation;}
    void orientation(Orientation &orientation) {m_orientation = orientation;}

    QString func() { return m_func; }
    void func(QString &func) { m_func = func; }

    QQmlListProperty<Port> connectors();

    Port *at(const int index) { return m_portList[index]; }
    int size() { return m_portList.size(); }

signals:
    void nameChanged();
    void networkChanged();
    void xChanged();
    void yChanged();
    void orientationChanged();
    void funcChanged();
    void connectorsChanged();

public slots:

private:
    static void append_port(QQmlListProperty<Port> *list, Port *port);
    static Port *port_at(QQmlListProperty<Port> *list, int index);

    QString m_name;
    QString m_network;
    int m_x;
    int m_y;
    Orientation m_orientation;
    QString m_func;
    QList<Port *> m_portList;

public:
};

} // namespace model

#endif // CHIPCOMPONENT_H
