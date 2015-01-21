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

#ifndef NETWORK_H
#define NETWORK_H

#include <QObject>

#include "grid.h"
#include "gridpoint.h"
#include "chipcomponent.h"
#include "channel.h"

namespace model {

/**
 * @brief The network class
 *
 * The network class (Network on Chip) contains all elements
 * that create the network:
 *
 * 1. Components
 * 2. Ports
 * 3. Channels between ports on components
 *
 * Each channel represents 3 wires; irdy, trdy and data (@see channel)
 *
 * This class allows the construction of a Noc that can
 * later be used as a component.
 *
 * If needed the designer can close a Noc with sources and sinks.
 *
 * TODO: [Network] We need a graph representation for DFS / BFS
 * Options are adjancency matrix, adj. list or adj. multilists
 * The problem is that an edge (u, v) can occur multiple times
 * with different ports on each sides i.e. port-chipcomponent combinations
 * are unique.
 *
 * This is a reminder for use by the VT plugin
 *
 */
class Network : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString name READ name WRITE name NOTIFY nameChanged)
    Q_PROPERTY(Grid grid READ grid WRITE grid NOTIFY gridChanged)
    Q_PROPERTY(QQmlListProperty<ChipComponent> components READ components NOTIFY componentsChanged)
    Q_PROPERTY(QQmlListProperty<Channel> channels READ channels NOTIFY channelsChanged)

public:
    explicit Network(QObject *parent = 0);
    ~Network();

    QString name() const { return m_name; }
    void name(QString &name) { m_name = name; }

    Grid grid() const { return m_grid; }
    void grid(Grid &grid) { m_grid = grid; }

    QQmlListProperty<ChipComponent> components() { return m_components; }

    QQmlListProperty<Channel> channels() { return m_channels; }

signals:
    void gridChanged();
    void nameChanged();
    void componentsChanged();
    void channelsChanged();

public slots:

private:
    QString m_name;
    Grid m_grid;
    QQmlListProperty<ChipComponent> m_components;
    QQmlListProperty<Channel> m_channels;
};

} // namespace model

#endif // NETWORK_H
