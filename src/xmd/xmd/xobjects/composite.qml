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
import QtQuick.Layouts 1.1
import XMAS.model 1.0 as Model
import XMAS 1.0 as XMAS

XComponent {
    id:component

    // Properties
    width: boxed ? 200 : 100
    height: Math.max(portsLeft.count,portsRight.count) * (portSpace) + 30
    type: Model.XComponent.Composite
    prefix: "C"
    property string filename:""
    property string alias:"Composite"
    property string image:""
    property bool boxed: true
    property int portSpace: 30 //2 times default gridsize - 1 time portsize

    // Javascript
    function updatePorts() {
        try {
            var ports = getPorts()
            inportModel.clear()
            outportModel.clear()
            for (var key in ports) {
                if(ports[key]===Model.XPort.INPORT) {
                    inportModel.append({portName: key, portType:ports[key]})
                }
                if(ports[key]===Model.XPort.OUTPORT) {
                    outportModel.append({portName: key, portType:ports[key]})
                }
            }
        } catch(e){
            writeLog("[composite.qml - ]" + e, Qt.red)
        }
    }

    // Event handling
    onComponentAdded: updatePorts()

    // Content
    RowLayout{
        anchors.fill: parent

        // Input ports (left)
        Rectangle{
            id:columnLeft
            Layout.minimumWidth: 15
            Layout.maximumWidth: 15
            Layout.fillHeight: true
            color:"transparent"
            z:-1
            Repeater{
                id:portsLeft
                model:inportModel
                delegate: portDelegate
            }
        }

        Rectangle {
            id: box
            Layout.fillHeight: true
            Layout.fillWidth: true
            border.color: "black"
            border.width: boxed ? 4 : 0
            radius: 10
            color: boxed ? "white" : "transparent"
            z:-2
            Image {
                id:symbol
                height: boxed ? component.height - 50 : 100
                width: 100
                fillMode: Image.PreserveAspectFit
                source: image // ? "qrc:/symbols/content/symbols/" + image : ""
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

        // Output ports (right)
        Rectangle{
            id:columnRight
            Layout.minimumWidth: 15
            Layout.maximumWidth: 15
            Layout.fillHeight: true
            color:"transparent"
            z:-1
            Repeater{
                id: portsRight
                model:outportModel
                delegate: portDelegate
            }
        }
    }

    ListModel {
        id: inportModel
    }

    ListModel {
        id: outportModel
    }

    Component {
        id: portDelegate
        XPort {
            id:port
            name:portName
            type:parseInt(portType)
            nameAlignCenter: boxed
            x:type===Model.XPort.INPORT ? 5 : 10
            y:30 + index * portSpace
            Rectangle {
                id:portWire
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
