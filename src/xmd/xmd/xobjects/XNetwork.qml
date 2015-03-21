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
import "../uicontrols"
import "../xobjects/xchannel.js" as Code
import "../xmapper/controller.js" as Ctrl
import XMAS.model 1.0 as XMAS


XMAS.XNetwork {
    id: network
    // Properties
    scale: 1.0
    width: 5940 //2970
    height: 4200 //2100
    property bool selectionMode: Qt.Unchecked
    property int gridsize: 10
    property int margin: 25
    property string color: "white"

    // Signals
    signal moveSelected(var group)
    signal showComponentNames(var checked)

    // Event handling

    // JavaScripts

    // Scale
    function doScale(dScale){
        if(dScale===undefined)
            scale = 1.0
        else
            scale += dScale
        if (scale > 2) scale = 2
        if (scale < 0.2) scale = 0.2
    }

    // Select item
    function select(item){
        selection.select([item])
    }

    // Select all items
    function selectAll() {
        selection.select(canvasItems())
    }

    // Clear network
    function clear(){
        selectAll()
        selection.deleteSelected()
    }

    function checkTarget(port) {
        if (wire.outport
                && wire.outport !== port
                && wire.inport !== port) {
            if (wire.outport.type !== port.type){
                wire.inport = port
            }
        } else {
            wire.inport =  wire.connecting ? null : wire.inport
        }
    }

    function wiring(port) {
        if (wire.outport) {
            if(wire.outport.type !== port.type)
            {
                Code.doConnect(wire.outport,port)
                Code.channel = null
                wire.outport = null
                wire.inport = null
                wire.connecting = false
            }
            else { port.connected=false}
        } else {
            wire.outport = port
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
        if(wire.outport){
            return wire.outport.type !== port.type
        }
        return true
    }

    function canvasItems(){
        var children = network.children
        var items = []
        for (var child in children){
            if(children[child].objectName==="component")
                // || children[child].objectName==="channel")
                items.push(children[child])
        }
        return items
    }

    Rectangle {
        id:background
        anchors.fill: parent
        color:parent.color
    }

    //used to show the wiring path when adding a connection
    XWire{
        id: wire
        property bool connecting: false
        property var outport: null
        property var inport: null
        property int mx: 0
        property int my: 0
        visible: connecting
        size: 4
        color: "red"

        onOutportChanged: {
            if(outport) {
                wire.x1 = outport.mapToItem(network,5,5).x
                wire.y1 = outport.mapToItem(network,5,5).y
                wire.x2 = wire.x1
                wire.y2 = wire.y1
            }
        }
        onInportChanged: {
            if(inport) {
                wire.x2 = inport.mapToItem(network,5,5).x
                wire.y2 = inport.mapToItem(network,5,5).y
            }
        }
        onVisibleChanged: {
            if(!visible){
                wire.x1 = 0
                wire.y1 = 0
                wire.x2 = 0
                wire.y2 = 0
            }
        }

        Rectangle {
            id: endcap
            color: "white"
            x: wire.width - 5
            y: -5 + wire.size / 2
            height: 10
            width: 10
            rotation: -parent.rotation
            radius: {
                if(wire.outport) {
                    wire.outport.type === XMAS.XPort.Target ? 10  : 0
                } else 0
            }
            border.color: wire.color
            border.width: 2
        }
    }

    Selection {
        id: selection
        onSizeChanged: selection.find(canvasItems())
        onShowContextMenu:contextMenu.popup()
        onPositionChanged: moveSelected(selection)
    }

    MouseArea {
        id: area
        anchors.fill: parent
        hoverEnabled: wire.connecting
        acceptedButtons: Qt.LeftButton | Qt.RightButton
        onPressed: {
            if (mouse.button == Qt.RightButton)
            {
                if(wire.connecting)
                {
                    wire.connecting = false
                    wire.outport = null
                    wire.inport = null
                }
                else
                {
                    contextMenu.popup()
                }
            }
            if (mouse.button == Qt.LeftButton){
                if(network.selectionMode){
                    selection.start(mouse)
                }
            }
        }
        onReleased: {
            if (mouse.button == Qt.LeftButton){
                if(network.selectionMode){
                    selection.find(canvasItems())
                }
            }
        }

        onClicked: {
            if (mouse.button == Qt.LeftButton) {
                if(!network.selectionMode) {
                    selection.clear()
                }
            }
            if (mouse.button == Qt.RightButton) {

            }
        }

        onPositionChanged: {
            if(network.selectionMode) selection.updateInitialSize(mouse)
            wire.x2 = mouse.x
            wire.y2 = mouse.y
        }
    }

    // Connections
    Connections {
        target: datacontrol
        //onCreateComponent: Ctrl.loadComponent(object)
        onCreateNetwork: Ctrl.createNetwork(object)
    }

    Connections {
        target: mainwindow
        onZoomIn: doScale(0.1)
        onZoomOut: doScale(-0.1)
        onZoomFit: doScale()
        onSelectAll: selectAll()
        onSelectionMode: selectionMode = checked
        onShowComponentNames: showComponentNames(checked)
    }
}

