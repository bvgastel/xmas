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

#include "component.h"

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
    Q_PROPERTY(Port initiator READ initiator WRITE initiator NOTIFY initiatorChanged)
    Q_PROPERTY(Port target READ target WRITE target NOTIFY targetChanged)

public:
    explicit Channel(QObject *parent = 0);
    ~Channel();

signals:
    void nameChanged();
    void initiatorChanged();
    void targetChanged();

public slots:


public:

    QString name() const { return m_name; }
    void name(QString &name) { m_name = name; }

    Port initiator() const { return m_initiator; }
    void initiator(Port &initiator) { m_initiator = initiator; }

    Port target() const { return m_target; }
    void target(Port &target) { m_target = target; }

private:

    QString m_name;
    /**
     * @brief m_initiator The initiator of the channel
     */
    Port m_initiator;
    /**
     * @brief m_target The target of the channel
     */
    Port m_target;

};

} // namespace model

#endif // CHANNEL_H
