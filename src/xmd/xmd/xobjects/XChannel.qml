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
import "xchannel.js" as Code
import "../uicontrols"
import XMAS.model 1.0 as Model

Model.XChannel {
    id: channel
    objectName: "channel"
    focus: true
    property int id: 0
    outport: null           // NOTE: This means no value onComponentComplete()
    inport: null            // NOTE: This means no value onComponentComplete()
    property color color: "darkblue"
    property bool selected: false


    // TODO: gbo: insert network.connect(out, in) here??
    function doUpdate1() {
        wire.x1 = mapFromItem(outport,5,5).x
        wire.y1 = mapFromItem(outport,5,5).y
    }
    function doUpdate2() {
        wire.x2 = mapFromItem(inport,5,5).x
        wire.y2 = mapFromItem(inport,5,5).y
    }


    onOutportChanged: outport ? doUpdate1() : null
    onInportChanged: inport ? doUpdate2() : null

    onSelectedChanged: {
        focus = selected
        wire.color = selected ? "steelblue" : "darkblue"
        wire.size = selected ? 4 : 2
    }

    // FIXME: Stefan: not sure how to do this, want to connect signal to mainWindow.log()
    onWriteLog: console.log(msg, color)

    //TODO: replace straight canvas wire with pathfinder logic (horizontal/vertical)
    XWire {
        id: wire
        color: "darkblue"
        size: 2

        MouseArea {
            anchors.fill: parent
            anchors.margins: 5
            acceptedButtons: Qt.LeftButton | Qt.RightButton
            onClicked: {
                if (mouse.button == Qt.LeftButton) {
                    var tmp = selected
                    if(mouse.modifiers != Qt.ControlModifier){
                        //channel.parent ? channel.parent.clearSelections(channel): null
                    }
                    selected = !tmp
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
            onTriggered: Code.remove(channel)
        }
    }

    Connections {
        target: outport
        onUpdate: doUpdate1()
        onRemoved: Code.remove(channel)
    }
    Connections {
        target: inport
        onUpdate: doUpdate2()
        onRemoved: Code.remove(channel)
    }

    Connections {
        target: parent
//        onGroupSelected: Code.selected = group.contains(wire.x1,wire.y1) || group.contains(wire.x2,wire.y2)
//        onDeleteSelected: if (Code.selected) Code.remove()
//        onClearSelection: Code.selected = false
    }


}

