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
import QtQuick.Layouts 1.0
import QtQuick.Dialogs 1.1
import QtQuick.Window 2.1
import "content"
//import org.qtproject.example 1.0

ApplicationWindow {
    visible: true
    width: 1000
    height: 800
    minimumWidth: 400
    minimumHeight: 300

    title: "test" //document.documentTitle + " - Text Editor Example"

    MessageDialog {
        id: aboutBox
        title: "About XMD"
        text: "This is a basic text editor \nwritten with Qt Quick Controls"
        icon: StandardIcon.Information
    }

    Action {
        id: cutAction
        text: "Cut"
        shortcut: "ctrl+x"
        iconSource: "qrc:/content/images/cut.png"
        iconName: "edit-cut"
        onTriggered: textArea.cut()
    }

    Action {
        id: copyAction
        text: "Copy"
        shortcut: "Ctrl+C"
        iconSource: "qrc:/content/images/copy.png"
        iconName: "edit-copy"
        //onTriggered: textArea.copy()
    }

    Action {
        id: pasteAction
        text: "Paste"
        shortcut: "ctrl+v"
        iconSource: "qrc:/content/images/paste.png"
        iconName: "edit-paste"
        //onTriggered: textArea.paste()
    }

    Action {
        id: zoomInAction
        text: "Zoom In"
        shortcut: "+"
        iconSource: "qrc:/content/images/zoomin.png"
        iconName: "zoom-in"
        //onTriggered: textArea.paste()
    }

    Action {
        id: zoomOutAction
        text: "Zoom Out"
        shortcut: "-"
        iconSource: "qrc:/content/images/zoomout.png"
        iconName: "zoom-out"
        //onTriggered: textArea.paste()
    }

    FileDialog {
        id: fileDialog
        nameFilters: ["Model files (*.xmdm)", "Composite files (*.xmdc)", "Project files (*.xmdp)"]
        //onAccepted: document.fileUrl = fileUrl
    }

    Action {
        id: fileOpenAction
        iconSource: "qrc:/content/images/open.png"
        iconName: "model-open"
        text: "Open"
        onTriggered: fileDialog.open()
    }

    menuBar: MenuBar {
        Menu {
            title: "&File"
            MenuItem { action: fileOpenAction }
            MenuItem { text: "Quit"; onTriggered: Qt.quit() }
        }
        Menu {
            title: "&Edit"
            MenuItem { action: copyAction }
            MenuItem { action: cutAction }
            MenuItem { action: pasteAction }
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

            ToolBarSeparator {}


            ToolButton { action: copyAction }
            ToolButton { action: cutAction }
            ToolButton { action: pasteAction }

            ToolBarSeparator {}

            ToolButton { action: zoomInAction }
            ToolButton { action: zoomOutAction }

            ToolBarSeparator {}


            Item { Layout.fillWidth: true }
        }
    }

    Loader {source: "canvas.qml"; width: parent.width; height: parent.height }

//    ToolBar {
//        id: secondaryToolBar
//        width: parent.width

//        RowLayout {
//            anchors.fill: parent

//            Item { Layout.fillWidth: true }
//        }
//    }




    //    TextArea {
    //        Accessible.name: "document"
    //        id: textArea
    //        frameVisible: false
    //        width: parent.width
    //        anchors.top: secondaryToolBar.bottom
    //        anchors.bottom: parent.bottom
    //        baseUrl: "qrc:/"
    //        //text: document.text
    //        textFormat: Qt.RichText
    //        Component.onCompleted: forceActiveFocus()
    //    }

    //    DocumentHandler {
    //        id: document
    //        target: textArea
    //        cursorPosition: textArea.cursorPosition
    //        selectionStart: textArea.selectionStart
    //        selectionEnd: textArea.selectionEnd
    //        Component.onCompleted: document.fileUrl = "qrc:/example.html"
    //        onFontSizeChanged: {
    //            fontSizeSpinBox.valueGuard = false
    //            fontSizeSpinBox.value = document.fontSize
    //            fontSizeSpinBox.valueGuard = true
    //        }
    //        onFontFamilyChanged: {
    //            var index = Qt.fontFamilies().indexOf(document.fontFamily)
    //            if (index == -1) {
    //                fontFamilyComboBox.currentIndex = 0
    //                fontFamilyComboBox.special = true
    //            } else {
    //                fontFamilyComboBox.currentIndex = index
    //                fontFamilyComboBox.special = false
    //            }
    //        }
    //    }
}
