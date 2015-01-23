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
#include "utils.h"

TestComponent::TestComponent(QObject *parent) : QObject(parent)
{

}

TestComponent::~TestComponent()
{

}

void TestComponent::testComponentCreation() {
    model::Utils::registreModel();
    QQmlEngine engine;
    QQmlComponent component(&engine, QUrl("qrc:testcomp_1.qml"));
    model::ChipComponent *comp = qobject_cast<model::ChipComponent *>(component.create());
    if (comp) {
        QCOMPARE(comp->name(), QString("testcomp1"));
        QVERIFY(comp->network() == nullptr);
        QCOMPARE(comp->x(), 1);
        QCOMPARE(comp->y(), 1);
        QCOMPARE(comp->count_connectors(), 2);
        for (int i = 0; i < comp->count_connectors(); i++) {
            QString n = QString(std::to_string(i+1).c_str());
            model::Port *p = comp->at(i);
            QCOMPARE(p->name(), QString("testport").append(n));
            QCOMPARE(p->rdy(), QString("rdy").append(n));
            // i = 0 ==> in, i == 1 ==> out
            bool isIn = i == 0 ? true : false;
            QCOMPARE(p->isInport(), isIn);
            QCOMPARE(p->isOutport(), !isIn);
        }
    } else {
        QString msg = model::Utils::qmlBuildError(component);
        QVERIFY2(false, msg.toStdString().c_str());
    }
}

