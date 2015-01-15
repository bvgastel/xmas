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

#ifndef CONNECTION_H
#define CONNECTION_H

#include <QQuickItem>
#include "connector.h"

class Connector;

/**
 * @brief The Connection class
 */
class Connection : public QQuickItem
{
    Q_OBJECT

public:

    Connection(QQuickItem * parent=0);

    Connector *startConnector() const { return m_startConnector; }
    Connector *endConnector() const { return m_endConnector; }

    virtual ~Connection();

private:
    Connector *m_startConnector;
    Connector *m_endConnector;

};

#endif // CONNECTION_H
