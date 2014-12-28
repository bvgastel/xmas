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
#ifndef NOC_H
#define NOC_H

#include <QObject>
#include <QMap>
#include <QDataStream>
#include <memory>

#include "component.h"
#include "port.h"
#include "channel.h"

/**
 * @brief The Noc class
 *
 * The NoC class (Network on Chip) contains all elements
 * that create the network:
 *
 * 1. Components
 * 2. Ports
 * 3. Channels between ports on components
 *
 * Each NoC can also function as a component once finished.
 * Each channel represents 3 wires; irdy, trdy and data (@see channel)
 *
 * This class allows the construction of a Noc that can
 * later be used as a component.
 *
 * If needed the designer can close a Noc with sources and sinks.
 *
 */
class Noc : public Component, public QObject
{
    Q_OBJECT

public:

    explicit Noc(QString name);
    ~Noc();

    const QString name() const;

    Noc &add(std::shared_ptr<Component> );
    Noc &add(std::shared_ptr<InPort> inPort);
    Noc &add(std::shared_ptr<OutPort> outPort);
    Noc &add(std::shared_ptr<Channel> channel);

    bool valid();
    bool hasDeadlock();
    bool hasCycle();

signals:

    void componentAdded(std::shared_ptr<const Component> comp);
    void inPortAdded(std::shared_ptr<const InPort> inport);
    void outPortAdded(std::shared_ptr<const OutPort> outport);
    void channelAdded(std::shared_ptr<const Channel> channel);

private:
    /**
     * @brief name of the Noc
     */
    QString m_name;
    /**
     * @brief m_comp_map map of components by name
     */
    QMap<QString, std::shared_ptr<Component>> m_comp_map;
    /**
     * @brief m_inport_map map of InPorts in Noc by name
     */
    QMap<QString, std::shared_ptr<InPort>> m_inport_map;
    /**
     * @brief m_outport_map map of OutPorts in Noc by name
     */
    QMap<QString, std::shared_ptr<OutPort>> m_outport_map;

    /**
     * @brief m_connection_map map of connections.
     *
     * Connections between components are unique. Each
     * connection connection one output port with one
     * input port and each port can only partake in one
     * connection.
     */
    QMap<QString, std::shared_ptr<Channel>> m_channel_map;


};

#endif // NOC_H
