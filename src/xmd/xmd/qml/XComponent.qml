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
    width: 200
    height: 200
    focus: true
    property int id: 0
    property string type: "unknown"
    property string name: ""
    property alias size: component.scale
    property alias orientation: component.rotation
    orientation: Xmas.North
    property bool selected: false

    //tempory properties (stefan)

    property int rightBound: 10000 //component.parent.width - component.width
    property int bottomBound: 10000 //component.parent.height - component.height

    signal update()
    signal showDialog()


    MouseArea {
        anchors.fill: component
        anchors.margins: 10
        preventStealing: true
        hoverEnabled: false
        acceptedButtons: Qt.LeftButton | Qt.RightButton
        drag.target: component
        drag.minimumX: 0
        drag.minimumY: 0
        drag.maximumX: 10000 ///component.parent.width - component.width
        drag.maximumY: 10000 ///component.parent.height - component.height

        onClicked: {
            if (mouse.button == Qt.LeftButton) {
                selected = !selected
                //scope.focus = !scope.focus
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

    onRotationChanged: component.update()
    onScaleChanged: component.update()
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


    TextInput {
        text: name
        rotation: -parent.rotation
        color: "blue"
        wrapMode: TextInput.NoWrap
        font.pointSize : 16
        anchors.left: parent.left
        anchors.top: parent.top
        onEditingFinished: {name = text; focus = false}
        onFocusChanged: if(focus)selectAll()
    }


    Connections {
        target: parent
        onGroupSelected: component.selected = group.contains(component.x,component.y)
        onDeleteSelected: if (component.selected) Ctrl.destroy(component)
        onClearSelection: component.selected = false
        onMoveSelected: if (component.selected) doMove(dx,dy)
    }

    //TODO : when group move use group boundary!!
    function doMove(dx,dy){
        x = x + dx
        y = y + dy
        if (x < 0) x = 0
        if (y < 0) y = 0
        if (x + width > parent.right) x = parent.right - width
        if (y + height > parent.bottom) y = parent.bottom - height
        component.update()
    }

    Menu {
        id: contextMenu
        MenuItem {
            text: "Properties"
            onTriggered: component.showDialog()
        }
    }


}
