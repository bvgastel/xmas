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
import QtQuick.Layouts 1.0
import QtQuick.Dialogs 1.1
import QtQuick.Window 2.1
import "content"
import "content/connectionCreation.js" as Code
import "controller.js" as Ctrl


Rectangle {
    id: sheet
    scale: zoomFactor
    width: 2970
    height: 2100
    color: "white"
    property real zoomFactor: 1.0

    function zoomFit()
    {
        zoomFactor = 1.0;
    }
    function zoomIn()
    {
        zoomFactor = zoomFactor + 0.1;
        if (zoomFactor > 2) zoomFactor = 2
    }
    function zoomOut()
    {
        zoomFactor = zoomFactor - 0.1;
        if (zoomFactor < 0.1) zoomFactor = 0.1
    }

    focus: true
    z: -10


    //used to show the wiring path when adding a connection
    Canvas {
        id: wire
        z:20
        anchors.fill: parent
        property real mouseX: 0
        property real mouseY: 0
        property bool connecting: false
        property var connector1: null
        property var connector2: null

        visible: connecting
        enabled: connecting
        onPaint: {
            var ctx = getContext('2d')
            ctx.strokeStyle = "darkblue"
            ctx.lineWidth = 4.0
            ctx.clearRect(0, 0, wire.width, wire.height);
            if (connecting)
            {
                var x =  connector1.parent.x + connector1.x + connector1.width/2
                var y = connector1.parent.y + connector1.y  + connector1.height/2
                ctx.beginPath()
                ctx.moveTo(x ,y)
                ctx.lineTo(mouseX + connector1.width/2,mouseY + connector1.height/2)
                ctx.rect(mouseX,mouseY,connector1.width,connector1.height)
                ctx.stroke()
            }
        }
    }

    function checkTarget(connector) {
        if (wire.connector1 && wire.connector1 !== connector && wire.connector2 !== connector) {
            wire.connector2 = connector
            wire.mouseX = connector.x + connector.parent.x
            wire.mouseY = connector.y + connector.parent.y
            wire.requestPaint()
            // TODO: emit signal for datamodel
        } else {
            wire.connector2 =  wire.connecting ? null : wire.connector2
        }
    }

    function wiring(connector) {
        console.log(connector.name)
        if (wire.connector1) {
            console.log("connectie gemaakt")
            wire.connector1 = null
            wire.connecting = false
            //Code.doConnect(connector)
            wire.requestPaint()
        } else {
            wire.connector1 = connector
            wire.mouseX = connector.x + connector.parent.x
            wire.mouseY = connector.y + connector.parent.y
            wire.connecting = true
            console.log("connectie bezig")
        }

    }

    MouseArea {
        id: area
        anchors.fill: wire
        hoverEnabled: wire.connecting
        acceptedButtons: Qt.LeftButton | Qt.RightButton
        onPressed: {
            if (mouse.button == Qt.RightButton && wire.connecting) {
                wire.connecting = false
                wire.connector1.connected = false
                wire.connector1 = null
                wire.connector2 = null
                wire.requestPaint()
            }
        }
        onPositionChanged: {
            wire.mouseX = mouse.x
            wire.mouseY = mouse.y
            wire.requestPaint()
        }
    }

    Connections {
        target: controller
        onComponentCreate: Ctrl.loadComponent(object) //console.log("component create request")

    }
}

