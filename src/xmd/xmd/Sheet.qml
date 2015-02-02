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
    scale: 1.0
    width: 2970
    height: 2100
    color: "white"

    function zoomFit()
    {
        scale = 1.0;
    }
    function zoomIn()
    {
        scale = scale + 0.1;
        if (scale > 2) scale = 2
    }
    function zoomOut()
    {
        scale = scale - 0.1;
        if (scale < 0.2) scale = 0.2
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
        property var port1: null
        property var port2: null

        visible: connecting
        enabled: connecting
        onPaint: {
            var ctx = getContext('2d')
            ctx.strokeStyle = "darkblue"
            ctx.lineWidth = 4.0
            ctx.clearRect(0, 0, wire.width, wire.height);
            if (connecting)
            {
                var x =  port1.parent.x + port1.x + port1.width/2
                var y = port1.parent.y + port1.y  + port1.height/2
                ctx.beginPath()
                ctx.moveTo(x ,y)
                ctx.lineTo(mouseX + port1.width/2,mouseY + port1.height/2)
                ctx.rect(mouseX,mouseY,port1.width,port1.height)
                ctx.stroke()
            }
        }
    }

    function checkTarget(port) {
        if (wire.port1 && wire.port1 !== port && wire.port2 !== port) {
            wire.port2 = port
            wire.mouseX = port.x + port.parent.x
            wire.mouseY = port.y + port.parent.y
            wire.requestPaint()
            // TODO: emit signal for datamodel
        } else {
            wire.port2 =  wire.connecting ? null : wire.port2
        }
    }

    function wiring(port) {
        console.log(port.name)
        if (wire.port1) {
            console.log("connectie gemaakt")
            Code.doConnect(wire.port1,port)
            Code.channel = null
            wire.port1 = null
            wire.port2 = null
            wire.connecting = false
            wire.requestPaint()
        } else {
            wire.port1 = port
            wire.mouseX = port.x + port.parent.x
            wire.mouseY = port.y + port.parent.y
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
                wire.port1.connected = false
                wire.port1 = null
                wire.port2 = null
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

