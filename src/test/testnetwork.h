#ifndef TESTNETWORK_H
#define TESTNETWORK_H

#include <QtTest/QtTest>


class TestNetwork : public QObject
{
    Q_OBJECT
public:
    explicit TestNetwork(QObject *parent = 0);
    ~TestNetwork();

signals:

private slots:
    void initTestCase();

    void createQueue();
    void emitComponentAdded();
    void duplicateComponentName();

    void cleanupTestCase();
};

#endif // TESTNETWORK_H
