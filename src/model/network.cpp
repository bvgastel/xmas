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

#include "network.h"

model::Network::Network(QObject *parent) : QObject(parent)
{

}

model::Network::~Network()
{

}

QQmlListProperty<model::ChipComponent> model::Network::components() {
    return QQmlListProperty<model::ChipComponent>(this, 0,
                                               &model::Network::append_comp_list,
                                               &model::Network::count_comp_list,
                                               &model::Network::at_comp_list,
                                               &model::Network::clear_comp_list);
}

QQmlListProperty<model::Channel> model::Network::channels() {
    return QQmlListProperty<model::Channel>(this, 0,
                                               &model::Network::append_channel,
                                               &model::Network::count_channel,
                                               &model::Network::at_channel,
                                               &model::Network::clear_channel);
}

int model::Network::count_comp_list(QQmlListProperty<model::ChipComponent> *property) {
    Network *network = qobject_cast<Network *>(property->object);
    if (network) {
        return network->m_comp_list.size();
    }
    //TODO: should we emit an error? How? this is static!
   return 0;
}

void model::Network::append_comp_list(QQmlListProperty<model::ChipComponent> *property,
                                   model::ChipComponent *chip_component)
{
    Network *network = qobject_cast<Network *>(property->object);
    if (network) {
        chip_component->setParent(network);
        network->m_comp_list.append(chip_component);
    }
}


// FIXME: do we really need a cast to QList in order to get at(index)?
/**
 * @brief model::Network::at_component
 *
 * @param property
 * @param index
 * @return
 */
model::ChipComponent *model::Network::at_comp_list(QQmlListProperty<model::ChipComponent> *property, int index) {
    return static_cast< QList<model::ChipComponent *> *>(property->data)->at(index);
}

void model::Network::clear_comp_list(QQmlListProperty<model::ChipComponent> *property) {
    Network *network = qobject_cast<Network *>(property->object);
    if (network) {
        network->m_comp_list.clear();
    }
}

int model::Network::count_channel(QQmlListProperty<model::Channel> *property) {
    Network *network = qobject_cast<Network *>(property->object);
    if (network) {
        return network->m_channel_list.size();
    }
    return 0;
}

void model::Network::append_channel(QQmlListProperty<model::Channel> *property,
                                   model::Channel *channel)
{
    Network *network = qobject_cast<Network *>(property->object);
    if (network) {
        channel->setParent(network);
        network->m_channel_list.append(channel);
    }
}


// FIXME: do we really need a cast to QList in order to get at(index)?
/**
 * @brief model::Network::at_channel
 *
 * @param property
 * @param index
 * @return
 */
model::Channel *model::Network::at_channel(QQmlListProperty<model::Channel> *property, int index) {
    return static_cast< QList<model::Channel *> *>(property->data)->at(index);
}

void model::Network::clear_channel(QQmlListProperty<model::Channel> *property) {
    Network *network = qobject_cast<Network *>(property->object);
    if (network) {
        network->m_channel_list.clear();
    }
}

