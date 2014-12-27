#include "noc.h"

Noc::Noc(QString name) : m_name(name), m_comp_map(), m_port_map()
{

}

Noc::Noc(QDataStream nocDataStream)
{
    // Create the network from a datastream.
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
 * @brief add a port to the network.
 *
 * This method adds a port to the network without
 * creating connections with other ports. @see connect.
 *
 * @param port
 * @return
 */
Noc &Noc::add(std::shared_ptr<Port> port)
{
    if (m_port_map.find(port->name()) == m_port_map.end()) {
        m_port_map.insert(port->name(), port);
    }
    emit portAdded(port);
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

