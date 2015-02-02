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
import "../controller.js" as Ctrl

Item {
    id: component
    objectName: "component"
    width: 200
    height: 200
    focus: true
    //TODO : scale property: each componentonent can be zoomed seperately
    property bool created: false
    property bool selected: false
    property string type: "default"
    property string name: "c?"

    // orientation = rotation property
    // zoom = scale property


    //tempory properties (stefan)
    property var sheet: parent
    property int rightBound: 10000 //component.parent.width - component.width
    property int bottomBound: 10000 //component.parent.height - component.height
    property int step: 10

    signal update()

    Keys.onDeletePressed: {Ctrl.destroy(component)}
    Keys.onLeftPressed: x - step < 0 ? x = 0 : x = x - step
    Keys.onRightPressed: x + step > rightBound ? x = rightBound : x = x + step
    Keys.onDownPressed: y + step > bottomBound ? y = bottomBound : y = y + step
    Keys.onUpPressed: y - step < 0 ? y = 0 : y = y - step

    MouseArea {
        anchors.fill: component
        anchors.margins: 10
        hoverEnabled: false
        acceptedButtons: Qt.LeftButton | Qt.RightButton
        drag.target: component
        drag.minimumX: 0
        drag.minimumY: 0
        drag.maximumX: 10000 ///component.parent.width - component.width
        drag.maximumY: 10000 ///component.parent.height - component.height

        onClicked: {
            selected = !selected
            scope.focus = !scope.focus
        }

        onPositionChanged: component.update()
        //onExited: selection.visible = false

        onWheel: {
            if (wheel.modifiers & Qt.ControlModifier) {
                component.rotation -= wheel.angleDelta.y /120 * 90;
                if (Math.abs(component.rotation) < 45)
                    component.rotation = 0;
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

    FocusScope {
        id: scope
        x: component.x; y: component.y
        width: component.width; height: component.height
    }
}
