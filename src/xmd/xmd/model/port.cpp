/*********************************************************************
  *
  * Copyright (C) <name>, <year>
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

#include "port.h"
#include "component.h"

model::XPort::XPort(QQuickItem *parent)
    : QQuickItem(parent)
{
}

model::XPort::~XPort()
{
}

model::XPort::PortType model::XPort::type() const {
    return m_type;
}

void model::XPort::type(PortType type) {
    m_type = type;
    emit typeChanged();
}

QString model::XPort::name() {
    return m_name;
}

void model::XPort::name(QString name) {
    if (name != m_name) {
        m_name = name;
    }
    emit nameChanged();
}

bool model::XPort::isConnected() {
    Port *port = this->getPort();
    if (port) {
        return port->isConnected();
    }
    return false;
}

/**
 * @brief model::XPort::getPort
 *
 * Find the Port as defined in xmas.h and return
 * it. This port should have the same name;
 *
 * Note: no check on type, this must be correct as
 * set by Component.
 *
 * @return
 */
Port *model::XPort::getPort() {
    auto component = qobject_cast<Component *>(this->parent());
    if (component) {
        XMASComponent *c = component->xmas_component();
        if (c) {
            for (Port *p : c->ports()) {
                if (QString(p->getName()) == name()) {
                    return p;
                }
            }
        }
    }
    return nullptr;
}

model::Component *model::XPort::getComponent() {
    auto component = qobject_cast<Component *>(parent());
    return component;
}
