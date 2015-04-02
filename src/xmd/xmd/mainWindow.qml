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
import QtQuick.Dialogs 1.2
import QtQuick.Window 2.1
import QtGraphicalEffects 1.0
import Qt.labs.settings 1.0
import "qrc:/xmas/xobjects/"
import "qrc:/ui/uicontrols/"
import XMAS 1.0 as XMAS

ApplicationWindow {
    id: mainwindow
    // Properties
    visible: true
    width: 1000
    height: 800
    minimumWidth: 800
    minimumHeight: 600
    color: "darkgrey"
    title: Qt.application.name + "     -     " + network.fileName + (network.modified ? "*" : "")

    property string modelFolder: util.modelPath()
    property bool confirmQuit:true
    property bool autoSave:true
    property bool showComponentNames:true
    property bool showPortNames:true
    property bool showGrid:true
    property bool snapToGrid:true

    // Persistent properties
    Settings {
        category: "xmd"
        property alias x: mainwindow.x
        property alias y: mainwindow.y
        property alias width: mainwindow.width
        property alias height: mainwindow.height
        property alias confirmQuit: mainwindow.confirmQuit
        property alias autoSave: mainwindow.autoSave
        property alias modelFolder: mainwindow.modelFolder
        property alias showComponentNames: mainwindow.showComponentNames
        property alias showPortNames: mainwindow.showPortNames
        property alias showGrid: mainwindow.showGrid
        property alias snapToGrid: mainwindow.snapToGrid
    }

    //TODO replace with qmllist in c++, belongs to plugin dialog (under construction)
    property var vtNameList

    // Signals
    signal cut
    signal copy
    signal paste
    signal zoom(var value)
    signal selectAll
    signal selectionMode(var checked)
    signal modelSetupDialog
    signal clearLog

    // JavaScripts

    // Closes the current model and starts a new one
    function newModel(save){
        if(save) saveModel()
        if(network.newFile()) {
            network.clear()
        }
    }

    function openModel(save){
        if(save) saveModel()
        network.modified = false
        dialogFileOpen.open()
    }

    function saveModel(){
        network.saveFile(network.url())
        network.modified = false
    }

    function log(message,color,name){
        xconsole.log(message,color,name)
    }

    // Event handling
    onClosing: {
        close.accepted = false; network.modified ? dialogSaveBeforeQuit.open() : confirmQuit ? dialogQuit.open() : Qt.quit()
    }
    //TODO load plugins in c++ property list
    Component.onCompleted: {fileNewAction.trigger(); plugincontrol.loadPlugins()}

    //#######################################################################################################
    //
    // Actions
    //
    //#######################################################################################################

    Action {
        id: fileNewAction
        iconSource: "qrc:/icons/content/new.png"
        iconName: "new-model"
        text: "New"
        shortcut: StandardKey.New
        onTriggered: network.modified ? dialogSaveBeforeNew.open(): newModel(false)
    }

    Action {
        id: fileOpenAction
        iconSource: "qrc:/icons/content/open.png"
        iconName: "model-open"
        text: "Open..."
        shortcut: StandardKey.Open
        onTriggered: network.modified ? dialogSaveBeforeOpen.open() : openModel(false)
    }

    Action {
        id: fileSaveAction
        iconSource: "qrc:/icons/content/save.ico"
        iconName: "model-save"
        text: "Save"
        shortcut: StandardKey.Save
        onTriggered: network.fileName === "?.json" ? modelSetupDialog() : saveModel()
    }

    Action {
        id: setupAction
        iconSource: "qrc:/icons/content/setup.ico"
        iconName: "setup"
        text: "Setup..."
        onTriggered: appSetupDialog.show()
    }

    Action {
        id: modelSetupAction
        iconSource: "qrc:/icons/content/model-setup.ico"
        iconName: "model-setup"
        text: "Model setup..."
        onTriggered: modelSetupDialog()
    }

    Action {
        id: cutAction
        text: "Cut"
        shortcut: StandardKey.Cut
        iconSource: "qrc:/icons/content/cut.png"
        iconName: "edit-cut"
        onTriggered: cut()
    }

    Action {
        id: copyAction
        text: "Copy"
        shortcut: StandardKey.Copy
        iconSource: "qrc:/icons/content/copy.png"
        iconName: "edit-copy"
        onTriggered: copy()
    }

    Action {
        id: pasteAction
        text: "Paste"
        shortcut: StandardKey.Paste
        iconSource: "qrc:/icons/content/paste.png"
        iconName: "edit-paste"
        onTriggered: paste()
    }

    Action {
        id: showComponentNamesAction
        text: "Show component names"
        checkable: true
        checked: mainwindow.showComponentNames
        onTriggered: showComponentNames = checked
    }

    Action {
        id: showPortNamesAction
        text: "Show port names"
        checkable: true
        checked: mainwindow.showPortNames
        onTriggered: showPortNames = checked
    }

    Action {
        id: zoomInAction
        text: "Zoom In"
        shortcut: StandardKey.ZoomIn
        iconSource: "qrc:/icons/content/zoom-in.ico"
        iconName: "zoom-in"
        onTriggered: zoom(0.1)
    }

    Action {
        id: zoomOutAction
        text: "Zoom Out"
        shortcut: StandardKey.ZoomOut
        iconSource: "qrc:/icons/content/zoom-out.ico"
        iconName: "zoom-out"
        onTriggered: zoom(-0.1)
    }

    Action {
        id: unZoomAction
        text: "Unzoom"
        shortcut: "Ctrl+1"
        iconSource: "qrc:/icons/content/unzoom.ico"
        iconName: "unzoom"
        onTriggered: zoom(1-network.scale)
    }

    Action {
        id: selectAreaAction
        text: "Selection Mode"
        shortcut:"Ctrl"
        iconSource: "qrc:/icons/content/select.png"
        iconName: "select"
        checkable: true
        checked: network.selectionMode
        onToggled: selectionMode(checked)
    }

    Action {
        id: selectAllAction
        text: "Select all"
        tooltip: "Select all items on the network"
        shortcut: StandardKey.SelectAll
        iconSource: "qrc:/icons/content/select_all.png"
        iconName: "selectAll"
        onTriggered: selectAll()
    }

    Action {
        id: showGridAction
        text: "Show grid"
        tooltip: "Show grid on model page"
        shortcut: "Ctrl+Shift+G"
        iconSource: "qrc:/icons/content/grid.ico"
        iconName: "Grid"
        checkable: true
        checked: showGrid
        onTriggered: showGrid = checked
    }

    Action {
        id: snapToGridAction
        text: "Snap to grid"
        tooltip: "Snap items to grid"
        shortcut: "Ctrl+Shift+S"
        iconSource: "qrc:/icons/content/snap.ico"
        iconName: "Snap"
        checkable: true
        checked: snapToGrid
        onTriggered: snapToGrid = checked
    }

    Action {
        id: quitAction
        text: "Quit"
        shortcut: StandardKey.Quit
        iconSource: "qrc:/icons/content/quit.ico"
        iconName: "Quit"
        onTriggered: network.modified ? dialogSaveBeforeQuit.open() : confirmQuit ? dialogQuit.open() : Qt.quit()
    }

    Action {
        id: clearLogAction
        text: "Clear log"
        shortcut: ""
        iconSource: "qrc:/icons/content/clean.ico"
        iconName: "Clean"
        onTriggered: clearLog()
        enabled: xconsole.open
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
            MenuItem { action: setupAction }
            MenuSeparator{}
            MenuItem { action: quitAction }
        }
        Menu {
            title: "&Edit"
            MenuItem { action: copyAction }
            MenuItem { action: cutAction }
            MenuItem { action: pasteAction }
            MenuSeparator{}
            MenuItem {action: selectAreaAction}
            MenuItem {action: selectAllAction}
            MenuSeparator{}
            MenuItem { action: modelSetupAction }

        }

        Menu {
            title: "&View"

            MenuItem { action: zoomInAction }
            MenuItem { action: zoomOutAction }
            MenuItem { action: unZoomAction }
            MenuSeparator{}
            MenuItem { action: showGridAction }
            MenuItem { action: snapToGridAction }
            MenuSeparator{}
            MenuItem { action: showComponentNamesAction }
            MenuItem { action: showPortNamesAction }
            MenuSeparator{}
            MenuItem { action: clearLogAction }
        }

        Menu {
            title: "&Help"
            MenuItem { text: "About..." ; onTriggered: aboutBox.open() }
        }
    }

    toolBar: ToolBar {
        id: mainToolBar
        style: ToolBarStyle {
            padding {left:0;right:0;top:0;bottom:0}
            background: Rectangle {
                border.color: "gray"
                color: "lightgray"
           }
        }
        ColumnLayout{
            anchors.fill: parent
            RowLayout {
                Layout.fillWidth: true
                Layout.preferredHeight: 24
                spacing: 2
                anchors.leftMargin: 5
                anchors.rightMargin: 5
                //                Layout.alignment: Qt.AlignVCenter
                ToolButton { action: fileNewAction }
                ToolButton { action: fileOpenAction }
                ToolButton { action: fileSaveAction }
                ToolBarSeparator {}
                ToolButton { action: modelSetupAction }
                ToolBarSeparator {}

                ToolButton { action: copyAction }
                ToolButton { action: cutAction }
                ToolButton { action: pasteAction }

                ToolBarSeparator {}

                ToolButton { action: zoomInAction }
                ToolButton { action: zoomOutAction }
                ToolButton { action: unZoomAction }

                ToolBarSeparator {}

                ToolButton {action: selectAreaAction}
                ToolButton {action: selectAllAction}

                ToolBarSeparator {}
                ToolButton {action: showGridAction}
                ToolButton {action: snapToGridAction}

                Item { Layout.fillWidth: true }
                ToolBarSeparator{}
                ToolButton {action: quitAction}
            }
            RowLayout {
                XToolBar{
                    id: xmasToolbar
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                }
            }
        }
    }


    //#######################################################################################################
    //
    // Time triggered actions
    //
    //#######################################################################################################
    Timer {
        id: autoSaveTimer
        interval: 500000 //every 5min
        running: autoSave & network.modified
        repeat: true
        onTriggered: saveModel()
    }


    //#######################################################################################################
    //
    // Dialogs
    //
    //#######################################################################################################

    ApplicationSetupDialog{
        id:appSetupDialog
    }

    //File Open
    FileDialog {
        id: dialogFileOpen
        nameFilters: [
            "Model files (*.xmdm *.json)",
            "All files (*)"]
        onAccepted: {
            datacontrol.fileOpen(fileUrl)
            //            if(network.fileOpen(fileUrl))
            //            {
            network.folder = folder
            network.fileName = fileUrl.toString().replace(folder + "/" ,"" )
            //            }
        }
    }

    // About
    MessageDialog {
        id: aboutBox
        title: "About XMD"
        text: "XMD is an XMAS Model Designer tool"
        icon: StandardIcon.Information //"qrc:/icons/content/app.ico"
    }

    // File overwrite
    MessageDialog {
        id: dialogOverwrite
        title: "Overwrite"
        icon: StandardIcon.Question
        text:  "Overwrite " + network.fileName + "?  Press save to confirm!"
        standardButtons: StandardButton.No | StandardButton.Yes
        onYes: saveModel(true)
    }

    // Save before new?
    MessageDialog {
        id: dialogSaveBeforeNew
        title: "New model."
        icon: StandardIcon.Question
        text:  "Save " + network.fileName + " first?"
        standardButtons: StandardButton.No | StandardButton.Yes
        onNo:  newModel(false)
    }

    // Save before open?
    MessageDialog {
        id: dialogSaveBeforeOpen
        title: "Open model."
        icon: StandardIcon.Question
        text:  "Save " + network.fileName + " first?"
        standardButtons: StandardButton.No | StandardButton.Yes
        onYes: openModel(true)
        onNo:  openModel(false)
    }

    // Save before quit?
    MessageDialog {
        id: dialogSaveBeforeQuit
        title: "Quit application."
        icon: StandardIcon.Question
        text:  "Save before quitting? " + network.fileName + "?"
        standardButtons: StandardButton.No | StandardButton.Yes
        // Qt BUG (MS Windows): need to destroy dialog internally before quit
        // to prevent warning "External WM_DESTROY received for  QWidgetWindow..."
        onNo: {this.destroy(); Qt.quit()}
        // gbo: why don't you save on yes??
        // onYes: save file somewhere if name is filled.
    }

    // Quit?
    MessageDialog {
        id: dialogQuit
        title: "Quit?"
        icon: StandardIcon.Question
        text:  "Are you sure you want to quit?"
        standardButtons: StandardButton.No | StandardButton.Yes
        // Qt BUG (MS Windows): need to destroy dialog internally before quit
        // to prevent warning "External WM_DESTROY received for  QWidgetWindow..."
        onYes: {this.destroy(); Qt.quit()}
    }

    //#######################################################################################################
    //
    // XMAS toolbar & Canvas
    //
    //#######################################################################################################

    SplitView {
        //anchors { top: xmasToolbar.bottom ; bottom: parent.bottom; left: parent.left; right: parent.right}
        anchors.fill: parent
        orientation: Qt.Vertical
        Item {
            Layout.fillHeight: true
            Flickable {
                id: view
                //center the scene by default
                anchors.fill: parent
                contentWidth: network.width
                contentHeight: network.height
                pixelAligned: true
                interactive: !network.selectionMode
                clip:true

                XNetwork{
                    id:network
                    onScaleChanged: {
                        view.resizeContent(network.width * network.scale,
                                           network.height * network.scale,Qt.point(0,0))
                    }

                }
                // Only show the scrollbars when the view is moving.
                states:
                    State {
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

        XConsole {
            id: xconsole
            Layout.minimumHeight: headerHeight
        }

        //remember the log height
        onResizingChanged: {
            xconsole.lastHeight = xconsole.height
            xconsole.open = xconsole.lastHeight > 0
        }

    }

    //#######################################################################################################
    //
    // Connections
    //
    //#######################################################################################################
    Connections {
        target: util
        onWriteLog: xconsole.log("[util]: " + message,color)
    }

    /************************************************
     * Data Control
     ************************************************/
    Connections {
        target: datacontrol
        onWriteLog: xconsole.log("[datacontrol]: " + message,color)
    }

    /************************************************
     * Plugin Control
     ************************************************/
    Connections {
        target: plugincontrol
        onWriteLog: xconsole.log("[plugincontrol]: " + message,color)
        onPluginsLoaded: {
            mainwindow.vtNameList = vtNameList
            for (var i in vtNameList) {
                xconsole.addPlugin(vtNameList[i])
            }
        }
    }
}
