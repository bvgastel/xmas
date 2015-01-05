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

#include <memory>
#include <stdexcept>

#include <QBuffer>

#include "testcomponent.h"

TestComponent::TestComponent(QObject *parent) : QObject(parent)
{

}

TestComponent::~TestComponent()
{

}

/**
 * @brief TestComponent::checkComponent
 *
 *  A private utility function to check some basic characteristics
 *  after creation. Any deviation leads to a normal test deviation
 *  as if a testcase failed. Probably the pre condition for a test
 *  case is not fully satisfied.
 *
 * @param comp          The component you want to check
 * @param name          The name it should have
 * @param in_size       The number of in ports.
 * @param out_size      The number of out ports.
 * @param function      The value of function.
 */
void TestComponent::checkComponent(std::shared_ptr<Component> comp, QString name, int in_size, int out_size, QString function) {
    QVERIFY(comp->name() == name);
    QVERIFY(comp->in_size() == in_size);
    QVERIFY(comp->out_size() == out_size);
    QVERIFY(comp->function() == function);
}

void TestComponent::emptyComponent() {
    try {
        Component::PortList in = Component::PortList();
        Component::PortList out = Component::PortList();
        std::shared_ptr<Component> comp = std::make_shared<Component>("empty", in, out);
        QFAIL("Creating a component without ports should fail miserably.");
    } catch (std::runtime_error &r) {
        QVERIFY(true);
    }
}

void TestComponent::equalComponent() {
    Component::PortList in1 = Component::PortList();
    Component::PortList out1 = Component::PortList();
    Component::PortNamePropPair p1 = {"in1", "function 1"};
    in1.append(p1);
    std::shared_ptr<Component> comp1 = std::make_shared<Component>("equal", in1, out1);

    Component::PortList in2 = Component::PortList();
    Component::PortList out2 = Component::PortList();
    Component::PortNamePropPair p2 = {"in1", "function 1"};
    in2.append(p2);
    std::shared_ptr<Component> comp2 = std::make_shared<Component>("equal", in2, out2);

    checkComponent(comp1, "equal", 1, 0, "");
    checkComponent(comp2, "equal", 1, 0, "");

    QVERIFY(*comp1 == *comp2);
}

void TestComponent::equalComponentInPort() {
    Component::PortList in1 = Component::PortList();
    Component::PortNamePropPair p1 = {"in1", "function 1"};
    in1.append(p1);
    Component::PortList out1 = Component::PortList();
    std::shared_ptr<Component> comp1 = std::make_shared<Component>("1 in port", in1, out1);

    Component::PortList in2 = Component::PortList();
    Component::PortNamePropPair p2 = {"in1", "function 1"};
    in2.append(p2);
    Component::PortList out2 = Component::PortList();
    std::shared_ptr<Component> comp2 = std::make_shared<Component>("1 in port", in2, out2);

    checkComponent(comp1, "1 in port", 1, 0, "");
    checkComponent(comp2, "1 in port", 1, 0, "");

    QVERIFY(*comp1 == *comp2);
}

void TestComponent::equalComponentOutPort() {
    Component::PortList in1 = Component::PortList();
    Component::PortList out1 = Component::PortList();
    Component::PortNamePropPair p1 = {"out1", "function 1"};
    out1.append(p1);
    std::shared_ptr<Component> comp1 = std::make_shared<Component>("1 out port", in1, out1);

    Component::PortList in2 = Component::PortList();
    Component::PortList out2 = Component::PortList();
    Component::PortNamePropPair p2 = {"out1", "function 1"};
    out2.append(p2);
    std::shared_ptr<Component> comp2 = std::make_shared<Component>("1 out port", in2, out2);

    checkComponent(comp1, "1 out port", 0, 1, "");
    checkComponent(comp2, "1 out port", 0, 1, "");

    QVERIFY(*comp1 == *comp2);
}

void TestComponent::equalComponentFunction() {
    Component::PortList in1 = Component::PortList();
    Component::PortList out1 = Component::PortList();
    Component::PortNamePropPair p1 = {"out1", "function 1"};
    out1.append(p1);
    std::shared_ptr<Component> comp1 = std::make_shared<Component>("1 function", in1, out1, "1");

    Component::PortList in2 = Component::PortList();
    Component::PortList out2 = Component::PortList();
    Component::PortNamePropPair p2 = {"out1", "function 1"};
    out2.append(p2);
    std::shared_ptr<Component> comp2 = std::make_shared<Component>("1 function", in2, out2, "1");

    checkComponent(comp1, "1 function", 0, 1, "1");
    checkComponent(comp2, "1 function", 0, 1, "1");

    QVERIFY(*comp1 == *comp2);
}

void TestComponent::unequalComponentName() {
    Component::PortList in1 = Component::PortList();
    Component::PortList out1 = Component::PortList();
    Component::PortNamePropPair p1 = {"in1", "function 1"};
    in1.append(p1);
    std::shared_ptr<Component> comp1 = std::make_shared<Component>("name 1", in1, out1, "1");

    Component::PortList in2 = Component::PortList();
    Component::PortList out2 = Component::PortList();
    Component::PortNamePropPair p2 = {"in1", "function 1"};
    in2.append(p2);
    std::shared_ptr<Component> comp2 = std::make_shared<Component>("name 2", in2, out2, "1");

    checkComponent(comp1, "name 1", 1, 1, "1");
    checkComponent(comp2, "name 2", 1, 1, "1");

    QVERIFY(*comp1 != *comp2);
}

void TestComponent::unequalComponentInPortSize() {
    Component::PortList in1 = Component::PortList();
    Component::PortList out1 = Component::PortList();
    Component::PortNamePropPair p1 = {"in1", "function 1"};
    in1.append(p1);
    std::shared_ptr<Component> comp1 = std::make_shared<Component>("name 1", in1, out1, "1");

    Component::PortList in2 = Component::PortList();
    Component::PortList out2 = Component::PortList();
    Component::PortNamePropPair p2 = {"in1", "function 1"};
    in2.append(p2);
    Component::PortNamePropPair p3 = {"in2", "function 1"};
    in2.append(p3);

    std::shared_ptr<Component> comp2 = std::make_shared<Component>("name 1", in2, out2, "1");

    checkComponent(comp1, "name 1", 1, 0, "1");
    checkComponent(comp2, "name 1", 2, 0, "1");

    QVERIFY(*comp1 != *comp2);
}
void TestComponent::unequalComponentInPort() {
    Component::PortList in1 = Component::PortList();
    Component::PortList out1 = Component::PortList();
    Component::PortNamePropPair p1 = {"in1", "function 1"};
    in1.append(p1);
    std::shared_ptr<Component> comp1 = std::make_shared<Component>("name 1", in1, out1, "1");

    Component::PortList in2 = Component::PortList();
    Component::PortList out2 = Component::PortList();
    Component::PortNamePropPair p2 = {"in1", "function 2"};
    in2.append(p2);
    std::shared_ptr<Component> comp2 = std::make_shared<Component>("name 1", in2, out2, "1");

    checkComponent(comp1, "name 1", 1, 0, "1");
    checkComponent(comp2, "name 1", 1, 0, "1");

    QVERIFY(*comp1 != *comp2);
}
void TestComponent::unequalComponentOutPortSize() {
    Component::PortList in1 = Component::PortList();
    Component::PortList out1 = Component::PortList();
    Component::PortNamePropPair p1 = {"in1", "function 1"};
    in1.append(p1);
    std::shared_ptr<Component> comp1 = std::make_shared<Component>("name 1", in1, out1, "1");

    Component::PortList in2 = Component::PortList();
    Component::PortList out2 = Component::PortList();
    Component::PortNamePropPair p2 = {"in1", "function 1"};
    in2.append(p2);
    Component::PortNamePropPair p3 = {"out1", "function 1"};
    out2.append(p3);
    std::shared_ptr<Component> comp2 = std::make_shared<Component>("name 1", in2, out2, "1");

    QVERIFY(*comp1 != *comp2);
}
void TestComponent::unequalComponentOutPortName() {
    Component::PortList in1 = Component::PortList();
    Component::PortList out1 = Component::PortList();
    Component::PortNamePropPair p1 = {"in1", "function 1"};
    in1.append(p1);
    Component::PortNamePropPair p3 = {"out1", "function 1"};
    out1.append(p3);
    std::shared_ptr<Component> comp1 = std::make_shared<Component>("name 1", in1, out1, "1");

    Component::PortList in2 = Component::PortList();
    Component::PortList out2 = Component::PortList();
    Component::PortNamePropPair p2 = {"in1", "function 1"};
    in2.append(p2);
    Component::PortNamePropPair p4 = {"out2", "function 1"};
    out2.append(p4);
    std::shared_ptr<Component> comp2 = std::make_shared<Component>("name 1", in2, out2, "1");

    checkComponent(comp1, "name 1", 1, 1, "1");
    checkComponent(comp2, "name 1", 1, 1, "1");

    QVERIFY(*comp1 != *comp2);
}
void TestComponent::unequalComponentOutPortFunction() {
    Component::PortList in1 = Component::PortList();
    Component::PortList out1 = Component::PortList();
    Component::PortNamePropPair p1 = {"in1", "function 1"};
    in1.append(p1);
    Component::PortNamePropPair p3 = {"out1", "function 1"};
    out1.append(p3);
    std::shared_ptr<Component> comp1 = std::make_shared<Component>("name 1", in1, out1, "1");

    Component::PortList in2 = Component::PortList();
    Component::PortList out2 = Component::PortList();
    Component::PortNamePropPair p2 = {"in1", "function 1"};
    in2.append(p2);
    Component::PortNamePropPair p4 = {"out1", "function 2"};
    out2.append(p4);
    std::shared_ptr<Component> comp2 = std::make_shared<Component>("name 1", in2, out2, "1");

    checkComponent(comp1, "name 1", 1, 1, "1");
    checkComponent(comp2, "name 1", 1, 1, "1");

    QVERIFY(*comp1 != *comp2);
}
void TestComponent::unequalComponentFunction() {
    Component::PortList in1 = Component::PortList();
    Component::PortList out1 = Component::PortList();
    Component::PortNamePropPair p1 = {"in1", "function 1"};
    in1.append(p1);
    std::shared_ptr<Component> comp1 = std::make_shared<Component>("1 function", in1, out1, "1");

    Component::PortList in2 = Component::PortList();
    Component::PortList out2 = Component::PortList();
    Component::PortNamePropPair p2 = {"in1", "function 1"};
    in2.append(p2);
    std::shared_ptr<Component> comp2 = std::make_shared<Component>("1 function", in2, out2, "2");

    checkComponent(comp1, "name 1", 1, 0, "1");
    checkComponent(comp2, "name 1", 1, 0, "2");

    QVERIFY(*comp1 != *comp2);
}

// TODO: Finish serializing: how to read from buffer to Component object without default constructor?
void TestComponent::serializeComponent() {
    Component::PortList in1 = Component::PortList();
    Component::PortList out1 = Component::PortList();
    Component::PortNamePropPair p1 = {"in1", "function 1"};
    in1.append(p1);
    Component::PortNamePropPair p3 = {"out1", "function 1"};
    out1.append(p3);
    std::shared_ptr<Component> comp1 = std::make_shared<Component>("name 1", in1, out1, "1");

    checkComponent(comp1, "name 1", 1, 1, "1");

    QBuffer buffer;
    QDataStream data(&buffer);

    data << *comp1;

//    Component comp2;
//    data >> comp2;

    QVERIFY(false);
}

