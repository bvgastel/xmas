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

Channel::Channel(std::shared_ptr<Component::OutPort> out, std::shared_ptr<Component::InPort> in)
        : m_out(out), m_in(in)
{
}

Channel::~Channel()
{
    // no delete necessary
}

const QString Channel::name() const
{
    return m_name;
}
/**
 * @brief Channel::in
 *
 * This is the inport that this channel is connected to.
 * The channel writes to the inport so the component
 * can read from the in.
 *
 * @return the in port of type InPort.
 */
std::shared_ptr<const Component::InPort> Channel::initiator() const
{
    return m_in;
}

/**
 * @brief Channel::out
 *
 * This is the outport that this channel is connected to.
 * The channel reads from the outport that the
 * component writes to.
 *
 * @return the out port of type OutPort.
 */
std::shared_ptr<const Component::OutPort> Channel::target() const
{
    return m_out;
}
