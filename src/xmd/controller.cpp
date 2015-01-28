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

#include "controller.h"

Controller::Controller(QObject* parent)
    : QObject(parent)
{
}

/**
 * @brief Controller::componentCreated
 * @param comp
 * @return
 */
bool Controller::componentCreated(QVariant object)
{
    Q_UNUSED(object)
    qDebug() << "Component created by designer";
    qDebug() << "object = " << object;
    return true;
}

/**
 * @brief Controller::componentDestroyed
 * @param object
 * @return
 */
bool Controller::componentDestroyed(QVariant object)
{
    Q_UNUSED(object)
    qDebug() << "Component destroyed by designer";
    return true;
}

/**
 * @brief Controller::componentChanged
 * @param object
 * @return
 */
bool Controller::componentChanged(QVariant object)
{
    Q_UNUSED(object)
    qDebug() << "Component changed by designer";
    return true;
}


bool Controller::connectionCreated(QVariant object)
{
    Q_UNUSED(object)
    qDebug() << "Connection created by designer";
    return true;
}

bool Controller::connectionDestroyed(QVariant object)
{
    Q_UNUSED(object)
    qDebug() << "Connection destroyed by designer";
    return true;
}

bool Controller::connectionChanged(QVariant object)
{
    Q_UNUSED(object)
    qDebug() << "Connection changed by designer";
    return true;
}
