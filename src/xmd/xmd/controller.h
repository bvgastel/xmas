/************************************************************************
 *
 * Copyright Stefan Versluys, 2014
 *
 * This file is part of the xmas-design tool.
 *
 *  The xmas-design tool is free software: you can redistribute it
 *  and/or modify it under the terms of the GNU General Public License
 *  as published by the Free Software Foundation, either version 3 of
 *  the License, or (at your option) any later version.
 *
 *  The xmas-design tool is distributed in the hope that it will be
 *  useful,  but WITHOUT ANY WARRANTY; without even the implied warranty
 *  of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with the xmas-design tool.  If not, see
 *  <http://www.gnu.org/licenses/>.
 *
 *
 * Parts of this code were copied from Qt examples with following copyright
 * and license notices:
 *
 * Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies)
 *
 * under BSD license.
 *
 *
 **************************************************************************/
#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QVariant>
#include <QColor>
#include <QDebug>

class Controller : public QObject
{
    Q_OBJECT
    Q_ENUMS(Orientation)
    Q_ENUMS(PortType)

signals: //to view
    void createComponent(QVariant object);
    bool createChannel(QVariant object);
    bool clearNetwork();
    void log(QString message,QColor color);

public slots:  //from view
    bool componentCreated(QVariant object);
    bool componentDestroyed(QVariant object);
    bool componentChanged(QVariant object);
    bool channelCreated(QVariant object);
    bool channelDestroyed(QVariant object);
    bool channelChanged(QVariant object);

    bool xmv2xmd(); //connected to the paste button on the toolbar
public:
    explicit Controller(QObject* parent = 0);
    enum Orientation {
        North = 0,
        East = 90,
        South = 180,
        West = 270,
        NorthWest = 45,
        SouthWest = 225,
        NorthEast = 315,
        SouthEast = 135
    };
    enum PortType {Target , Initiator};

    ~Controller();

private:
    bool scratch();
    void output(QString message,QColor color);

};

#endif // CONTROLLER_H









