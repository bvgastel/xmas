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
    id: window

    property int centerOffset: 32

    MouseArea {
        anchors.fill: parent
        onClicked: window.focus = false;

    }

    // Create a flickable to view a large canvas.
    Flickable {
        id: view

        anchors { top: toolbox.bottom ; bottom: parent.bottom; left: parent.left; right: parent.right}
        contentWidth: 1000
        contentHeight: 1000


        Rectangle {
            id: scene
            height: view.contentHeight
            width: view.contentWidth
            color: "white"
            opacity: 50
            anchors.margins: 50

        }

        // Only show the scrollbars when the view is moving.
        states: State {
            name: "ShowBars"
            when: view.movingVertically || view.movingHorizontally
            PropertyChanges { target: verticalScrollBar; opacity: 1 }
            PropertyChanges { target: horizontalScrollBar; opacity: 1 }
        }

        transitions: Transition {
            NumberAnimation { properties: "opacity"; duration: 400 }
        }

    }

    // top panel
    Rectangle {
        id: toolbox

        height: 48
        gradient: Gradient {
                 GradientStop { position: 0.0; color: "lightgrey" }
                 GradientStop { position: 1.0; color: "grey" }
             }
        //color: "lightgrey"
        anchors { right: parent.right; top: parent.top; left: parent.left}

        Column {
            //anchors.fill: parent
            spacing: 8

            Rectangle {
                //width: palette.width + 10; height: palette.height + 10
                width: parent.width - 100 ; height: palette.height + 10
                border.color: "black"

                Row {
                    id: palette
                    //anchors.centerIn: parent

                    spacing: 15

                    PaletteItem {
                        anchors.verticalCenter: parent.verticalCenter
                        componentFile: "../qml/queue.qml"
                        source: "content/images/svgs/queue.svg"
                    }
                    PaletteItem {
                        anchors.verticalCenter: parent.verticalCenter
                        componentFile: "../qml/function.qml"
                        source: "content/images/svgs/function.svg"
                    }
                    PaletteItem {
                        anchors.verticalCenter: parent.verticalCenter
                        componentFile: "../qml/fork.qml"
                        source: "content/images/svgs/fork.svg"
                    }
                    PaletteItem {
                        anchors.verticalCenter: parent.verticalCenter
                        componentFile: "../qml/join.qml"
                        source: "content/images/svgs/join.svg"
                    }
                    PaletteItem {
                        anchors.verticalCenter: parent.verticalCenter
                        componentFile: "../qml/switch.qml"
                        source: "content/images/svgs/switch.svg"
                    }
                    PaletteItem {
                        anchors.verticalCenter: parent.verticalCenter
                        componentFile: "../qml/merge.qml"
                        source: "content/images/svgs/merge.svg"
                    }
                    PaletteItem {
                        anchors.verticalCenter: parent.verticalCenter
                        componentFile: "../qml/sink.qml"
                        source: "content/images/svgs/sink.svg"
                    }
                    PaletteItem {
                        anchors.verticalCenter: parent.verticalCenter
                        componentFile: "../qml/source.qml"
                        source: "content/images/svgs/source.svg"
                    }
                    PaletteItem {
                        anchors.verticalCenter: parent.verticalCenter
                        componentFile: "../qml/in.qml"
                        source: "content/images/svgs/in.svg"
                    }
                    PaletteItem {
                        anchors.verticalCenter: parent.verticalCenter
                        componentFile: "../qml/out.qml"
                        source: "content/images/svgs/out.svg"
                    }
                    PaletteItem {
                        anchors.verticalCenter: parent.verticalCenter
                        componentFile: "../qml/spidergon.qml"
                        source: "content/images/svgs/composite.svg"
                    }

                }

            }



        }
    }

    // Attach scrollbars to the right and bottom edges of the view.
    ScrollBar {
        id: verticalScrollBar
        width: 12; height: view.height-12
        anchors.right: view.right
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

    DropShadow {
           anchors.fill: view
           horizontalOffset: 3
           verticalOffset: 3
           radius: 8.0
           samples: 16
           color: "#80000000"
           source: view
       }

}

