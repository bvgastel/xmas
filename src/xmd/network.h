#ifndef NETWORK_H
#define NETWORK_H

#include <map>
#include <memory>
#include <QObject>

class XMASComponent;

class Network : public QObject
{
    Q_OBJECT
public:
    Network();
    ~Network();

    XMASComponent* addComponent(XMASComponent* component);

signals:
    void componentAdded(XMASComponent* component);

private:
    std::map<std::string, std::unique_ptr<XMASComponent>> m_components;
};

class NetworkException : public std::runtime_error
{
public:
    NetworkException(const std::string& msg)
        : runtime_error(msg)
    {
    }
};

#endif // NETWORK_H
