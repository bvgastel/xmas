#ifndef NETWORK_H
#define NETWORK_H

#include <map>
#include <memory>
#include <QObject>

class XMASComponent;
class Input;
class Output;

class Network : public QObject
{
    Q_OBJECT
public:
    Network();
    ~Network();

    XMASComponent* addComponent(XMASComponent* component);
    void addChannel(Output& output, Input& input);

signals:
    void componentAdded(XMASComponent* component);
    void componentMoved(XMASComponent* component);
    void channelAdded(Output& output, Input& input);

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
