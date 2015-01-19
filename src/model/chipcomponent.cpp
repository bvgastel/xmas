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

#include <QtQml>

#include "port.h"
#include "chipcomponent.h"

/**
 * @brief Component::Component Constructor and default constructor
 * @param parent
 */
model::ChipComponent::ChipComponent(QObject *parent) : QObject(parent), m_orientation(Up)
{
}

/**
 * @brief Component::~Component Destructor
 */
model::ChipComponent::~ChipComponent()
{

}

/**
 * @brief Component::connectors
 * @return
 */
QQmlListProperty<model::Port> model::ChipComponent::connectors()
{
    return QQmlListProperty<model::Port>(this, 0,
                                               &model::ChipComponent::append_port,
                                               0,
                                               &model::ChipComponent::port_at,
                                               0);
}

/**
 * @brief Component::append_connector
 * @param list
 * @param connector
 */
void model::ChipComponent::append_port(QQmlListProperty<model::Port> *list,
                                   model::Port *port)
{
    ChipComponent *component = qobject_cast<ChipComponent *>(list->object);
    if (component) {
        port->setParent(component);
        component->m_portList.append(port);
    }
}

/**
 * @brief port_at returns a pointer to the port at specified index (zero based)
 *
 * @param list The property list
 * @param index The zero based index of the port within the list.
 * @return the port if successful, nulptr if not successful
 */
// FIXME: do we really need a cast to QList in order to get at(index)?
model::Port *model::ChipComponent::port_at(QQmlListProperty<model::Port> *property, int index) {
    return static_cast< QList<model::Port *> *>(property->data)->at(index);
}
