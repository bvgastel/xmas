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

Noc::Noc(QString name) : m_name(name), m_comp_map()
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
 * The identification of a component is its name. That means
 * a component name must be unique within a network.
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
 * @brief Connect two components with specified portnames
 *
 * Connect the first component using one of its outports
 * with the second component using one of its inports.
 *
 * The identification of a channel is its name. That means
 * a channel name must be unique within a network.
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

/**********************************************************************************************/

std::ostream &operator<< (std::ostream &os, const std::shared_ptr<Noc> noc) {
    os << noc->name().toStdString() << "[";
    std::string glue = "";
    foreach (auto comp , noc->m_comp_map) {
        os << glue << comp;
        glue = ", ";
    }
    os << "]";
    os.flush();
    return os;
}

bool operator== (const Noc &lnoc, const Noc &rnoc) {
    if ((lnoc.m_name != rnoc.m_name)
      || (lnoc.m_comp_map != rnoc.m_comp_map)
      || (lnoc.m_channel_map != rnoc.m_channel_map)) {
        return false;
    }
    return true;
}

bool operator!= (const Noc &lnoc, const Noc &rnoc) {
    return !(lnoc == rnoc);
}
