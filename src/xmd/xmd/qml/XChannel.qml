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
    property var port1: null
    property var port2: null
    property color color: "darkblue"


    function doUpdate1() {
        line.requestPaint()
    }
    function doUpdate2() {
        line.requestPaint()
    }

    function remove() {
        port1.connected = false
        port2.connected = false
        destroy(channel)
        controller.channelDestroyed(channel)
    }

    //TODO replace straight canvas line with pathfinder logic (horizontal/vertical)
    Canvas {
        id: line
        anchors.fill: parent
        onPaint: {
            var x1 = mapFromItem(port1,10,10).x
            var y1 = mapFromItem(port1,10,10).y
            var x2 = mapFromItem(port2,10,10).x
            var y2 = mapFromItem(port2,10,10).y
            var ctx = getContext('2d')
            ctx.clearRect(0, 0, line.width, line.height);
            ctx.strokeStyle = "darkblue"
            ctx.lineWidth = 2.0
            ctx.beginPath()
            ctx.moveTo(x1,y1)
            ctx.lineTo(x2,y2)
            ctx.stroke()
        }
//        MouseArea {
//            anchors.fill: parent
//            preventStealing: true
//        }
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

