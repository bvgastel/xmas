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

#include "board.h"

model::Board::Board(QObject *parent) : QObject(parent)
{

}

model::Board::Board(const Board &rhs) : QObject() {
    if (this != &rhs) {
        m_network = rhs.m_network;
        m_width = rhs.m_width;
        m_height = rhs.m_height;
    }
}

model::Board::~Board()
{

}

void model::Board::network(QString &network) {
    // TODO: check network exists, was declared earlier
    m_network = network;
    emit networkChanged();
}

model::Board &model::Board::operator=(const model::Board &rhs) {
    if (this != &rhs) {
        // TODO: check network exists
        m_network = rhs.m_network;
        m_width = rhs.m_width;
        m_height = rhs.m_height;
    }
    return *this;
}
