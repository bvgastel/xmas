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

#ifndef CHANNEL_H
#define CHANNEL_H

#include <QObject>

#include "gridpoint.h"
#include "chipcomponent.h"


// TODO: Consider using smart pointers : does it work well with qml / quick2? ---> Comment in github issue

namespace model {

/**
 * @brief The Channel class
 *
 * The wires between components on an Noc. Each
 * channel in reality consists of 3 wires:
 *
 * 1. The irdy or initiator ready signal wire
 * 2. The trdy or target ready signal wire
 * 3. The data wire.
 *
 * The data wire can have any type. The irdy
 * and trdy wires can send a signal when ready.
 * They are control wires.
 *
 * A channel has exactly two components with
 * on one end the output, from the channel reads
 * its data and on the other side the in port
 * to which the channel writes its data.
 *
 * The component attached to the outport sends
 * the signal irdy when ready. The other component
 * (inport bound) receives this signal.
 *
 * The compoent attached to the inport sends
 * the signal trdy when ready. The other component
 * (outport bound) receives this signal.
 *
 * TODO: [Channel] We need the data type for the VT
 * TODO: [Channel] We need to know the components we are connecting
 *
 */
class Channel : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString name READ name WRITE name NOTIFY nameChanged)
    Q_PROPERTY(QString initiator READ initiator WRITE initiator NOTIFY initiatorChanged)
    Q_PROPERTY(QString init_port READ init_port WRITE init_port NOTIFY init_portChanged)
    Q_PROPERTY(QString target READ target WRITE target NOTIFY targetChanged)
    Q_PROPERTY(QString target_port READ target_port WRITE target_port NOTIFY target_portChanged)
    Q_PROPERTY(QString datatype READ datatype WRITE datatype NOTIFY datatypeChanged)
    Q_PROPERTY(QQmlListProperty<model::GridPoint> ptList READ ptList NOTIFY ptListChanged)

    // FIXME: we probably cannot have Port in a property: what can we do? use the port name and comp name?

public:
    explicit Channel(QObject *parent = 0);
    ~Channel();

signals:
    void nameChanged();
    void initiatorChanged();
    void init_portChanged();
    void targetChanged();
    void target_portChanged();
    void datatypeChanged();
    void ptListChanged();

public slots:


public:

    QString name() const { return m_name; }
    void name(QString &name) { m_name = name; }

    QString initiator() const { return m_initiator; }
    void initiator(QString &initiator) { m_initiator = initiator; }

    QString init_port() const { return m_init_port; }
    void init_port(QString &init_port) { m_init_port = init_port; }

    QString target() const { return m_target; }
    void target(QString &target) { m_target = target; }

    QString target_port() const { return m_target_port; }
    void target_port(QString &target_port) { m_target_port = target_port; }

    QString datatype() const { return m_datatype; }
    void datatype(QString &datatype) { m_datatype = datatype; }

    QQmlListProperty<model::GridPoint> ptList();

    model::GridPoint *pt(const int index) { return m_ptList.at(index); }
    int ptSize() const {return m_ptList.size(); }

private:
    static void append_gridPoint(QQmlListProperty<GridPoint> *property, GridPoint *gridPoint);

    QString m_name;
    /**
     * @brief m_initiator The initiator of the channel: name of the chip component.
     */
    QString m_initiator;
    /**
     * @brief m_in_port The input port from the initiator.
     */
    QString m_init_port;
    /**
     * @brief m_target The target of the channel: name of the chip component.
     */
    QString m_target;
    /**
     * @brief m_out_port The output port to the target.
     */
    QString m_target_port;
    /**
     * @brief m_data The data type of the channel
     */
    QString m_datatype;
    /**
     * @brief m_ptList A list of points that fix a corner in the connecting line
     *
     * Between input and output port, the line can have additional points fixed that
     * it will pass. These points are listed in a QList.
     */
    QList<model::GridPoint *> m_ptList;

};

} // namespace model

#endif // CHANNEL_H
