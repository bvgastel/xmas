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
import QtQuick.Controls.Styles 1.3
import QtQuick.Layouts 1.1
import QtQuick.Dialogs 1.1
import QtQuick.Window 2.1
import QtGraphicalEffects 1.0
import Qt.labs.settings 1.0

import "xobjects"
import "uicontrols"
//import XMAS.model 1.0 as XMAS

ApplicationWindow {
    id: mainwindow
    // Properties
    visible: true
    width: 1000
    height: 800
    minimumWidth: 400
    minimumHeight: 300
    color: "darkgrey"
    title: network.fileName + (network.modified ? "*" : "") + "     -     " + Qt.application.name

    property string defaultModelDirectory
    property int newModelCounter:1

    // Persistent properties
    Settings {
        category: "xmd"
        property alias x: mainwindow.x
        property alias y: mainwindow.y
        property alias width: mainwindow.width
        property alias height: mainwindow.height
        property alias defaultModelDirectory: mainwindow.defaultModelDirectory
    }

    //TODO replace with qmllist in c++, belongs to plugin dialog (under construction)
    property var vtNameList

    // Signals
    signal cut
    signal copy
    signal paste
    signal zoomIn
    signal zoomOut
    signal zoomFit
    signal selectAll
    signal selectionMode(var checked)
    signal showComponentNames(var checked)

    // JavaScripts

    function openComposite(){
        openCompositeDialog.open()
    }

    function log(text,color)
    {
        outputLog.log(text,color)
    }

    function modelName(url)
    {
        return "QUrl()"
    }

    // Event handling
    onClosing: {close.accepted = false;  network.modified ? dialogSaveOnClose.open() : dialogQuit.open()}
    //TODO load plugins in c++ property list
    Component.onCompleted: plugincontrol.loadPlugins()


    //#######################################################################################################
    //
    // Actions
    //
    //#######################################################################################################

    Action {
        id: fileNewAction
        iconSource: "qrc:/content/new.png"
        iconName: "new-model"
        text: "New"
        shortcut: StandardKey.New
        //onTriggered: fileOpenDialog.open()
    }

    Action {
        id: fileOpenAction
        iconSource: "qrc:/content/open.png"
        iconName: "model-open"
        text: "Open..."
        shortcut: StandardKey.Open
        onTriggered: fileOpenDialog.open()
    }

    Action {
        id: fileSaveAction
        iconSource: "qrc:/content/save.ico"
        iconName: "model-save"
        text: "Save"
        shortcut: StandardKey.Save
        onTriggered: fileSaveDialog.open()
    }

    Action {
        id: fileSaveAsAction
        iconSource: "qrc:/content/saveAs.ico"
        iconName: "model-save-as"
        text: "Save as..."
        shortcut: StandardKey.SaveAs
        onTriggered: fileSaveDialog.open()
    }

    Action {
        id: fileCloseAction
        text: "Close"
        shortcut: StandardKey.Close
        //onTriggered:
    }

    Action {
        id: cutAction
        text: "Cut"
        shortcut: StandardKey.Cut
        iconSource: "qrc:/content/cut.png"
        iconName: "edit-cut"
        onTriggered: cut()
    }

    Action {
        id: copyAction
        text: "Copy"
        shortcut: StandardKey.Copy
        iconSource: "qrc:/content/copy.png"
        iconName: "edit-copy"
        onTriggered: copy()
    }

    Action {
        id: pasteAction
        text: "Paste"
        shortcut: StandardKey.Paste
        iconSource: "qrc:/content/paste.png"
        iconName: "edit-paste"
        onTriggered: paste()
    }

    Action {
        id: showComponentNamesAction
        text: "Show Component names"
        checkable: true
        checked: true
        onTriggered: showComponentNames(checked)
    }

    Action {
        id: zoomInAction
        text: "Zoom In"
        shortcut: StandardKey.ZoomIn
        iconSource: "qrc:/content/zoom-in.png"
        iconName: "zoom-in"
        onTriggered: zoomIn()
    }

    Action {
        id: zoomOutAction
        text: "Zoom Out"
        shortcut: StandardKey.ZoomOut
        iconSource: "qrc:/content/zoom-out.png"
        iconName: "zoom-out"
        onTriggered: zoomOut()
    }

    Action {
        id: zoomFitAction
        text: "Zoom Fit"
        shortcut: "Ctrl+1"
        iconSource: "qrc:/content/zoom-fit.png"
        iconName: "zoom-fit"
        onTriggered: zoomFit()
    }

    Action {
        id: selectAreaAction
        text: "Selection Mode"
        shortcut:"Ctrl"
        iconSource: "qrc:/content/select.png"
        iconName: "select"
        checkable: true
        checked: network.selectionMode
        onToggled: selectionMode(checked)
    }

    Action {
        id: selectAllAction
        text: "Select All"
        shortcut: StandardKey.SelectAll
        iconSource: "qrc:/content/select_all.png"
        iconName: "selectAll"
        onTriggered: selectAll()
        tooltip: "Select All items on the network"
    }

    Action {
        id: fileQuitAction
        text: "Quit"
        shortcut: StandardKey.Quit
        iconSource: "qrc:/content/quit.ico"
        iconName: "Quit"
        onTriggered: network.modified ? dialogSaveOnQuit.open() : Qt.quit()
    }

    Action {
        id: packetAction
        text: "Packet"
        shortcut: ""
        iconSource: "qrc:/content/packet.png"
        iconName: "Packet"
        onTriggered: packetDialog.show()
    }


    Action {
        id: loadPlugins
        iconSource: "qrc:/content/plugin.ico"
        iconName: "plugins"
        text: "VT"
        shortcut: "Alt+P"
        //onTriggered:
    }

    Action {
        id: runVtAction
        text: "Run"
        shortcut: "Ctrl+R"
        iconSource: "qrc:/content/run.ico"
        iconName: "select"
        onTriggered: {
            for (var i in mainwindow.vtNameList) {
                var params = plugincontrol.pluginParams(mainwindow.vtNameList[i])
                log ("params = [" + params +"] Running to be implemented", "green");

            }
        }
    }

    Action {
        id: stopVtAction
        text: "Stop"
        shortcut: ""
        iconSource: "qrc:/content/stop.ico"
        iconName: "select"
        //onTriggered: controller.stop()
    }


    //#######################################################################################################
    //
    // Menu & toolbars
    //
    //#######################################################################################################


    menuBar: MenuBar {
        Menu {
            title: "&File"
            MenuItem { action: fileNewAction }
            MenuItem { action: fileOpenAction }
            MenuItem { action: fileSaveAction }
            MenuItem { action: fileSaveAsAction }
            MenuItem { action: fileCloseAction }
            MenuSeparator{}
            MenuItem { action: fileQuitAction }
        }
        Menu {
            title: "&Edit"
            MenuItem { action: copyAction }
            MenuItem { action: cutAction }
            MenuItem { action: pasteAction }
            MenuSeparator{}
            MenuItem {action: selectAreaAction}
            MenuItem {action: selectAllAction}
        }

        Menu {
            title: "&View"

            MenuItem { action: zoomInAction }
            MenuItem { action: zoomOutAction }
            MenuItem { action: zoomFitAction }
            MenuSeparator{}
            MenuItem { action: showComponentNamesAction }
        }

        Menu {
            title: "&Tools"
            MenuItem { action: runVtAction }
            MenuItem { action: stopVtAction }
        }

        Menu {
            title: "&Help"
            MenuItem { text: "About..." ; onTriggered: aboutBox.open() }
        }
    }

    toolBar: ToolBar {
        id: mainToolBar
        width: parent.width
        style: ToolBarStyle {
            padding {
                left: 5
                right: 5
                top: 2
                bottom: 2
            }
            background: Rectangle {
                implicitWidth: 100
                implicitHeight: 40
                border.color: "gray"
                color: "lightgray"
                //                   gradient: Gradient {
                //                       GradientStop { position: 0 ; color: "lightgray" }
                //                       GradientStop { position: 1 ; color: "gray" }
                //                   }
            }
        }
        RowLayout {
            anchors.fill: parent
            spacing: 5
            ToolButton { action: fileNewAction }
            ToolButton { action: fileOpenAction }
            ToolButton { action: fileSaveAction }
            ToolButton { action: fileSaveAsAction }
            ToolBarSeparator {}


            ToolButton { action: copyAction }
            ToolButton { action: cutAction }
            ToolButton { action: pasteAction }

            ToolBarSeparator {}

            ToolButton { action: zoomInAction }
            ToolButton { action: zoomOutAction }
            ToolButton { action: zoomFitAction }

            ToolBarSeparator {}

            ToolButton {action: selectAreaAction}
            ToolButton {action: selectAllAction}

            ToolBarSeparator {}

            ToolButton {action: packetAction}

            ToolBarSeparator {}

            ToolButton {action: runVtAction}
            ToolButton {action: stopVtAction}

            ComboBox {
                width: 200
                //TODO replace with plugin list
                model: [ "SyntaxChecker"]
                implicitWidth: 200
                implicitHeight: 20
            }

            //TODO replace with plugin progress value
            ProgressBar {
                id:progressbar
                value: 50
                indeterminate: false
                minimumValue: 0
                maximumValue: 100

                style: ProgressBarStyle {
                    background: Rectangle {
                        radius: 5
                        color: "darkgray"
                        border.color: "darkgray"
                        border.width: 0
                        implicitWidth: 200
                        implicitHeight: 18
                    }
                    progress: Rectangle {
                        border.width:1
                        border.color:"steelblue"
                        radius: 4
                        gradient: Gradient {
                            GradientStop { position: 0.0; color: "steelblue" }
                            GradientStop { position: 0.4; color: "lightsteelblue" }
                            GradientStop { position: 1.0; color: "steelblue" }
                        }
                        Item {
                            anchors.fill: parent
                            anchors.margins: 1
                            visible: progressbar.indeterminate
                            clip: true
                            Row {
                                Repeater {
                                    Rectangle {
                                        color: index % 2 ? "steelblue" : "lightsteelblue"
                                        width: 20 ; height: progressbar.height
                                    }
                                    model: progressbar.width / 20 + 2
                                }
                                XAnimator on x {
                                    from: 0 ; to: -40
                                    loops: Animation.Infinite
                                    running: progressbar.indeterminate
                                }
                            }
                        }
                    }
                }
            }
            //TODO replace with plugin progress value

            Item { Layout.fillWidth: true }
        }
    }


    XToolBar{
        id: xmasToolbar
        height:48
        anchors {right: parent.right;  left: parent.left}
    }

    //#######################################################################################################
    //
    // Dialogs
    //
    //#######################################################################################################

    XPacketDialog {
        id: packetDialog
    }

    //File Open
    FileDialog {
        id: fileOpenDialog
        nameFilters: [
            "Model files (*.xmdm *.fjson *.json)",
            "Composite files (*.xmdc)",
            "Project files (*.xmdp)",
            "All files (*)"]
        onAccepted: datacontrol.fileOpen(fileUrl)
    }

    //File Save
    FileDialog {
        id: fileSaveDialog
        selectMultiple: false
        selectExisting: true
        nameFilters: [
            "Model files (*.xmd *.fjson *.json)",
            "Composite files (*.xmdc)",
            "Project files (*.xmdp)",
            "All files (*)"]

        onAccepted: {
            if(network.saveFile(fileUrl))
            {
                network.fileName = fileUrl
                network.modified = false
            }
        }
    }

    //File Save as
    FileDialog {
        id: openCompositeDialog
        nameFilters: [
            "Model files (*.xmd *.fjson *.wck *.json)",
            "Composite files (*.xmdc)",
            "Project files (*.xmdp)",
            "All files (*)"]
        //onAccepted: network.openComposite(fileUrl)
    }

    // About
    MessageDialog {
        id: aboutBox
        title: "About XMD"
        text: "XMD is an XMAS Model Designer tool"
        icon: StandardIcon.Information //"qrc:/content/app.ico"
    }

    // File overwrite
    MessageDialog {
        id: dialogOverwrite
        title: fileSaveAsAction.text
        icon: StandardIcon.Question
        text:  network.fileName + " already exists!  Replace?"
        detailedText: "To replace a file means that its existing contents will be lost. " +
                      "The file that you are copying now will be copied over it instead."
        standardButtons: StandardButton.Yes | StandardButton.No
        onYes: console.log("overwritten!")
        onNo: visible=false
    }

    // Close file without save?
    MessageDialog {
        id: dialogSaveOnClose
        title: "Close model."
        icon: StandardIcon.Question
        text:  "Close " + network.fileName + " without saving it?"
        standardButtons: StandardButton.Yes | StandardButton.No
        onYes: Qt.quit()
        onNo: visible=false
    }

    // Quit without save?
    MessageDialog {
        id: dialogSaveOnQuit
        title: "Quit application."
        icon: StandardIcon.Question
        text:  "Quit without saving " + network.fileName + "?"
        standardButtons: StandardButton.Yes | StandardButton.No
        onYes: Qt.quit()
        onNo: visible=false
    }

    // Quit?
    MessageDialog {
        id: dialogQuit
        title: "Quit?"
        icon: StandardIcon.Question
        text:  "Really wanna quit such a magnificent tool :P ?"
        standardButtons: StandardButton.Yes | StandardButton.No
        onYes: Qt.quit()
        onNo: visible=false
    }


    //#######################################################################################################
    //
    // Canvas
    //
    //#######################################################################################################

    SplitView {
        anchors { top: xmasToolbar.bottom ; bottom: parent.bottom; left: parent.left; right: parent.right}
        orientation: Qt.Vertical
        Item {
            Layout.fillHeight: true
            Flickable {
                id: view
                //center the scene by default
                anchors.fill: parent
                contentX: network ? (1 - network.scale) * network.width * 0.5 : 0
                contentY: network ? (1 - network.scale) * network.height * 0.5 : 0
                contentWidth: network ? network.width * network.scale : 0
                contentHeight: network ? network.height * network.scale : 0
                pixelAligned: true
                interactive: network ? !network.selectionMode : true

                //contentWidth: contentItem.childrenRect.width; contentHeight: contentItem.childrenRect.height


                //        onFlickEnded: {
                //            console.log("x : " + view.visibleArea.xPosition
                //                        + " wr : " + view.visibleArea.widthRatio
                //                        + " y : " + view.visibleArea.yPosition
                //                        + " hr : " + view.visibleArea.heightRatio)
                //        }

                XNetwork{
                    id:network
                    fileName: "Model" + newModelCounter
                    onMoveSelected: {
                        if(group.x < view.contentX)
                            scrollLeft.start()
                        else
                            scrollLeft.stop()
                        if(group.x + group.width - 50 > view.contentX + view.width)
                            scrollRight.start()
                        else
                            scrollRight.stop()
                        if(group.y < view.contentY)
                            scrollUp.start()
                        else
                            scrollUp.stop()
                        if(group.y + group.height - 50 > view.contentY + view.height)
                            scrollDown.start()
                        else
                            scrollDown.stop()
                    }

                }
                // Only show the scrollbars when the view is moving.
                states:
                    State {
                    name: "ShowBars"
                    when: view.movingVertically || view.movingHorizontally
                          || scrollLeft.running || scrollRight.running
                          || scrollUp.running || scrollDown.running
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

            MouseArea {
                id: scrollZoneLeft
                width: 10
                height: view.height
                anchors.left: view.left
                hoverEnabled: true
                preventStealing: false
                z: 100
                onExited: view.returnToBounds()
            }

            MouseArea {
                id: scrollZoneRight
                width: 10
                height: view.height
                anchors.right: view.right
                hoverEnabled: true
                preventStealing: false
                z: 100
                onExited: view.returnToBounds()
            }

            MouseArea {
                id: scrollZoneTop
                width: view.width
                height: 10
                anchors.top: view.top
                hoverEnabled: true
                preventStealing: false
                z: 100
                onExited: view.returnToBounds()
            }

            MouseArea {
                id: scrollZoneBottom
                width: view.width
                height: 10
                anchors.bottom: view.bottom
                hoverEnabled: true
                preventStealing: false
                z: 100
                onExited: view.returnToBounds()
            }

            SmoothedAnimation {
                id: scrollLeft
                target: view
                property: "contentX"
                to:-50
                velocity: 1000
                onStopped: view.returnToBounds()
                //running: scrollZoneLeft.containsMouse
            }

            SmoothedAnimation {
                id: scrollRight
                target: view
                property: "contentX"
                to: view.contentWidth + 50
                velocity: 1000
                onStopped: view.returnToBounds()
                //running: scrollZoneRight.containsMouse
            }

            SmoothedAnimation {
                id: scrollUp
                target: view
                property: "contentY"
                to: -50
                velocity: 1000
                onStopped: view.returnToBounds()
                //running: scrollZoneTop.containsMouse
            }

            SmoothedAnimation {
                id: scrollDown
                target: view
                property: "contentY"
                to: view.contentHeight + 50
                velocity: 1000
                onStopped: view.returnToBounds()
                //running: scrollZoneBottom.containsMouse
            }
        }

        //remember the log height
        onResizingChanged: {
            outputLog.lastHeight = outputLog.height
            outputLog.open = outputLog.lastHeight > 0
        }

        OutputLog {
            id: outputLog
            Layout.minimumHeight: headerHeight
        }

    }

    //#######################################################################################################
    //
    // Connctions
    //
    //#######################################################################################################

    /************************************************
     * Data Control
     ************************************************/
    Connections {
        target: datacontrol
        onWriteLog: log(message,color)
    }

    /************************************************
     * Plugin Control
     ************************************************/
    Connections {
        target: plugincontrol
        onWriteLog: log(message,color)
        onPluginsLoaded: {
            mainwindow.vtNameList = vtNameList
            var line = " Loaded plugins: [ ";
            var glue = "";
            for (var i in vtNameList) {
                line += glue + vtNameList[i];
                glue = ", ";
            }
            line += " ]";
            log(line, "red");
        }
    }

}
