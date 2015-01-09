#include "testnetwork.h"

#include "network.h"
#include "checker/xmas.h"

TestNetwork::TestNetwork(QObject *parent) : QObject(parent)
{

}

TestNetwork::~TestNetwork()
{

}

void TestNetwork::initTestCase() {
    qDebug ("call before anything else");
}

void TestNetwork::cleanupTestCase() {
    qDebug ("called after any test cases.");
}

void TestNetwork::createQueue() {

    Network network;

    XMASQueue* queue = new XMASQueue {"Queue1", 12};
    XMASComponent* queue2 = network.addComponent(queue);
    QVERIFY(queue == queue2);

    QVERIFY(queue->getName() == "Queue1");
    QVERIFY(queue->c == 12);

}

void TestNetwork::emitComponentAdded() {
    Network network;

    XMASComponent* emittedComponent = nullptr;
    XMASComponent* comp = new XMASSink("sink");

    connect(&network, &Network::componentAdded, [&](XMASComponent* c) {
        emittedComponent = c;
    });

    network.addComponent(comp);
    QCOMPARE(emittedComponent, comp);
}

void TestNetwork::duplicateComponentName() {
    Network network;

    auto c1 = new XMASQueue {"xMAS!", 12};
    auto c2 = new XMASFunction {"xMAS!"};

    network.addComponent(c1);
    QVERIFY_EXCEPTION_THROWN(network.addComponent(c2), NetworkException);
}
