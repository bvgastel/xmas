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

import QtQuick 2.0

MouseArea {
    id: handle
    width: 10
    height: 10
    property int cursor: Qt.ArrowCursor
    property int axis: Drag.XAndYAxis

    hoverEnabled: true
    preventStealing: true
    acceptedButtons: Qt.LeftButton | Qt.RightButton
    drag.axis: axis
    onEntered: cursorShape = cursor
    onExited: cursorShape = Qt.ArrowCursor
    Rectangle {
        anchors.fill: parent
        color:"white"
        border.color:"steelblue"
        border.width: 1
    }
}
