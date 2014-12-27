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
#include "noc.h"

Noc::Noc(QString name) : m_name(name), m_comp_map(), m_inport_map(), m_outport_map()
{
    // Empty Constructor body
}

Noc::~Noc()
{

}

const QString Noc::name() const {
    return m_name;
}

/**
 * @brief add a component to the network.
 *
 * This method adds a component to the network. It does not
 * create connections. @see connect for making connections.
 *
 * @return Noc
 */
Noc &Noc::add(std::shared_ptr<Component> comp) {
    if (m_comp_map.find(comp->name()) == m_comp_map.end()) {
        m_comp_map.insert(comp->name(), comp);
    }
    emit componentAdded(comp);
    return *this;
}

/**
 * @brief add an InPort to the network.
 *
 * This method adds an InPort to the network without
 * creating connections with other ports. @see connect.
 *
 * @param inport
 * @return Noc
 */
Noc &Noc::add(std::shared_ptr<InPort> inPort)
{
    if (m_inport_map.find(inPort->name()) == m_inport_map.end()) {
        m_inport_map.insert(inPort->name(), inPort);
    }
    emit inPortAdded(inPort);
    return *this;
}

/**
 * @brief add an OutPort to the network.
 *
 * This method adds an OutPort to the network without
 * creating connections with other ports. @see connect.
 *
 * @param outport
 * @return Noc
 */
Noc &Noc::add(std::shared_ptr<OutPort> outPort)
{
    if (m_outport_map.find(outPort->name()) == m_outport_map.end()) {
        m_outport_map.insert(outPort->name(), outPort);
    }
    emit outPortAdded(outPort);
    return *this;
}

/**
 * @brief Connect two components with specified portnames
 *
 * Connect the first component using one of its outports
 * with the second component using one of its inports.
 *
 *
 * @param comp_out A reference to the first component
 * @param port_out the outport for the first component
 * @param comp_in A reference to the second component
 * @param port_in the inport for the second component
 *
  * @return the Noc
 */
Noc &Noc::add(std::shared_ptr<Channel> channel)
{
    if (m_channel_map.find(channel->name()) == m_channel_map.end()) {
        m_channel_map.insert(channel->name(), channel);
    }
    emit channelAdded(channel);
    return *this;
}

/**
 * @brief valid checks the network for syntactic validity.
 *
 * This method checks whether the components, ports and connections are
 * completely specified and consistent.
 *
 * @return true if the network is valid.
 */
bool Noc::valid() {
    bool result = false;
    return result;
}

/**
 * @brief hasDeadlock checks the network for deadlocks.
 *
 * This method uses a verification tool to verify the existence of
 * deadlocks.
 *
 * @return true if a deadlock was found.
 */
bool Noc::hasDeadlock() {
    bool result = true;
    return result;
}

/**
 * @brief hasCycle checks the network for cycles
 *
 * This network uses a verification tool to verify the existence of
 * cycles.
 *
 * @return true if the a cycle was found.
 */
bool Noc::hasCycle() {
    bool result = true;
    return result;
}
