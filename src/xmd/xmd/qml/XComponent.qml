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
import QtQuick.Dialogs 1.2
import QtQuick.Window 2.2
import XMAS 1.0
import "../controller.js" as Ctrl

Item {
    id: component
    objectName: "component"
    width: 100
    height: 100
    focus: true
    property int id: 0
    property string type: "unknown"
    property string name: ""
    property alias size: component.scale
    property alias orientation: component.rotation
    orientation: Xmas.North
    property bool selected: false
    scale: 1.00
    property bool withDialog: false
    property bool topLabel: true

    signal update()
    signal showDialog()
    signal boundReached(var dx, var dy)

    //name tag
    TextInput {
        id:label
        text: name
        rotation: Math.abs(parent.rotation) > 135 && Math.abs(parent.rotation) < 225 ? -parent.rotation : 0
        color: "blue"
        wrapMode: TextInput.NoWrap
        anchors.bottomMargin: 2
        font.pointSize : 12
        anchors {
            left: parent.left
            bottom: topLabel ? parent.top : undefined
            top: topLabel ? undefined : parent.bottom
        }
        //anchors.bottom: parent.top
        onEditingFinished: {name = text; focus = false}
        onFocusChanged: if(focus)selectAll()
    }



    MouseArea {
        anchors.fill: component
        anchors.margins: 5
        preventStealing: true
        hoverEnabled: false
        acceptedButtons: Qt.LeftButton | Qt.RightButton
        drag.target: component
        drag.minimumX: leftBound()
        drag.minimumY: topBound()
        //TODO : replace with sheet bounds
        drag.maximumX: rightBound()
        drag.maximumY: bottomBound()

        onClicked: {
            if (mouse.button == Qt.LeftButton) {
                selected = !selected
            }
            if (mouse.button == Qt.RightButton){
                contextMenu.popup()
            }
        }
        onDoubleClicked: component.showDialog()

        onPositionChanged: component.update()
        //onExited: selection.visible = false

        onWheel: {
            if (wheel.modifiers & Qt.ControlModifier) {
                component.rotation -= wheel.angleDelta.y /120 * 90;
                component.rotation %= 360.0
                if (Math.abs(component.rotation) < 45)
                    component.rotation = 0;
                if (Math.abs(component.rotation) > 315)
                    component.rotation = 0;
            }
            if (wheel.modifiers & Qt.AltModifier) {
                component.scale -= wheel.angleDelta.x /120 * 0.25;
                if (Math.abs(component.scale) < 0.25)
                    component.scale = 0.25;
                if (Math.abs(component.scale) > 2.0)
                    component.scale = 2.0;
            }
        }
    }

    onRotationChanged:component.update()
    onScaleChanged: doMove(0,0)
    onSelectedChanged: focus = selected


    Rectangle {
        id: selection
        anchors.fill: component
        anchors.margins: -2
        color: "lightsteelblue"
        border.color: "steelblue"
        border.width: 1
        visible: selected
        opacity: 0.5
        z:-1
    }

    Connections {
        target: parent
        onGroupSelected: component.selected = group.contains(component.x,component.y)
        onDeleteSelected: if (component.selected) Ctrl.destroy(component)
        onClearSelection: component.selected = false
        onMoveSelected: if (component.selected) doMove(dx,dy)
    }


    function doMove(dx,dy){
        x = x + dx
        y = y + dy
        if (x < leftBound())
        {
            boundReached(leftBound()-x,0)
            x = leftBound()
        }
        if (y < topBound())
        {
            boundReached(0,topBound()-y)
            y = topBound()
        }
        if (x > rightBound())
        {
            boundReached(x-rightBound(),0)
            x = rightBound()
        }
        if (y > bottomBound())
        {
            boundReached(0,y-bottomBound())
            y = bottomBound()
        }
        component.update()
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
    }


}
