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
#include "channel.h"
#include "xmas.h"

model::Channel::Channel(QQuickItem *parent)
    : QQuickItem(parent),
      m_outport(nullptr),
      m_inport(nullptr)
{
}

model::Channel::~Channel()
{
}

model::XPort *model::Channel::outport() {
    return m_outport;
}

void model::Channel::outport(XPort *port){
    if (m_outport==port) return;
    m_outport = port;
    emit outportChanged();
}

model::XPort *model::Channel::inport() {
    return m_inport;
}

void model::Channel::inport(XPort *port){
    if (m_inport==port) return;
    m_inport = port;
    emit inportChanged();
}
