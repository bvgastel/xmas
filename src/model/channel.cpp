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

#include "channel.h"

model::Channel::Channel(QObject *parent) : QObject(parent)
{

}

model::Channel::~Channel()
{

}

QQmlListProperty<QPoint> model::Channel::ptList() {
    return QQmlListProperty<QPoint>(this, 0,
                                          &model::Channel::append_pt,
                                          0,
                                          &model::Channel::pt_at,
                                          0);
}

/**
 * @brief Channel::append_pt
 * @param list
 * @param connector
 */
void model::Channel::append_pt(QQmlListProperty<QPoint> *list,
                                   QPoint *pt)
{
    Channel *channel = qobject_cast<Channel *>(list->object);
    if (channel) {
        pt->setParent(channel);
        channel->m_portList.append(pt);
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
QPoint *model::ChipComponent::pt_at(QQmlListProperty<QPoint> *property, int index) {
    return static_cast< QList<QPoint *> *>(property->data)->at(index);
}

