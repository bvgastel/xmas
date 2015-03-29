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
import XMAS.model 1.0 as Model
import XMAS 1.0 as XMAS

Model.XPort {
    id:port
    z:1
    objectName: "port"
    width: 10; height:10
    rotation: (port.type === Model.XPort.INPORT) ? 0 : 180
    property bool nameAlignCenter: false

    transform: Translate {x: -port.width/2; y: -port.height/2 }

    signal update()
    signal removed()

    Rectangle{
        id:portShape
        color: connected ? "black" : "red"
        anchors.fill: parent
        border.color: "black"
        border.width: mousearea.containsMouse && network.isValidPort(port) ? 2 : 0
        radius: port.type === Model.XPort.INPORT ? 0 : port.width * 0.5
    }
    Text {
        id:label
        text:name
        anchors.left: nameAlignCenter ? port.right : portShape.right
        anchors.bottom: nameAlignCenter ?  undefined : portShape.top
        anchors.verticalCenter: nameAlignCenter ? portShape.verticalCenter : undefined
        anchors.leftMargin: nameAlignCenter ? 15 : 0
        rotation:-port.rotation
        visible: mainwindow.showPortNames

    }


    Component.onDestruction: removed()

    // Connections
    Connections {
        target: component
        onUpdate: update()
    }


    MouseArea {
        id: mousearea
        anchors.fill: parent
        anchors.margins: -10 // magic port :)
        hoverEnabled: !connected && network.isValidPort(port)
        preventStealing: true
        acceptedButtons: Qt.LeftButton | Qt.RightButton
        onPressed: {
            if (mouse.button === Qt.LeftButton
                    && !connected) {
                //connected = true    --> not allowed, read-only
                // TODO: gbo: where to call network.connect(port-out, port-in)?
                network.wiring(port)
            } else {mouse.accepted=false}
        }
        onContainsMouseChanged: {
            network.checkTarget(port)
        }
    }
}
