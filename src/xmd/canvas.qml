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
    property real zoomFactor: 0.3

    width: 1000
    height: 800

    // Create a flickable to view a large drawing.
    Flickable {
        id: view
        anchors { top: toolbar.bottom ; bottom: parent.bottom; left: parent.left; right: parent.right}
        contentWidth: scene.width
        contentHeight: scene.height
        contentX: 990 //scene.width * zoomFactor
        contentY: 700 //toolbar.height + contentHeight/2

        Rectangle {
            id: scene
            scale: zoomFactor
            width: 2970
            height: 2100
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

    // Attach scrollbars to the right and bottom edges of the view.
      ScrollBar {
          id: verticalScrollBar
          width: 12; height: view.height-12 - toolbar.height
          anchors.right: view.right
          anchors.top: toolbar.bottom
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


    //! [top-level transitions]
    transitions: Transition {
        PropertyAnimation { duration: 3000 }
        ColorAnimation { duration: 3000 }
    }
    //! [top-level transitions]




}

