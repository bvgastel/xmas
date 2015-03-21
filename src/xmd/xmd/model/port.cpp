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

#include "simplestring.h"
#include "port.h"
#include "component.h"

model::XPort::XPort(QQuickItem *parent)
    : QQuickItem(parent)
{
    m_component = nullptr;
}

model::XPort::~XPort()
{
    // NOTE: should we delete m_port?
}

void model::XPort::classBegin() {
    // no action necessary
}

void model::XPort::componentComplete() {
    m_component = qobject_cast<Component *>(parent());
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
    if (m_component) {
        XMASComponent *c = m_component->xmas_component();
        if (c) {
            for (Port *p : c->ports()) {
                if (QString(p->getName()) == getName()) {
                    return p;
                }
            }
        }
    }
    return nullptr;
}

