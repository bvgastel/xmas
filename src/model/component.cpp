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

/**
 * @brief Component::Component Constructor and default constructor
 * @param parent
 */
model::Component::Component(QObject *parent) : QObject(parent), m_orientation(Up)
{

}

/**
 * @brief Component::~Component Destructor
 */
model::Component::~Component()
{

}

/**
 * @brief Component::connectors
 * @return
 */
QQmlListProperty<model::Port> model::Component::connectors()
{
    return QQmlListProperty<model::Port>(this, 0,
                                               &model::Component::append_port,
                                               0,
                                               0,
                                               0);
}

/**
 * @brief Component::append_connector
 * @param list
 * @param connector
 */
void model::Component::append_port(QQmlListProperty<model::Port> *list,
                                   model::Port *port)
{
    Component *component = qobject_cast<Component *>(list->object);
    if (component) {
        port->setParent(component);
        component->m_portList.append(port);
    }
}
