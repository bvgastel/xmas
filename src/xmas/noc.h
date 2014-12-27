#ifndef NOC_H
#define NOC_H

#include <QObject>
#include <QMap>
#include <QDataStream>
#include <memory>

#include "component.h"
#include "port.h"
#include "channel.h"

/**
 * @brief The Noc class
 *
 * The NoC class (Network on Chip) contains all elements
 * that create the network:
 *
 * 1. Components
 * 2. Connections.
 * 3. Ports
 *
 * Each NoC can also function as a component once finished.
 *
 * If needed the designer can close a Noc with sources and sinks.
 *
 */
class Noc : public QObject
{
    Q_OBJECT

public:

    explicit Noc(QString name);
    explicit Noc(QDataStream nocDataStream);
    ~Noc();

    const QString name() const;

    Noc &add(std::shared_ptr<Component> );
    Noc &add(std::shared_ptr<Port> port);
    Noc &add(std::shared_ptr<Channel> channel);

    bool valid();
    bool hasDeadlock();
    bool hasCycle();

signals:

    void componentAdded(std::shared_ptr<const Component> comp);
    void portAdded(std::shared_ptr<const Port> port);
    void channelAdded(std::shared_ptr<const Channel> channel);

private:
    /**
     * @brief name of the Noc
     */
    QString m_name;
    /**
     * @brief m_comp_map map of components by name
     */
    QMap<QString, std::shared_ptr<Component>> m_comp_map;
    /**
     * @brief m_port_map map of ports in Noc by name
     */
    QMap<QString, std::shared_ptr<Port>> m_port_map;

    /**
     * @brief m_connection_map map of connections.
     *
     * Connections between components are unique. Each
     * connection connection one output port with one
     * input port and each port can only partake in one
     * connection.
     */
    QMap<QString, std::shared_ptr<Channel>> m_channel_map;


};

#endif // NOC_H
