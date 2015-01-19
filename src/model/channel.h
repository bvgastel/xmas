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
    Q_PROPERTY(QString in_port READ in_port WRITE in_port NOTIFY in_portChanged)
    Q_PROPERTY(QString target READ target WRITE target NOTIFY targetChanged)
    Q_PROPERTY(QString out_port READ out_port WRITE setOut_port NOTIFY out_portChanged)
    Q_PROPERTY(QQmlListProperty<QPoint> ptList READ ptList NOTIFY ptListChanged)

    // FIXME: we probably cannot have Port in a property: what can we do? use the port name and comp name?

public:
    explicit Channel(QObject *parent = 0);
    ~Channel();

signals:
    void nameChanged();
    void initiatorChanged();
    void in_portChanged();
    void targetChanged();
    void out_portChanged();
    void ptListChanged();

public slots:


public:

    QString name() const { return m_name; }
    void name(QString &name) { m_name = name; }

    QString initiator() const { return m_initiator; }
    void initiator(QString &initiator) { m_initiator = initiator; }

    QString in_port() const { return m_in_port; }
    void in_port(QString &in_port) { m_in_port = in_port; }

    QString target() const { return m_target; }
    void target(QString &target) { m_target = target; }

    QString out_port() const { return m_out_port; }
    void out_port(QString &out_port) { m_out_port = out_port; }

    QQmlListProperty<QPoint *> ptList();

    QPoint *at(const int index) { return m_ptList.at(index); }

private:
    static void append_pt(QQmlListProperty<QPoint> *list, QPoint *port);
    static QPoint *pt_at(QQmlListProperty<QPoint> *list, int index);

    QString m_name;
    /**
     * @brief m_initiator The initiator of the channel: name of the chip component.
     */
    QString m_initiator;
    /**
     * @brief m_in_port The input port from the initiator.
     */
    QString m_in_port;
    /**
     * @brief m_target The target of the channel: name of the chip component.
     */
    QString m_target;
    /**
     * @brief m_out_port The output port to the target.
     */
    QString m_out_port;
    /**
     * @brief m_ptList A list of points that fix a corner in the connecting line
     *
     * Between input and output port, the line can have additional points fixed that
     * it will pass. These points are listed in a QList.
     */
    QList<QPoint *> m_ptList;

};

} // namespace model

#endif // CHANNEL_H
