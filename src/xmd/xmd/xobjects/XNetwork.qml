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
 * Parts of this ChannelJs were copied from Qt examples with following copyright
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
import "qrc:/ui/uicontrols/"
import "qrc:/javascripts/xobjects/xchannel.js" as ChannelJs
import "qrc:/javascripts/xobjects/xnetwork.js" as NetworkJs
import XMAS.model 1.0 as Model
import XMAS 1.0 as XMAS


Model.XNetwork {
    id: network
    // Properties
    scale: 1.0
    size: Qt.size(5940,4200)
    width: size.width
    height: size.height
    transformOrigin: Item.TopLeft
    alias:""
    boxedImage: true
    imageName: ""
    property string fileName: "?.json"
    property string folder: modelFolder
    property bool selectionMode: Qt.Unchecked
    property int gridsize: 10
    property int margin: 25
    property string color: "white"
    property bool modified: false

    // Signals
    signal moveSelected(var group)

    // Event handling
    onChildrenChanged: modified=true
    onWriteLog: log(XMAS.Util.Designer,message, color)
    onPacketChanged: modified=true

    // JavaScripts

    function url(){
       return network.folder + "/" + network.fileName
    }

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
        network.fileName = "?.json"
        network.folder = modelFolder
        network.alias = ""
        network.imageName = ""
        network.boxedImage = true
        network.modified = false
    }


    function checkTarget(port) {
        if (wire.port1
                && wire.port1 !== port
                && wire.port2 !== port) {
            if (wire.port1.type !== port.type){
                wire.port2 = port
            }
        } else {
            wire.port2 =  wire.connecting ? null : wire.port2
        }
    }

    function wiring(port) {
        if (wire.port1) {
            if(wire.port1.type !== port.type)
            {
                ChannelJs.doConnect(wire.port1,port)
                ChannelJs.channel = null
                wire.port1 = null
                wire.port2 = null
                wire.connecting = false
            }
            //else { port.connected=false}
        } else {
            wire.port1 = port
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

    function canvasItems(){
        var children = network.children
        var items = []
        for (var child in children){
            if(children[child].objectName==="component") // || children[child].objectName==="channel")
                items.push(children[child])
        }
        return items
    }

    function deleteSelected(){
        selection.deleteSelected()
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
        property var port1: null
        property var port2: null
        property int mx: 0
        property int my: 0
        visible: connecting
        size: 4
        color: "red"

        onPort1Changed: {
            if(port1) {
                wire.x1 = port1.mapToItem(network,5,5).x
                wire.y1 = port1.mapToItem(network,5,5).y
                wire.x2 = wire.x1
                wire.y2 = wire.y1
            }
        }
        onPort2Changed: {
            if(port2) {
                wire.x2 = port2.mapToItem(network,5,5).x
                wire.y2 = port2.mapToItem(network,5,5).y
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
                if(wire.port1) {
                    wire.port1.type === Model.XPort.Target ? 10  : 0
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
                    wire.port1 = null
                    wire.port2 = null
                } else {
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

    Menu {
        id: contextMenu
        MenuItem {
            text: "Delete"
            onTriggered: selection.deleteSelected()
        }
        MenuSeparator{}
        MenuItem {
            action: showComponentNamesAction
        }
        MenuItem {
            action: showPortNamesAction
        }
        MenuSeparator{}
        MenuItem {
            action: modelSetupAction
        }
    }


    ModelSetupDialog {
        id:setupDialog
        onSetupAccepted: mainwindow.saveModel()
    }

    // Connections
    Connections {
        target: datacontrol
        onCreateNetwork: NetworkJs.createNetwork(object)
    }

    Connections {
        target: mainwindow
        onZoomIn: doScale(0.1)
        onZoomOut: doScale(-0.1)
        onZoomFit: doScale()
        onSelectAll: selectAll()
        onSelectionMode: selectionMode = checked
        onModelSetupDialog: setupDialog.show()
    }
}

