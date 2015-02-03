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


    //tempory properties (stefan)
    property bool selected: false
    property int rightBound: 10000 //component.parent.width - component.width
    property int bottomBound: 10000 //component.parent.height - component.height
    property int step: 10

    signal update()

    Keys.onDeletePressed: {Ctrl.destroy(component)}
    Keys.onLeftPressed: x - step < 0 ? x = 0 : x = x - step
    Keys.onRightPressed: x + step > rightBound ? x = rightBound : x = x + step
    Keys.onDownPressed: y + step > bottomBound ? y = bottomBound : y = y + step
    Keys.onUpPressed: y - step < 0 ? y = 0 : y = y - step

    TextInput {
        text: component.name
        rotation: -parent.rotation
        cursorVisible: false
        maximumLength: 10
        wrapMode: TextInput.NoWrap
        //font.pointSize : 8 / parent.scale
    }

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
                scope.focus = !scope.focus
            }
             if (mouse.button == Qt.RightButton){
                 contextMenu.popup()
             }
        }

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

    Rectangle {
        id: selection
        anchors.fill: component
        anchors.margins: -2
        color: "lightsteelblue"
        border.color: "blue"
        border.width: 2
        visible: scope.focus
        //opacity: 0.5
        z:-1
    }



//    Dialog {
//        visible: true
//        title: "Blue sky dialog"

//        contentItem: Rectangle {
//            color: "lightskyblue"
//            implicitWidth: 400
//            implicitHeight: 100
//            Text {
//                text: "Hello blue sky!"
//                color: "navy"
//                anchors.centerIn: parent
//            }
//        }
//    }

    Menu {
        id: contextMenu
        title: "Component"
        MenuItem {
            text: "Test"
            shortcut: "Ctrl+X"
            //onTriggered: ...
        }
    }

    FocusScope {
        id: scope
        x: component.x; y: component.y
        width: component.width; height: component.height
    }
}
