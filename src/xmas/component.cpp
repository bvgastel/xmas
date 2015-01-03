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
#include "component.h"


Component::Component(QString name)
    : m_name(name)
{
    if (m_name == "") {
        m_name = "unknown component";
    }
}

const QString Component::name() const {
    return m_name;
}

void Component::name(const QString name) {
    m_name = name;
}

Component &Component::add(std::shared_ptr<InPort> in) {
    if (m_inport_map.find(in->name()) == m_inport_map.end()) {
        m_inport_map.insert(in->name(), in);
    }
    return *this;
}

Component &Component::add(std::shared_ptr<OutPort> out) {
    if (m_outport_map.find(out->name()) == m_outport_map.end()) {
        m_outport_map.insert(out->name(), out);
    }
    return *this;
}

Component::Port::Port(QString name, std::shared_ptr<Component> comp)
    : m_name(name), m_comp(comp)
{
}

Component::Port::~Port()
{
}

const QString Component::Port::name() const {
    return m_name;
}

Component::OutPort::OutPort(QString name, std::shared_ptr<Component> comp, QString irdy) : Port(name, comp), m_irdy(irdy)
{

}

Component::OutPort::~OutPort()
{

}

Component::InPort::InPort(QString name, std::shared_ptr<Component> comp, QString trdy) : Port(name, comp), m_trdy(trdy)
{

}

Component::InPort::~InPort()
{

}

std::ostream &operator<< (std::ostream &os, const Component &comp) {
    os << comp.name() << "[in=(";
    std::string glue = "";
    foreach (auto port , comp.m_inport_map) {
        os << glue << port;
        glue = ", ";
    }
    os << "), out=(";
    foreach (auto port , comp.m_outport_map) {
        os << glue << port;
        glue = ", ";
    }
    os << ")]";
    os.flush();
    return os;
}

bool operator== (const Component lcomp, const Component rcomp) {
    if ((lcomp.m_name != rcomp.m_name)
        || (lcomp.m_inport_map != rcomp.m_inport_map)
        || (lcomp.m_outport_map != rcomp.m_outport_map)
        || (lcomp.m_function != lcomp.m_function)
        ) {
        return false;
    }
    return true;
}

bool operator!= (const Component lcomp, const Component rcomp) {
    return !(lcomp == rcomp);
}
