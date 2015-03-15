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
import "../uicontrols"
import "../xmapper/controller.js" as Ctrl

Model.XComponent {
    id: component

    // Properties
    objectName: "component"
    width: 100
    height: box.height
    scale: 1.00
    rotation: 0
    focus: true
    type: Model.XComponent.Unknown
    z:1 //on top of channels
    property int uid:-1
    property string prefix: ""
    property int index:-1
    property bool selected: false
    property bool withDialog: false
    property bool topLabel: true
    property bool boxVisible: false
    property string symbolSource: ""

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

    function doRotate(dAngle){
        component.rotation += (dAngle + 360.0)   // -90 --> 270°
        component.rotation %= 360.0
        if (Math.abs(component.rotation) < 45)
            component.rotation = 0;
        if (Math.abs(component.rotation) > 315)
            component.rotation = 0;
    }


    function leftBound(){
        return sheet.margin + width/2 * (scale-1)
    }
    function topBound(){
        return sheet.margin + height/2 * (scale -1)
    }
    function rightBound(){
        return sheet.width - width * scale  - sheet.margin + width/2 * (scale-1)
    }
    function bottomBound(){
        return sheet.height - height * scale  - sheet.margin + height/2 * (scale-1)
    }

    // Event handling
    onRotationChanged:component.update()
    onScaleChanged: doMove(0,0)
    onWriteLog: console.log(message,color)
    onExpressionChanged: validmarker.color = result === -1 ? "transparent" : "red"

    // Selection highlite
     Rectangle {
         id: highLite
         anchors.fill: mousearea
         anchors.margins: -5
         color:"lightsteelblue"
         border.color: "steelblue"
         border.width: 1
         visible: selected
         z:-1 // under parent = component
      }

    // box with symbol (center)
    Rectangle {
        id: box
        anchors.left: columnLeft.right  //symbol.width
        anchors.right: columnRight.left
        height: boxVisible ? Math.max(portsLeft.count,portsRight.count) * 30 + 20 : component.height
        z:-1
        border.color: "black"
        border.width: boxVisible ? 4 : 0
        radius: 10
        color: boxVisible ? "white" : "transparent"
        Image {
            id:symbol
            fillMode: Image.Stretch
            source: symbolSource
            anchors.centerIn: box
        }
        Text {
            //text: symbol.status === Image.Ready ? "" : type
        }
    }


    // Valid Marker (top left)
    Rectangle {
        id: validmarker
        visible: withDialog
        width: 15
        height: 15
        radius: 15
        color: "red"
        anchors.verticalCenter: nameItem.verticalCenter
        anchors.right: box.left
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
        hoverEnabled:  sheet.isConnecting() ? false : true
        acceptedButtons: Qt.LeftButton | Qt.RightButton
        drag.target: component
        drag.minimumX: leftBound()
        drag.minimumY: topBound()
        drag.maximumX: rightBound()
        drag.maximumY: bottomBound()

        onClicked: {
            if (mouse.button == Qt.LeftButton) {
                sheet.select(component)
            }
            if (mouse.button == Qt.RightButton){
                contextMenu.popup()
            }
        }
        onEntered: {
            if(sheet.isConnecting()){
                component.focus = true
                cursorShape = Qt.OpenHandCursor
            }
        }
        onExited: {component.focus = false; cursorShape = Qt.ArrowCursor}
        onPressed: {if(mouse.button == Qt.LeftButton)cursorShape = Qt.ClosedHandCursor}
        onReleased: {cursorShape = Qt.OpenHandCursor}
        onDoubleClicked: component.showDialog()

        onPositionChanged: {
            component.update()
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
            left: box.left
            right: box.right
            bottom: topLabel ? box.top : undefined
            top: topLabel ? undefined : box.bottom
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
            onEditingFinished: {name = text; focus = false}
            //TODO validate if name is unique
            focus:true
            onFocusChanged: if(focus)selectAll()
            readOnly: false
        }
    }


    // Input ports (left)
    Column {
        id:columnLeft
        spacing: boxVisible ? 20 : 50
        anchors.left: component.left
        anchors.verticalCenter: box.verticalCenter
        Repeater{
            id:portsLeft
            model:component.inputports
            delegate: XPort {
                name:modelData.name
                type:modelData.type
                //anchors.right: columnLeft.left
            }
        }
    }

    // Output ports (right)
    Column {
        id:columnRight
        spacing: boxVisible ? 20 : 50
        anchors.right: component.right
        anchors.verticalCenter: box.verticalCenter
        Repeater{
            id: portsRight
            model:component.outputports
            delegate: XPort {
                name:modelData.name
                type:modelData.type
                //anchors.left: columnRight.right
            }
        }
    }


    // Connections
    Connections {
        target: sheet
        onShowComponentNames: label.visible = checked
        onMoveSelected: if(component.selected) component.update()
    }

    // Context menu
    Menu {
        id: contextMenu
        MenuItem {
            visible: withDialog
            text: "Properties"
            onTriggered: component.showDialog()
        }
        MenuSeparator{visible: withDialog}
        MenuItem {
            text: "Delete"
            onTriggered: Ctrl.destroy(component)
        }
        MenuSeparator{}
        MenuItem {
            text: "Component name"
            checkable: true
            checked: label.visible
            onToggled: label.visible = checked
        }
        MenuSeparator{}
        MenuItem {
            text: "Rotate Right (90°)"
            iconSource: "qrc:/content/rotate_right.png"
            iconName: "RotateRight"
            onTriggered: doRotate(90)
        }
        MenuItem {
            text: "Rotate Left (90°)"
            iconSource: "qrc:/content/rotate_left.png"
            iconName: "RotateLeft"
            onTriggered: doRotate(-90)
        }
        MenuItem {
            text: "Increase Size"
            iconSource: "qrc:/content/bigger.ico"
            iconName: "Bigger"
            onTriggered: doScale(0.25)
        }
        MenuItem {
            text: "Decrease Size"
            iconSource: "qrc:/content/smaller.ico"
            iconName: "Smaller"
            onTriggered: doScale(-0.25)
        }
    }
}
