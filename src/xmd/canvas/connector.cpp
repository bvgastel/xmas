/************************************************************************
 *
 * Copyright Stefan Versluys, 2014
 *
 * This file is part of the xmas-design tool.
 *
 *  The xmas-design tool is free software: you can redistribute it
 *  and/or modify it under the terms of the GNU General Public License
 *  as published by the Free Software Foundation, either version 3 of
 *  the License, or (at your option) any later version.
 *
 *  The xmas-design tool is distributed in the hope that it will be
 *  useful,  but WITHOUT ANY WARRANTY; without even the implied warranty
 *  of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with the xmas-design tool.  If not, see
 *  <http://www.gnu.org/licenses/>.
 *
 *
 * Parts of this code were copied from Qt examples with following copyright
 * and license notices:
 *
 * Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies)
 *
 * under BSD license.
 *
 *
 **************************************************************************/

#include "connector.h"

class Component;
class Connection;

/**
 * @brief Connector::Connector
 */
Connector::Connector(QQuickItem *parent) : QQuickItem(parent),
    m_type(Connector::Input)
{
    m_connection = nullptr;
    m_connected = false;
}

/**
 * @brief Connector::~Connector
 */
Connector::~Connector()
{
}

/**
 * @brief Connector::name
 * @return
 */
QString Connector::name() const
{
    return m_name;
}

/**
 * @brief Connector::setName
 * @param name
 */
void Connector::setName(const QString &name)
{
    m_name = name;
}

/**
 * @brief Connector::deleteConnection
 * @param connection
 */
void Connector::deleteConnection()
{
    m_connection = nullptr;
}

