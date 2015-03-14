#ifndef NETWORK_H
#define NETWORK_H

#include <QQuickItem>

namespace model {
class Network : public QQuickItem
{
    Q_OBJECT

private:

    //property canvas size
    //property packet list
    //property name
    //property ...


public:
    explicit Network(QQuickItem *parent = 0);
    ~Network();
};
} // namespace model
#endif // NETWORK_H
