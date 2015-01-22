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
import "qrc:../content/connectionCreation.js" as Code

//TODO : implement common xmas connection style

Item {
    id: connection
    anchors.fill: parent
    property var connector1: null
    property var connector2: null

    property color color: "darkblue"
    property bool selected: false
    property bool connecting: false

    Canvas {
        id: canvas
        anchors.fill: parent

        //TODO : avoid collition with other objects
        //NOTE : just a straight line for now
        onPaint: {
            var ctx = getContext('2d')
            connection.selected ? ctx.lineWidth = 8.0 : ctx.lineWidth = 4.0
            ctx.strokeStyle = connection.color
            ctx.clearRect(0, 0, canvas.width, canvas.height);
            if (connector1 !== null && connector1.parent !== null)
            {
                ctx.beginPath()
                ctx.moveTo(connector1.parent.x + connector1.x + connector1.width/2 ,
                           connector1.parent.y + connector1.y  + connector1.height/2)
                if (connector2 !== null && connector2.parent !== null)
                {
                    ctx.lineTo(connector2.parent.x + connector2.x + connector2.width/2 ,
                               connector2.parent.y + connector2.y  + connector2.height/2)
                }
                else
                {
                    ctx.lineTo(area.mouseX, area.mouseY)

                }
                ctx.stroke()
            }

        }


        MouseArea {
            id: area
            anchors.fill: parent
            hoverEnabled: connecting
            preventStealing: connecting
            acceptedButtons: Qt.LeftButton | Qt.RightButton
            onPressed: {
                if (mouse.button == Qt.RightButton && connecting) {
                    Code.abortConnecting(connector1)
                    connection.destroy()
                }
            }
            onPositionChanged: {
                canvas.requestPaint()
            }
            onHoveredChanged: {
                canvas.requestPaint()
            }

        }
    }

}

