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

#include "board.h"
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
    Q_PROPERTY(Board board READ board WRITE board NOTIFY boardChanged)
    Q_PROPERTY(QQmlListProperty<ChipComponent> components READ components NOTIFY componentsChanged)
    Q_PROPERTY(QQmlListProperty<Channel> channels READ channels NOTIFY channelsChanged)

public:
    explicit Network(QObject *parent = 0);
    ~Network();

    QString name() const { return m_name; }
    void name(QString &name) {
        m_name = name;
        emit nameChanged();
    }

    Board board() const { return m_board; }
    void board(Board &board) {
        m_board = board;
        emit boardChanged();
    }

    // TODO: Note to self: INVOKE not Q_PROPERTY
    // see http://doc.qt.io/qt-5/qtqml-cppintegration-data.html : INVOKE not Q_PROPERTY
    // That way both read and write of QList is cheaper.
    // CHeck: is this about QList vs QML? Or am I misunderstanding stuff?


    QQmlListProperty<ChipComponent> components();

    QQmlListProperty<Channel> channels();

signals:
    void boardChanged();
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
    Board m_board;
    QList<ChipComponent *> m_comp_list;
    QList<Channel *> m_channel_list;
};

} // namespace model

#endif // NETWORK_H
