#ifndef NOC_H
#define NOC_H

#include <QObject>

class Noc
{
    Q_OBJECT

public:
    Noc();
    ~Noc();

    Noc &add(Component &);
    Noc &Connect(Component &, Component &);

    bool valid();
    bool hasDeadlock();
    bool hasCycle();

private:
    QVector<Component> qclist;
    Vector<Component> clist;


};

#endif // NOC_H
