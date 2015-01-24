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
    QQmlComponent component1(&engine, QUrl("qrc:testport_1.qml"));
    model::Port *port1 = qobject_cast<model::Port *>(component1.create());
    if (port1) {
        QCOMPARE(port1->name(), QString("testport1"));
        QCOMPARE(port1->rdy(), QString("rdy1"));
        QCOMPARE(port1->isInport(), true);
        QCOMPARE(port1->isOutport(), false);
    } else {
        QString msg = model::Utils::qmlBuildError(component1);
        QVERIFY2(false, msg.toStdString().c_str());
    }
    QQmlComponent component2(&engine, QUrl("qrc:testport_2.qml"));
    model::Port *port2 = qobject_cast<model::Port *>(component2.create());
    if (port2) {
        QCOMPARE(port2->name(), QString("testport2"));
        QCOMPARE(port2->rdy(), QString("rdy2"));
        QCOMPARE(port2->isInport(), false);
        QCOMPARE(port2->isOutport(), true);
    } else {
        QString msg = model::Utils::qmlBuildError(component1);
        QVERIFY2(false, msg.toStdString().c_str());
    }

}

