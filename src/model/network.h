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

#include "gridpoint.h"
#include "chipcomponent.h"
#include "channel.h"
#include "port.h"

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
    Q_PROPERTY(int width READ width WRITE width NOTIFY widthChanged)
    Q_PROPERTY(int height READ height WRITE height NOTIFY heightChanged)
    Q_PROPERTY(QQmlListProperty<model::ChipComponent> components READ components NOTIFY componentsChanged)
    Q_PROPERTY(QQmlListProperty<model::Channel> channels READ channels NOTIFY channelsChanged)

public:
    explicit Network(QObject *parent = 0);
    ~Network();

    QString name() const { return m_name; }
    void name(QString &name) {
        m_name = name;
        emit nameChanged();
    }

    int width() const { return m_width; }
    void width(int &width) {
        m_width = width;
        emit widthChanged();
    }

    int height() const { return m_height; }
    void height(int &height) {
        m_height = height;
        emit heightChanged();
    }

    Channel *channel(int index) const {
        return m_channel_list.at(index);
    }

    QQmlListProperty<ChipComponent> components();
    QQmlListProperty<Channel> channels();

signals:
    void widthChanged();
    void heightChanged();
    void nameChanged();
    void componentsChanged();
    void channelsChanged();

public slots:

private:
    static void append_comp_list(
            QQmlListProperty<ChipComponent> *list,
            ChipComponent *chip_component);

    static int count_comp_list(QQmlListProperty<ChipComponent> *property);

    static ChipComponent *at_comp_list(
            QQmlListProperty<ChipComponent> *property,
            int index);

    static void clear_comp_list(QQmlListProperty<ChipComponent> *property);


    static void append_channel(QQmlListProperty<Channel> *property,
                               Channel *channel);

    static int count_channel(QQmlListProperty<Channel> *property);

    static Channel *at_channel(QQmlListProperty<Channel> *property,
                               int index);

    static void clear_channel(QQmlListProperty<Channel> *property);


    QString m_name;
    int m_width;
    int m_height;
    QList<ChipComponent *> m_comp_list;
    QList<Channel *> m_channel_list;
};

} // namespace model

#endif // NETWORK_H
