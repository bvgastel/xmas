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

#include "testchipcomponent.h"

TestComponent::TestComponent(QObject *parent) : QObject(parent)
{

}

TestComponent::~TestComponent()
{

}

void TestComponent::testComponentCreation() {
    qmlRegisterType<model::Port>("Model", 1, 0, "Port");
    qmlRegisterType<model::ChipComponent>("Model", 1, 0, "ChipComponent");
    QQmlEngine engine;
    QQmlComponent component(&engine, QUrl("qrc:testcomp_1.qml"));
    model::ChipComponent *comp = qobject_cast<model::ChipComponent *>(component.create());
    if (comp) {
        QCOMPARE(comp->name(), QString("testcomp1"));
        //FIXME: How to read the QQmlListProperty connectors?
        //qWarning() << comp->connectors.count();

        model::Port *p = comp->at(0);
        //model::Port *p = comp->connectors[0];
        //model::Port *p = comp->connectors::port_at(comp->connectors, 0);
        QCOMPARE(p->name(), QString("testport2"));
    } else {
        QWARN("Creation of Port not successful. Recheck qml and qrc files.");
        QVERIFY(false);
    }
}

