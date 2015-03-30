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

XComponent {
    id:component
    width: boxed ? 200 : 100
    height: Math.max(portsLeft.count,portsRight.count) * (portSpace + 10) + 20
    type: Model.XComponent.Composite
    prefix: "C"
    property string url:""
    property string alias:"Composite"
    property string image:""
    property bool boxed: true
    property int portSpace: 20

    //TEST ports

//    onPortsChanged: getPorts()
    property var inports:[]
    property var outports:[]
    function getPorts(){
        for (var child in component.ports) {
            var chld = ports[child]
            console.log("child " + chld)
            if(chld.type===Model.XPort.INPORT) {
                inports.pop(chld)
            }
            if(chld.type===Model.XPort.OUTPORT) {
                outports.pop(chld)
            }
        }
    }

    Rectangle {
        id: box
        anchors.fill: parent
        anchors.leftMargin: boxed ? 18 : 0
        anchors.rightMargin: boxed ? 18 : 0
        z:-1
        border.color: "black"
        border.width: boxed ? 4 : 0
        radius: 10
        color: boxed ? "white" : "transparent"
        Image {
            id:symbol
            //fillMode: Image.Stretch
            height: boxed ? component.height - 50 : 100
            width: 100
            fillMode: Image.PreserveAspectFit
            source: image
            anchors.centerIn: parent
            onStatusChanged: if(symbol.status === Image.Error) boxed = true
            Text {
                text: symbol.status === Image.Error ? ("???\n" + alias) : alias
                visible: boxed
                anchors.centerIn: image ? undefined : parent
                anchors.top: image ? symbol.bottom : undefined
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
        }
    }


    // Input ports (left)
    Column {
        id:columnLeft
        spacing: portSpace
        anchors.left: component.left
        anchors.verticalCenter: box.verticalCenter
        Repeater{
            id:portsLeft
            model:inports
            delegate: XPort {
                name:modelData.name
                type:modelData.type
                nameAlignCenter: boxed
                Rectangle {
                    id:leftWire
                    color:"black"
                    z:-1
                    border.width: 0
                    height: 4
                    width: 15
                    visible: boxed
                    anchors.left: parent.horizontalCenter
                    anchors.verticalCenter: parent.verticalCenter
                }
            }
        }
    }

    // Output ports (right)
    Column {
        id:columnRight
        spacing: portSpace
        anchors.right: component.right
        anchors.verticalCenter: box.verticalCenter
        Repeater{
            id: portsRight
            model:outports
            delegate: XPort {
                name:modelData.name
                type:modelData.type
                nameAlignCenter: boxed
                Rectangle {
                    id:rightWire
                    color:"black"
                    z:-1
                    border.width: 0
                    height: 4
                    width: 15
                    visible: boxed
                    anchors.left: parent.horizontalCenter
                    anchors.verticalCenter: parent.verticalCenter
                }
            }
        }
    }
}
