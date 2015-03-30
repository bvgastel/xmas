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
import QtQuick.Controls.Styles 1.3
import QtQuick.Layouts 1.1
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
import Qt.labs.folderlistmodel 2.1

Window {
    id:dialog
    visible: false
    modality: Qt.WindowModal
    flags: Qt.Dialog
    color: "darkgrey"
    title:"Application Setup"
    width: 400
    height: 175
    minimumHeight: height
    minimumWidth: width
    maximumHeight: height
    maximumWidth: width

    signal setupAccepted

    ColumnLayout {
        id: column
        anchors.fill: parent
        spacing:5
        anchors.margins: 5

        // Main Model folder
        RowLayout {
            Layout.preferredHeight: 25
            Layout.maximumHeight: 25
            Layout.minimumHeight: 25
            anchors.top: parent.top
            Item{
                Layout.preferredWidth: 100
                Layout.fillHeight: true
                Label{
                    id: folderLabel
                    anchors.left: parent.left
                    anchors.top: parent.top
                    anchors.topMargin: 5
                    text:"Main model folder"
                    color:"darkblue"
                    font.pixelSize: 12
                    font.bold: true
                }
                Button{
                    anchors.top: parent.top
                    anchors.left: folderLabel.right
                    text:"..."
                    onClicked: modelFolderDialog.open()
                }
            }
        }
        RowLayout {
            Layout.preferredHeight: 40
            Layout.maximumHeight: 40
            Layout.minimumHeight: 40

            Rectangle{
                Layout.fillWidth: true
                Layout.fillHeight: true
                radius: 5
                border.color: "red"
                border.width: txtInputModelFolder.acceptableInput ? 0 : 2
                color: "white"
                TextInput{
                    id:txtInputModelFolder
                    anchors.fill:parent
                    anchors.leftMargin: 10
                    text: mainwindow.modelFolder
                    readOnly: true
                    inputMethodHints: Qt.ImhUrlCharactersOnly
                    validator: RegExpValidator{regExp:/^(\S.*)$/}
                    wrapMode: TextInput.WordWrap
                    font.pointSize : 8
                }
            }
        }

        // Model confirm to quit
        RowLayout {
            Layout.preferredHeight: 20
            Layout.maximumHeight: 20
            Layout.minimumHeight: 20
            Layout.preferredWidth: 75
            CheckBox {
                    id: chkConfirmQuit
                    anchors.right: parent.right
                    anchors.top: parent.top
                    anchors.topMargin: 5
                    checked: mainwindow.confirmQuit
                    style: CheckBoxStyle{
                        label: Text {
                            color:"darkblue"
                            font.pixelSize: 12
                            font.bold: true
                            text: "Confirm to quit"
                        }
                    }
                }
            }

        // Model auto save
        RowLayout {
            Layout.preferredHeight: 20
            Layout.maximumHeight: 20
            Layout.minimumHeight: 20
            Layout.preferredWidth: 75
            CheckBox {
                    id: chkAutoSave
                    anchors.right: parent.right
                    anchors.top: parent.top
                    anchors.topMargin: 5
                    checked: mainwindow.autoSave
                    style: CheckBoxStyle{
                        label: Text {
                            color:"darkblue"
                            font.pixelSize: 12
                            font.bold: true
                            text: "Auto save"
                        }
                    }
                }
            }

        // Dialog buttons
        RowLayout{
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignRight
            Button{
                action:cancelAction
                tooltip: ""
            }
            Button {
                isDefault: true
                action: okAction
                tooltip: ""
            }
        }
    }


    // Dialog actions
    Action {
        id:cancelAction
        text: "Cancel"
        onTriggered: dialog.close()
    }
    Action {
        id: okAction
        text: "Ok"
        enabled: txtInputModelFolder.acceptableInput
        onTriggered: {
            mainwindow.modelFolder = txtInputModelFolder.text
            mainwindow.confirmQuit = chkConfirmQuit.checked
            mainwindow.autoSave = chkAutoSave.checked
            dialog.setupAccepted()
            dialog.close()
        }
    }
    // Model path dialog
    FileDialog {
        id: modelFolderDialog
        selectExisting: true
        selectFolder: true
        selectMultiple: false
        onAccepted: mainwindow.modelFolder = modelFolderDialog.folder
    }
}

