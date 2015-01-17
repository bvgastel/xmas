/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
**     of its contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Layouts 1.0
import QtQuick.Dialogs 1.1
import QtQuick.Window 2.1
import "content"

Item {
    id: window

    property int centerOffset: 32

    height: 800; width: 1000

    MouseArea {
        anchors.fill: parent
        onClicked: window.focus = false;
    }

    //This is the message box that pops up when there's an error
    Rectangle {
        id: dialog

        opacity: 0
        anchors.centerIn: parent
        width: dialogText.width + 6; height: dialogText.height + 6
        border.color: 'black'
        color: 'lightsteelblue'
        z: 65535 //Arbitrary number chosen to be above all the items, including the scaled perspective ones.

        function show(str){
            dialogText.text = str;
            dialogAnim.start();
        }

        Text {
            id: dialogText
            x: 3; y: 3
            font.pixelSize: 14
        }

        SequentialAnimation {
            id: dialogAnim
            NumberAnimation { target: dialog; property:"opacity"; to: 1; duration: 1000 }
            PauseAnimation { duration: 5000 }
            NumberAnimation { target: dialog; property:"opacity"; to: 0; duration: 1000 }
        }
    }

    // Create a flickable to view a large canvas.
    Flickable {
        id: view

        anchors { top: toolbox.bottom ; bottom: parent.bottom; left: parent.left; right: parent.right}
        contentWidth: 2500
        contentHeight: 2500


        Rectangle {
            id: scene
            height: view.contentHeight
            width: view.contentWidth
            color: "lightblue"
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
        color: "lightgrey"
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

                    Button {
                        id: openFile
                        anchors.verticalCenter: parent.verticalCenter
                        text: "Open"
                        onClicked: fileDialog.open()
                    }

                    Button {
                        id: saveFile
                        anchors.verticalCenter: parent.verticalCenter
                        text: "Save"
                    }
                }

            }



        }
    }


    FileDialog {
        id: fileDialog
        nameFilters: ["Model files (*.xmdm)", "Composite files (*.xmdc)", "Project files (*.xmdp)"]
        //onAccepted: document.fileUrl = fileUrl
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
}
