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

#include "port.h"

model::Port::Port(QObject *parent) : QObject(parent)
{

}

model::Port::~Port()
{

}

/**
 * @brief model::Port::Port
 *
 * Copy constructor for Port (necessary for
 * any class that uses Port as value parameter.
 *
 * @param rhs The port on the right hand side.
 */
model::Port::Port(const model::Port &rhs) {
    m_name = rhs.m_name;
    m_rdy = rhs.m_rdy;
}

/**
 * @brief model::Port::operator=
 *
 * The assignment operator for Port from another Port.
 * Self assignment is no issue.
 *
 * @param port
 */
model::Port &model::Port::operator=(const Port &rhs) {
    if (&rhs != this) {
        this->m_name = rhs.m_name;
        this->m_rdy = rhs.m_rdy;
    }
    return *this;
}

