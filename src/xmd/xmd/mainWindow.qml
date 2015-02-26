/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the Qt Quick Controls module of the Qt Toolkit.
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
import QtQuick.Layouts 1.1
import QtQuick.Dialogs 1.1
import QtQuick.Window 2.1
import QtGraphicalEffects 1.0
import "xobjects"
import "uicontrols"

ApplicationWindow {
    id: mainwindow
    visible: true
    width: 1000
    height: 800
    minimumWidth: 400
    minimumHeight: 300
    color: "darkgrey"

    title: "XMAS Model Designer 2015"

    MessageDialog {
        id: aboutBox
        title: "About XMD"
        text: "XMD is an XMAS Model Designer tool"
        icon: StandardIcon.Information
    }

    Action {
        id: loadPlugins
        iconSource: "qrc:/content/images/plugin.png"
        iconName: "plugins"
        text: "VT"
        shortcut: "Alt+P"
        //onTriggered:
    }

    Action {
        id: fileOpenAction
        iconSource: "qrc:/content/open.png"
        iconName: "model-open"
        text: "Open"
        shortcut: StandardKey.Open
        onTriggered: fileDialog.open()
    }

    Action {
        id: fileSaveAction
        iconSource: "qrc:/content/save.png"
        iconName: "model-save"
        text: "Save"
        shortcut: StandardKey.Save
        onTriggered: sheet.test()//fileSaveDialog.open()
    }


    Action {
        id: cutAction
        text: "Cut"
        shortcut: StandardKey.Cut
        iconSource: "qrc:/content/cut.png"
        iconName: "edit-cut"
        onTriggered: log("Cut Action Clicked.","red") //textArea.cut()
    }

    Action {
        id: copyAction
        text: "Copy"
        shortcut: StandardKey.Copy
        iconSource: "qrc:/content/copy.png"
        iconName: "edit-copy"
        onTriggered: log("Copy Action Clicked.","blue") //textArea.copy()
    }

    Action {
        id: pasteAction
        text: "Paste"
        shortcut: StandardKey.Paste
        iconSource: "qrc:/content/paste.png"
        iconName: "edit-paste"
        onTriggered: log("Paste Action Clicked.","green") //textArea.paste()
    }

    Action {
        id: selectAllAction
        text: "Select All"
        shortcut: StandardKey.SelectAll
        onTriggered: sheet.selectAll()
        tooltip: "Select All items on the sheet"
    }

    Action {
        id: zoomInAction
        text: "Zoom In"
        shortcut: StandardKey.ZoomIn
        iconSource: "qrc:/content/zoom-in.png"
        iconName: "zoom-in"
        onTriggered: sheet.zoomIn()
    }

    Action {
        id: zoomOutAction
        text: "Zoom Out"
        shortcut: StandardKey.ZoomOut
        iconSource: "qrc:/content/zoom-out.png"
        iconName: "zoom-out"
        onTriggered: sheet.zoomOut()
    }

    Action {
        id: zoomFitAction
        text: "Zoom Fit"
        shortcut: "Ctrl+1"
        iconSource: "qrc:/content/zoom-fit.png"
        iconName: "zoom-fit"
        onTriggered: sheet.zoomFit()
    }

    Action {
        id: selectionCursorAction
        text: "Selection Mode"
        shortcut:""
        iconSource: "qrc:/content/select.png"
        iconName: "select"
        checkable: true
        onToggled: sheet.selectionMode = checked
    }

    Action {
        id: fileQuitAction
        text: "Quit"
        shortcut: StandardKey.Quit
        onTriggered: Qt.quit()
    }

    FileDialog {
        id: fileDialog
        nameFilters: [
            "Model files (*.xmdm *.fjson *.wck *.json)",
            "Composite files (*.xmdc)",
            "Project files (*.xmdp)",
            "All files (*)"]
        onAccepted: controller.fileOpen(fileUrl)
    }

    FileDialog {
        id: fileSaveDialog
        selectMultiple: false
        selectExisting: true
        nameFilters: [
            "Model files (*.xmdm *.fjson *.wck *.json)",
            "Composite files (*.xmdc)",
            "Project files (*.xmdp)",
            "All files (*)"]
        //onAccepted: controller.fileOpen(fileUrl)
    }

    function log(text,color)
    {
        outputLog.log(text,color)
    }

    menuBar: MenuBar {
        Menu {
            title: "&File"
            MenuItem { action: fileOpenAction }
            MenuItem { action: fileSaveAction }
            MenuSeparator{}
            MenuItem { action: fileQuitAction }
        }
        Menu {
            title: "&Edit"
            MenuItem { action: copyAction }
            MenuItem { action: cutAction }
            MenuItem { action: pasteAction }
            MenuSeparator{}
            MenuItem {action: selectAllAction}
        }

        Menu {
            title: "&View"
            MenuItem { action: zoomInAction }
            MenuItem { action: zoomOutAction }
            MenuItem { action: zoomFitAction }
        }

        Menu {
            title: "&Help"
            MenuItem { text: "About..." ; onTriggered: aboutBox.open() }
        }
    }

    toolBar: ToolBar {
        id: mainToolBar
        width: parent.width
        RowLayout {

            anchors.fill: parent
            spacing: 0
            ToolButton { action: fileOpenAction }
            ToolButton { action: fileSaveAction }
            ToolBarSeparator {}


            ToolButton { action: copyAction }
            ToolButton { action: cutAction }
            ToolButton { action: pasteAction }

            ToolBarSeparator {}

            ToolButton { action: zoomInAction }
            ToolButton { action: zoomOutAction }
            ToolButton { action: zoomFitAction }

            ToolBarSeparator {}

            ToolButton {action: selectionCursorAction}

            ToolBarSeparator {}

            Item { Layout.fillWidth: true }
        }
    }


    XToolBar{
        id: xmasToolbar
        height:48
        anchors {right: parent.right;  left: parent.left}
    }

    SplitView {
        anchors { top: xmasToolbar.bottom ; bottom: parent.bottom; left: parent.left; right: parent.right}
        orientation: Qt.Vertical
        Item {
            Layout.fillHeight: true
            Flickable {
                id: view
                //center the scene by default
                anchors.fill: parent
                contentX: sheet ? (1 - sheet.scale) * sheet.width * 0.5 : 0
                contentY: sheet ? (1 - sheet.scale) * sheet.height * 0.5 : 0
                contentWidth: sheet ? sheet.width * sheet.scale : 0
                contentHeight: sheet ? sheet.height * sheet.scale : 0
                pixelAligned: true
                interactive: sheet ? !sheet.selectionMode : true



                //        onFlickEnded: {
                //            console.log("x : " + view.visibleArea.xPosition
                //                        + " wr : " + view.visibleArea.widthRatio
                //                        + " y : " + view.visibleArea.yPosition
                //                        + " hr : " + view.visibleArea.heightRatio)
                //        }



                XNetwork{
                    id:sheet
                    transformOrigin: Item.TopLeft
                    color: "white"
                    width : 5940
                    height: 4200

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
                width: 12; height: view.height-12
                anchors.right: view.right
                anchors.top: view.top
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

        onResizingChanged: {
            outputLog.lastHeight = outputLog.height
            outputLog.open = outputLog.lastHeight > 0
        }

        OutputLog {
            id: outputLog
            Layout.minimumHeight: headerHeight
        }

    }
    Connections {
        target: controller
        onWriteLog: log(message,color) //console.log("component create request")
        onPluginsLoaded: {
            for (var i in object) {
                log(object[i].name,"red")
            }
        }

    }
    Component.onCompleted: {
        controller.loadPlugins()
    }
}
