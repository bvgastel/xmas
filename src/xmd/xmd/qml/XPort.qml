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
import "../content/connectionCreation.js" as Code

Item {
    id:port
    objectName: "port"
    width: 20; height:20
    x:-10; y:-10
    z:1
    property bool connected: false
    property string name: "a"
    property int type: 0 // input, output
    //orientation = roation property depends of de component body and type
    //property bool containsMouse: false

    signal update()
    signal removed()

    Rectangle{
        color: connected ? "black" : "red"
        anchors.fill: parent
        border.color: "black"
        border.width: area.containsMouse ? 4 : 0
    }

    onConnectedChanged: connected ? 0 : Code.abortConnecting(port)

    Component.onDestruction: removed()

    Connections {
     target: parent
     onUpdate: update()
     }

    MouseArea {
        id: area
        anchors.fill: parent
        anchors.margins: -10 // magic port :)
        hoverEnabled: !connected
        preventStealing: true
        acceptedButtons: Qt.LeftButton | Qt.RightButton
        onPressed: {
            if (mouse.button == Qt.LeftButton && !connected) {
                connected = true
                sheet.wiring(port)
            }
        }
        //onEntered: sheet.checkTarget(port)
        onContainsMouseChanged: {
            //port.containsMouse = containsMouse
            sheet.checkTarget(port)
        }
    }

}



