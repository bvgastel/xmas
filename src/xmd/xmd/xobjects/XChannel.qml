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
 * Parts of this channelJs were copied from Qt examples with following copyright
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
import "qrc:/javascripts/xobjects/xchannel.js" as ChannelJs
import "qrc:/ui/uicontrols/"
import XMAS.model 1.0 as Model
import XMAS 1.0 as XMAS

Model.XChannel {
    id: channel

    // Properties
    objectName: "channel"
    focus: selected
    z:100
    property color color: "darkblue"
    property bool selected: false

    // Signals

    // JavaScripts

    // update position if outport moves
    function doUpdate1() {
        wire.x1 = mapFromItem(outport,5,5).x
        wire.y1 = mapFromItem(outport,5,5).y
    }

    // update position if inport moves
    function doUpdate2() {
        wire.x2 = mapFromItem(inport,5,5).x
        wire.y2 = mapFromItem(inport,5,5).y
    }

    // remove this
    function remove(){
        ChannelJs.remove(channel)
    }

    // Event handling
    onOutportChanged: outport ? doUpdate1() : null
    onInportChanged: inport ? doUpdate2() : null

    // Graphical representation of a channel --> "wire"
    XWire {
        id: wire
        color: selected ? "steelblue" : "darkblue"
        size: selected ? 4 : 2

        MouseArea {
            anchors.fill: parent
            anchors.margins: 5
            acceptedButtons: Qt.LeftButton | Qt.RightButton
            onClicked: {
                if (mouse.button == Qt.LeftButton) {
                    network.select(channel,
                                   mouse.modifiers === Qt.ControlModifier)
                    //                    var tmp = selected
                    //                    if(mouse.modifiers != Qt.ControlModifier){
                    //                        //channel.parent ? channel.parent.clearSelections(channel): null
                    //                    }
                    //                    selected = !tmp
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
            onTriggered: remove()
        }
    }

    Connections {
        target: outport
        onUpdate: doUpdate1()
        onRemoved: remove()
    }
    Connections {
        target: inport
        onUpdate: doUpdate2()
        onRemoved: remove()
    }
}

