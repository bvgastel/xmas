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

QQmlListProperty<model::GridPoint> model::Channel::ptList() {
    return QQmlListProperty<model::GridPoint>(this, 0,
                                          &model::Channel::append_gridPoint,
                                          0,
                                          0,
                                          0);
}

/**
 * @brief model::Channel::network Setter for the networkname
 *
 * This method sets the network name, provided the network was declared earlier.
 * If network name does not exist, the method emits an error signal containing the error message.
 *
 * It is up to the user to catch the error with a slot.
 *
 * @param network The name of the network
 */
void model::Channel::network(QString &network) {
    // TODO: check network name
    m_network = network;
}

/**
 * @brief Channel::append_pt
 * @param property
 * @param gridPoint
 */
void model::Channel::append_gridPoint(QQmlListProperty<model::GridPoint> *property,
                                   model::GridPoint *gridPoint)
{
    Channel *channel = qobject_cast<Channel *>(property->object);
    if (channel) {
        gridPoint->setParent(channel);
        channel->m_ptList.append(gridPoint);
    }
}


