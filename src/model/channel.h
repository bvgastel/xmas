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

namespace model {

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
    Port m_initiator;
    Port m_target;

};

} // namespace model

#endif // CHANNEL_H
