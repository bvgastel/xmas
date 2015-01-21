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
#include <QtQml>

#include "testport.h"
#include "utils.h"

TestPort::TestPort(QObject *parent) : QObject(parent)
{

}

TestPort::~TestPort()
{

}

/**
 * @brief TestPort::testPortCreation A simple test for creating a Port.
 *
 * No further test necessary: QML takes care of all
 */
void TestPort::testPortCreation() {
    model::Utils::registreModel();
    QQmlEngine engine;
    QQmlComponent component(&engine, QUrl("qrc:testport_1.qml"));
    model::Port *port = qobject_cast<model::Port *>(component.create());
    if (port) {
        QCOMPARE(port->name(), QString("testport1"));
        QCOMPARE(port->network(), QString("network1"));
        QCOMPARE(port->rdy(), QString("rdy1"));
        QCOMPARE(port->compName(), QString("compName1"));
    } else {
        QWARN("Creation of Port not successful. Recheck qml and qrc files.");
        QVERIFY(false);
    }

}

