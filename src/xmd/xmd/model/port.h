/*********************************************************************
  *
  * Copyright (C) <name>, <year>
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

#ifndef PORT_H
#define PORT_H

#include <QQuickItem>

namespace model {

class Port : public QQuickItem
{
    Q_OBJECT
    Q_ENUMS(PortType)


//    property int id: 0 (komt van toepassing in composites , uniek volgnummer ipv naam)
//    property bool connected: false
//    property string name: "a"
//    property int type: XMAS.Data.Target

public:
    explicit Port(QQuickItem *parent = 0);
    ~Port();
    enum PortType {Target , Initiator};

signals:

public slots:
};

} // namespace model

#endif // PORT_H
