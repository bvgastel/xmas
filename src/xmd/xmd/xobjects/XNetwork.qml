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
import XMAS 1.0
import "../uicontrols"
import "xchannel.js" as Code
import "../xmapper/controller.js" as Ctrl


Rectangle {
    id: sheet
    scale: 1.0
    width: 5940 //2970
    height: 4200 //2100
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

    function selectAll()
    {
        selection.from = Qt.point(sheet.x,sheet.y)
        selection.to = Qt.point(sheet.width,sheet.height)
        sheet.groupSelected(selection)
    }

    function clear()
    {
        Ctrl.destroyAll(this)
    }

    function clearSelections()
    {
        //propagate to all children
        clearSelection()
    }

    function moveSelection(dx,dy) {
        //propagate to all children
        moveSelected(dx,dy)
    }

    focus: true
    z: -10

    property bool selectionMode: Qt.Unchecked
    property int gridsize: 10
    property int margin: 25

    signal groupSelected(var group)
    signal moveSelected(var dx,var dy)
    signal deleteSelected()
    signal clearSelection()

    Keys.onDeletePressed: deleteSelected()
    Keys.onEscapePressed: clearSelection()
    Keys.onLeftPressed: moveSelected(-gridsize,0)
    Keys.onRightPressed: moveSelected(gridsize,0)
    Keys.onDownPressed: moveSelected(0,gridsize)
    Keys.onUpPressed: moveSelected(0,-gridsize)

    //    Keys.onPressed: { if(event.modifiers=== Qt.ControlModifier) selectionMode = true }
    //    Keys.onReleased: { if(event.modifiers=== Qt.ControlModifier) selectionMode = false }


    //used to show the wiring path when adding a connection
    Canvas {
        id: wire
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
            ctx.lineWidth = 2.0
            ctx.clearRect(0, 0, wire.width, wire.height);
            if (connecting)
            {
                var x = wire.mapFromItem(port1,5,5).x
                var y = wire.mapFromItem(port1,5,5).y
                ctx.beginPath()
                ctx.moveTo(x ,y)
                ctx.lineTo(mouseX,mouseY)
                ctx.stroke()
                if(port1.type===Xmas.Target){
                    ctx.moveTo(mouseX + port1.width/2 ,mouseY + port1.height/2)
                    ctx.beginPath()
                    ctx.arc(mouseX,mouseY, port1.width/2, 0, 2*Math.PI, false)
                    ctx.stroke()
                } else {
                    ctx.beginPath()
                    ctx.rect(mouseX - port1.width/2,mouseY - port1.height/2,port1.width,port1.height)
                    ctx.stroke()
                }
                ctx.stroke()

            }
        }
    }

    function checkTarget(port) {
        if (wire.port1
                && wire.port1 !== port
                && wire.port2 !== port) {
            if (wire.port1.type !== port.type){
                wire.port2 = port
                wire.mouseX = wire.mapFromItem(port,5,5).x
                wire.mouseY = wire.mapFromItem(port,5,5).y
            }
            wire.requestPaint()
        } else {
            wire.port2 =  wire.connecting ? null : wire.port2
        }
    }

    function wiring(port) {
        if (wire.port1) {
            if(wire.port1.type !== port.type)
            {
                Code.doConnect(wire.port1,port)
                Code.channel = null
                wire.port1 = null
                wire.port2 = null
                wire.connecting = false

                wire.requestPaint() }
            else { port.connected=false}
        } else {
            wire.port1 = port
            wire.mouseX = port.x
            wire.mouseY = port.y
            wire.connecting = true
        }
    }

    function isConnecting() {
        return wire.connecting
    }

    function boundReached(dx,dy){
        console.log("boundReached")
    }

    function isValidPort(port){
        if(wire.port1){
            return wire.port1.type !== port.type
        }
        return true
    }



    Selection {id: selection}


    MouseArea {
        id: area
        anchors.fill: sheet
        hoverEnabled: wire.connecting
        acceptedButtons: Qt.LeftButton | Qt.RightButton
        onPressed: {
            if (mouse.button == Qt.RightButton)
            {
                if(wire.connecting)
                {
                    wire.connecting = false
                    wire.port1.connected = false
                    wire.port1 = null
                    wire.port2 = null
                    wire.requestPaint()
                }
                else
                {
                    contextMenu.popup()
                }
            }
            if (mouse.button == Qt.LeftButton){

                if(sheet.selectionMode){
                    sheet.clearSelection()
                    selection.from = Qt.point(mouse.x,mouse.y)
                    selection.to = Qt.point(mouse.x,mouse.y)
                    selection.visible = true
                    focus = true
                }
            }
        }
        onReleased: {
            if (mouse.button == Qt.LeftButton){
                if(sheet.selectionMode){
                    sheet.groupSelected(selection)
                    selection.from = Qt.point(0,0)
                    selection.to = Qt.point(0,0)
                    selection.visible = false
                }
            }
        }

        onClicked: {
            if (mouse.button == Qt.LeftButton) {
                if(!sheet.selectionMode) sheet.clearSelection()
                focus = true
            }
            if (mouse.button == Qt.RightButton) {

            }
        }

        onPositionChanged: {
            if(sheet.selectionMode) selection.to = Qt.point(mouse.x,mouse.y)
            wire.mouseX = mouse.x
            wire.mouseY = mouse.y
            wire.requestPaint()
        }
    }

    Connections {
        target: controller
        onCreateComponent: Ctrl.loadComponent(object)
        onCreateNetwork: Ctrl.createNetwork(object)
    }

    Menu {
        id: contextMenu
        MenuItem {
            text: "Delete"
            onTriggered: deleteSelected()
        }
    }
}
