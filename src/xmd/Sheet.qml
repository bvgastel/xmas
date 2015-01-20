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
import QtQuick.Layouts 1.0
import QtQuick.Dialogs 1.1
import QtQuick.Window 2.1
import QtGraphicalEffects 1.0
import "content"

Item {
    property int centerOffset: 0
    property real zoomFactor: 0.25

    function zoomFit()
    {
        zoomFactor = 1.0;
    }
    function zoomIn()
    {
        zoomFactor = zoomFactor + 0.1;
        if (zoomFactor > 2) zoomFactor = 2
    }
    function zoomOut()
    {
        zoomFactor = zoomFactor - 0.1;
        if (zoomFactor < 0.1) zoomFactor = 0.1
    }

    focus: true
    z: -10
    // Create a flickable to view a large drawing.
    Flickable {
        id: view
        anchors { top: toolbar.bottom ; bottom: parent.bottom; left: parent.left; right: parent.right}

        //center the scene by default
        contentX: (1 - scene.scale) * scene.width * 0.5
        contentY: (1 - scene.scale) * scene.height * 0.5
        contentWidth: scene.width
        contentHeight: scene.height

        Rectangle {
            id: scene
            scale: zoomFactor
            width: 11880
            height: 8400
            color: "white"
            opacity: 50
            }

        // Only show the scrollbars when the view is moving.
        states: State {
            name: "ShowBars"
            when: view.movingVertically || view.movingHorizontally
            PropertyChanges { target: verticalScrollBar; opacity: 1 }
            PropertyChanges { target: horizontalScrollBar; opacity: 1 }
        }

        transitions: Transition {
            NumberAnimation { properties: "opacity"; duration: 600 }
        }

    }

    XmasToolBar{
        id: toolbar
        height:48
        anchors {right: parent.right; top: parent.top; left: parent.left}
    }

    DropShadow {
           anchors.fill: view
           horizontalOffset: 3
           verticalOffset: 3
           radius: 8.0
           samples: 16
           color: "#80000000"
           source: view
       }



//    Canvas {
//            id: canvas
//            anchors.fill: view

//            property real lastX
//            property real lastY
//            property color color: "black"

//            onPaint: {
//                var ctx = getContext('2d')
//                ctx.lineWidth = 1.5
//                ctx.strokeStyle = canvas.color
//                ctx.beginPath()
//                ctx.moveTo(lastX, lastY)
//                lastX = area.mouseX
//                lastY = area.mouseY
//                ctx.lineTo(lastX, lastY)
//                ctx.stroke()
//            }
//            MouseArea {
//                id: area
//                anchors.fill: parent
//                onPressed: {
//                    canvas.lastX = mouseX
//                    canvas.lastY = mouseY
//                }
//                onPositionChanged: {
//                    canvas.requestPaint()
//                }
//            }
//        }



    // Attach scrollbars to the right and bottom edges of the view.
      ScrollBar {
          id: verticalScrollBar
          width: 12; height: view.height-12 - toolbar.height
          anchors.right: view.right
          anchors.top:  toolbar.bottom
          anchors.bottom: view.bottom
          opacity: 0
          orientation: Qt.Vertical
          position: view.visibleArea.yPosition
          pageSize: view.visibleArea.heightRatio
      }

      ScrollBar {
          id: horizontalScrollBar
          width: view.width-12; height: 12
          anchors.bottom: view.bottom
          opacity: 0
          orientation: Qt.Horizontal
          position: view.visibleArea.xPosition
          pageSize: view.visibleArea.widthRatio
      }

}

