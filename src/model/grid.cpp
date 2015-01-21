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

#include "grid.h"

model::Grid::Grid(QObject *parent) : QObject(parent)
{

}

model::Grid::Grid(const Grid &rhs) : QObject() {
    if (this != &rhs) {
        m_width = rhs.m_width;
        m_height = rhs.m_height;
    }
}

model::Grid::~Grid()
{

}

void model::Grid::network(QString &network) {
    // TODO: check network exists, was declared earlier
    m_network = network;
}

model::Grid &model::Grid::operator=(const model::Grid &rhs) {
    if (this != &rhs) {
        m_width = rhs.m_width;
        m_height = rhs.m_height;
        m_network = rhs.m_network;
    }
    return *this;
}

