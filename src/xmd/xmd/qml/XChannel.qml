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
import QtQuick 2.4

Item {
    id: channel
    objectName: "channel"
    anchors.fill: parent
    property int id: 0
    property var port1: null
    property var port2: null
    property color color: "darkblue"


    function doUpdate1() {
        line.x1 = mapFromItem(port1,10,10).x
        line.y1 = mapFromItem(port1,10,10).y
    }
    function doUpdate2() {
        line.x2 = mapFromItem(port2,10,10).x
        line.y2 = mapFromItem(port2,10,10).y
    }

    function remove() {
        port1.connected = false
        port2.connected = false
        destroy(channel)
        //controller.channelDestroyed(channel)
    }


    onPort1Changed: port1 ? doUpdate1() : null
    onPort2Changed: port2 ? doUpdate2() : null


    //TODO replace straight canvas line with pathfinder logic (horizontal/vertical)
    Line {
        id: line
        color: "darkblue"
        size: 2
    }

    Connections {
        target: port1
        onUpdate: doUpdate1()
        onRemoved: channel.remove()
    }
    Connections {
        target: port2
        onUpdate: doUpdate2()
        onRemoved: channel.remove()
    }


}

