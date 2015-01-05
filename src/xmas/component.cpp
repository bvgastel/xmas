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
#include <stdexcept>
#include <iostream>

#include <QtDebug>

#include "component.h"

/**
 * @brief Component::Component
 *
 * The constructor for a component with a specified name and the input ports and
 * output ports specified. The function is an arbitrary string containing the
 * function that the component performs on the input to produce output.
 *
 * @param name          The name of the component (should not be empty string)
 * @param inport_list   The list of input ports of the component. The list is a QVector
 *                      containing a pair of name and irdy proposition.
 * @param outport_list  The list of output ports of the component. The list is a QVector
 *                      containing a pair of name and trdy proposition.
 * @param function      The string representing the function the component performs
 *                      (default is empty string).
 */

Component::Component(QString name,
                     PortList inport_list,
                     PortList outport_list,
                     Function function) :   m_name(name),
                                            m_inport_map(),
                                            m_outport_map(),
                                            m_function(function)
{
    if (m_name == "") {
        throw std::runtime_error("component without name");
    }

    if (inport_list.isEmpty() && outport_list.isEmpty()) {
        throw std::runtime_error("component without ports is not a legal component");
    }

    /*
     * name_prop.first == name
     * name_prop.second == irdy / trdy string
     */
    foreach (auto name_prop, inport_list) {
        QString name = name_prop.first;
        QString prop = name_prop.second;
        auto in = std::make_shared<Port>(name, prop);
        if (m_inport_map.contains(name) || m_outport_map.contains(name)) {
            throw std::runtime_error("Double definition of port with name '" + name.toStdString());
        }
        m_inport_map.insert(name, in);
    }
    foreach (auto name_prop, outport_list) {
        auto out = std::make_shared<Port>(name_prop.first, name_prop.second);
        m_outport_map.insert(name_prop.first, out);
    }

}

/**
 * @brief Component::inport_size
 * @return the number of in ports.
 */
int Component::in_size() const {
    return m_inport_map.size();
}
/**
 * @brief Component::outport_size
 * @return the number of out ports.
 */
int Component::out_size() const {
    return m_outport_map.size();
}

Component::Function Component::rdy(QString name) const {

    if (!portExists(name)) {
        throw std::runtime_error("inport " + name.toStdString() + " does not exist.");
    }
    Function function;
    if (m_inport_map.contains(name)) {
        function = m_inport_map[name]->rdy();
    } else if (m_outport_map.contains(name)) {
        function = m_outport_map[name]->rdy();
    } else {
        throw std::runtime_error("This should never happen. Find help.");
    }
    return function;
}



Component::PortList Component::inList() const {
    return portList(m_inport_map);
}

Component::PortList Component::outList() const {
    return portList(m_outport_map);
}

Component::PortList Component::portList(QHash<QString, std::shared_ptr<Port>> portmap) const {
    PortList list;
    foreach(auto port, portmap) {
        PortNamePropPair pair = PortNamePropPair(port->name(), port->rdy());
        list.append(pair);
    }

    return list;
}

bool Component::portExists(QString name) const {
    return inportExists(name) || outportExists(name);
}

bool Component::inportExists(QString name) const {
    return m_inport_map.contains(name);
}

bool Component::outportExists(QString name) const {
    return m_outport_map.contains(name);
}

/************************************* Port methods *****************************************************************/

Component::Port::Port(QString name, QString rdy)
    : m_name(name), m_rdy(rdy)
{
}

Component::Port::~Port()
{
}

/********************************* Operator overrides ******************************************/

std::ostream &operator<< (std::ostream &os, const Component &comp) {
    os << comp.name().toStdString() << "[in=(";
    std::string glue = "";

    foreach (auto port, comp.m_inport_map) {
        os << glue << port;
        glue = ", ";
    }
    os << "), out=(";
    foreach (auto port, comp.m_outport_map) {
        os << glue << port;
        glue = ", ";
    }
    os << ")]";

    os.flush();
    return os;
}

QDataStream &operator<< (QDataStream &ds, const Component &comp) {
    ds << comp.name() << "[in=(";
    std::string glue = "";

    foreach (auto port, comp.m_inport_map) {
        ds << glue.c_str() << *port;
        glue = ", ";
    }
    ds << "), out=(";
    foreach (auto port, comp.m_outport_map) {
        ds << glue.c_str() << *port;
        glue = ", ";
    }
    ds << ")]";

    return ds;
}

bool operator== (const Component lcomp, const Component rcomp) {
    bool result = true;
    if (lcomp.m_name != rcomp.m_name) {
        result = false;
    }
    if (!checkPortEq(lcomp.m_outport_map, rcomp.m_outport_map)) {
        result = false;
    }
    if (!checkPortEq(lcomp.m_inport_map, rcomp.m_inport_map)) {
        result = false;
    }
    if (lcomp.m_function != rcomp.m_function) {
        result = false;
    }
    return result;
}

bool checkPortEq(const QHash<QString, std::shared_ptr<Component::Port>> &left, const QHash<QString, std::shared_ptr<Component::Port>> &right) {
    bool result = true;
    if (left.size() != right.size()) {
        result = false;
    } else {
        auto lkeys = left.keys();
        foreach (QString key, lkeys) {
            if (!right.contains(key)) {
                result = false;
                break;
            }
            if (*left[key] != *right[key]) {
                result = false;
                break;
            }
        }
    }
    return result;
}

std::ostream &operator<< (std::ostream &os, const Component::Port &port) {
    os << port.name().toStdString() << "(" << port.rdy().toStdString() << ")";
    os.flush();
    return os;
}

QDataStream &operator<< (QDataStream &ds, const Component::Port &port) {
    ds << port.name() << "(" << port.rdy() << ")";
    return ds;
}

bool operator==(const Component::Port l_port, const Component::Port r_port) {
    return  l_port.m_name == r_port.m_name &&
            l_port.m_rdy == r_port.m_rdy;
}
