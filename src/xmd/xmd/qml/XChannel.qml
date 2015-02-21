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
import QtQuick.Controls 1.3

Item {
    id: channel
    objectName: "channel"
    focus: true
    property int id: 0
    property var port1: null
    property var port2: null
    property color color: "darkblue"
    property bool selected: false


    function doUpdate1() {
        line.x1 = mapFromItem(port1,5,5).x
        line.y1 = mapFromItem(port1,5,5).y
    }
    function doUpdate2() {
        line.x2 = mapFromItem(port2,5,5).x
        line.y2 = mapFromItem(port2,5,5).y
    }

    function remove() {
        if(port1) port1.connected = false
        if(port2) port2.connected = false
        destroy(channel)
        //log("Channel deleted!","black")
        //TODO : channel is already null - send unique id or tempory copy?
        //controller.channelDestroyed(channel)
    }


    onPort1Changed: port1 ? doUpdate1() : null
    onPort2Changed: port2 ? doUpdate2() : null

    onSelectedChanged: {
        focus = selected
        line.color = selected ? "lightsteelblue" : "darkblue"
    }

    //TODO replace straight canvas line with pathfinder logic (horizontal/vertical)
    Line {
        id: line
        color: "darkblue"
        size: 2

        MouseArea {
            anchors.fill: parent
            anchors.margins: 5
            acceptedButtons: Qt.LeftButton | Qt.RightButton
            onClicked: {
                if (mouse.button == Qt.LeftButton) {
                    selected = !selected
                }
                if (mouse.button == Qt.RightButton){
                    contextMenu.popup()
                }
            }
        }
    }

    Menu {
        id: contextMenu
        MenuItem {
            text: "Delete"
            onTriggered: channel.remove()
        }
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

    Connections {
        target: parent
        onGroupSelected: channel.selected = group.contains(line.x1,line.y1) || group.contains(line.x2,line.y2)
        onDeleteSelected: if (channel.selected) channel.remove()
        onClearSelection: channel.selected = false
    }


}

