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
import QtQuick.Layouts 1.1
import QtQuick.Dialogs 1.2
import QtQuick.Window 2.2
import XMAS.model 1.0 as Model
import XMAS 1.0 as XMAS
import "qrc:/javascripts/xobjects/xcomponent.js" as ComponentJs
import "qrc:/ui/uicontrols/"

Model.XComponent {
    id: component

    // Properties
    objectName: "component"
    width: 100
    height: 100
    scale: 1.00
    rotation: 0
    focus: true
    type: Model.XComponent.Unknown
    z:1 //on top of channels
    property string prefix: ""
    property bool selected: false
    property bool withValidMarker: false
    property bool topLabel: true
    property bool positionHasChanged: true

    // Signals
    signal update()
    signal showDialog()

    // JavaScripts

    // Move
    function doMove(dX,dY){
        component.x += dX
        component.y += dY
        if (component.x < leftBound())
            component.x = leftBound()
        if (component.y < topBound())
            component.y = topBound()
        if (component.x > rightBound())
            component.x = rightBound()
        if (component.y > bottomBound())
            component.y = bottomBound()
        component.update()
    }

    // Scale
    function doScale(dScale){
        component.scale += dScale
        if (Math.abs(component.scale) < 0.25)
            component.scale = 0.25;
        if (Math.abs(component.scale) > 4.0)
            component.scale = 4.0;
    }

    // Rotate
    function doRotate(dAngle){
        component.rotation += (dAngle + 360.0)   // -90 --> 270°
        component.rotation %= 360.0
        if (Math.abs(component.rotation) < 45)
            component.rotation = 0;
        if (Math.abs(component.rotation) > 315)
            component.rotation = 0;
    }

    function leftBound(){
        return network.margin + width/2 * (scale-1)
    }
    function topBound(){
        return network.margin + height/2 * (scale -1)
    }
    function rightBound(){
        return network.width - width * scale  - network.margin + width/2 * (scale-1)
    }
    function bottomBound(){
        return network.height - height * scale  - network.margin + height/2 * (scale-1)
    }
    function insertMenuItem(item) {
        contextMenu.insertItem(0,item)
    }
    function insertMenuSeparator() {
        contextMenu.insertSeparator(0)
    }

    // remove this
    function remove(){
        ComponentJs.remove(component)
    }

    // Event handling
    onRotationChanged:{component.update();component.updateCanvasData();network.modified=true}
    onScaleChanged: {doMove(0,0);component.updateCanvasData();network.modified=true}
    onWriteLog: mainwindow.log(message,color)
    onSelectedChanged: if(!selected) label.focus = false
    onXChanged: {positionHasChanged = true;network.modified=true}
    onYChanged: {positionHasChanged = true;network.modified=true}

    // Selection highlite
    Rectangle {
        id: highLite
        anchors.fill: mousearea
        anchors.margins: -5
        color:"lightsteelblue"
        border.color: "steelblue"
        border.width: 1
        opacity: 0.5
        visible: selected
    }

    // Expression valid Marker (top left)
    Rectangle {
        id: validmarker
        visible: withValidMarker
        width: 15
        height: 15
        radius: 15
        color: expressionValid ? "green" : "red"
        anchors.verticalCenter: nameItem.verticalCenter
        anchors.right: component.left
        MouseArea {
            anchors.fill: parent
            acceptedButtons: Qt.LeftButton
            onDoubleClicked: component.showDialog()
        }
    }


    // Mouse area
    MouseArea {
        id: mousearea
        anchors.fill: component
        preventStealing: true
        hoverEnabled:  network.isConnecting() ? false : true
        acceptedButtons: Qt.LeftButton | Qt.RightButton
        drag.target: component
        drag.minimumX: leftBound()
        drag.minimumY: topBound()
        drag.maximumX: rightBound()
        drag.maximumY: bottomBound()

        onClicked: {
            if (mouse.button == Qt.LeftButton) {
                network.select(component,
                               mouse.modifiers === Qt.ControlModifier)
            }
            if (mouse.button == Qt.RightButton){
                contextMenu.popup()
            }
        }
        onEntered: {
            if(network.isConnecting()){
                component.focus = true
                cursorShape = Qt.OpenHandCursor
            }
        }
        onExited: {component.focus = false; cursorShape = Qt.ArrowCursor}
        onPressed: {if(mouse.button == Qt.LeftButton)cursorShape = Qt.ClosedHandCursor}
        onReleased: {
            cursorShape = Qt.OpenHandCursor
            if(positionHasChanged){
                component.updateCanvasData()
                positionHasChanged = false
            }

        }
        onDoubleClicked: component.showDialog()

        onPositionChanged: {
            if(drag.active) {
                if(network.gridSnap){
                    ComponentJs.doGridSnap(component)
                }
                component.update()
            }
        }

        onWheel: {
            if (wheel.modifiers & Qt.ControlModifier)
                doRotate(-wheel.angleDelta.y /120 * 90)
            if (wheel.modifiers & Qt.AltModifier)
                doScale(-wheel.angleDelta.x /120 * 0.25)
        }
    }

    // Items on top of component mousearea

    // Name (top center)
    Rectangle{
        id:nameItem
        rotation: Math.abs(parent.rotation) > 135 && Math.abs(parent.rotation) < 225 ? -parent.rotation : 0
        anchors {
            left: component.left
            right: component.right
            bottom: topLabel ? component.top : undefined
            top: topLabel ? undefined : component.bottom
        }
        color:"transparent"
        border.color: "red"
        border.width: component.name === "" ? 1 : 0
        anchors.margins: 2
        height: label.contentHeight

        TextInput {
            id:label
            text: name
            anchors.fill: nameItem
            horizontalAlignment: Qt.AlignHCenter
            verticalAlignment: Qt.AlignVCenter
            color: "blue"
            wrapMode: TextInput.NoWrap
            font.pointSize : 12
            onEditingFinished: {name = text;network.modified=true}
            onAccepted: {focus = false;network.modified=true}
            focus:true
            onFocusChanged: if(focus)selectAll()
            readOnly: false
            visible: mainwindow.showComponentNames
        }
    }

    // Connections
    Connections {
        target: network
        onMoveSelected: if(component.selected) component.update()
    }

    // Context menu
    Menu {
        id: contextMenu
        MenuItem {
            text: "Delete"
            onTriggered: remove()
        }
        MenuSeparator{}
        MenuItem {
            text: "Rotate Right (90°)"
            iconSource: "qrc:/icons/content/rotate_right.png"
            iconName: "RotateRight"
            onTriggered: doRotate(90)
        }
        MenuItem {
            text: "Rotate Left (90°)"
            iconSource: "qrc:/icons/content/rotate_left.png"
            iconName: "RotateLeft"
            onTriggered: doRotate(-90)
        }
        MenuItem {
            text: "Increase Size"
            iconSource: "qrc:/icons/content/bigger.ico"
            iconName: "Bigger"
            onTriggered: doScale(0.5)
        }
        MenuItem {
            text: "Decrease Size"
            iconSource: "qrc:/icons/content/smaller.ico"
            iconName: "Smaller"
            onTriggered: doScale(-0.5)
        }
    }

}
