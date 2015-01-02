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

#include "testcomponent.h"

TestComponent::TestComponent(QObject *parent) : QObject(parent)
{

}

TestComponent::~TestComponent()
{

}

void TestComponent::emptyComponent() {
    QCOMPARE(1,0);
}

void TestComponent::equalComponent() {
    QCOMPARE(1,0);
}

void TestComponent::unequalComponent() {
    QCOMPARE(1,0);
}

void TestComponent::componentNoPort() {
    QCOMPARE(1,0);
}

void TestComponent::componentInPort() {
    QCOMPARE(1,0);
}

void TestComponent::componentOutPort() {
    QCOMPARE(1,0);
}

void TestComponent::sink() {
    QCOMPARE(1,0);
}

void TestComponent::source() {
    QCOMPARE(1,0);
}

void TestComponent::merge() {
    QCOMPARE(1,0);
}


