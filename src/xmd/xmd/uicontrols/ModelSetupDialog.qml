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
    title:"Model Setup"
    width: 500
    height: 300
    minimumHeight: height
    minimumWidth: width
    maximumHeight: height
    maximumWidth: width

    signal setupAccepted

    FocusScope{
        focus: true
        Component.onCompleted:{
            txtInputModelFileName.forceActiveFocus()
            txtInputModelFileName.select(0,1)
        }

        Keys.onEscapePressed:cancelAction.trigger()

        ColumnLayout {
            anchors.fill: parent
            spacing:5
            anchors.margins: 5

            // Model group 1
            RowLayout {
                GroupBox {
                    id:group1
                    title: "Model properties"
                    Layout.fillWidth: true
                    ColumnLayout {
                        id: column
                        anchors.fill: parent
                        spacing:5

                        // Model file name
                        RowLayout {
                            Layout.preferredHeight: 25
                            Layout.maximumHeight: 25
                            Layout.minimumHeight: 25
                            Label{
                                Layout.preferredWidth: 75
                                Layout.fillHeight: true
                                text:"Filename"
                                color:"darkblue"
                                font.pixelSize: 12
                                font.bold: true
                                horizontalAlignment: Text.AlignRight
                                verticalAlignment: Text.AlignVCenter
                            }
                            Rectangle{
                                Layout.fillWidth: true
                                Layout.fillHeight: true
                                color: "white"
                                border.color: "red"
                                radius: 5
                                border.width: txtInputModelFileName.acceptableInput ? 0 : 2
                                TextInput{
                                    id:txtInputModelFileName
                                    anchors.fill:parent
                                    anchors.leftMargin: 10
                                    text: network.fileName
                                    focus:true
                                    selectByMouse:true
                                    validator: RegExpValidator{id:regFileName ; regExp:/^[\w,\s-]+(\.json){1}$/}
                                    wrapMode: TextInput.NoWrap
                                    font.pointSize : 10
                                    verticalAlignment: Text.AlignVCenter
                                    onAccepted: okAction.trigger()
                                    Keys.onTabPressed: modelFolderButton.forceActiveFocus()
                                }
                            }
                        }

                        // Model folder
                        RowLayout {
                            Layout.preferredHeight: 60
                            Layout.maximumHeight: 60
                            Layout.minimumHeight: 60
                            Item{
                                Layout.preferredWidth: 75
                                Layout.fillHeight: true
                                Label{
                                    id: folderLabel
                                    anchors.right: parent.right
                                    anchors.top: parent.top
                                    anchors.topMargin: 5
                                    text:"Folder"
                                    color:"darkblue"
                                    font.pixelSize: 12
                                    font.bold: true
                                }
                                Button{
                                    id:modelFolderButton
                                    anchors.top: folderLabel.bottom
                                    anchors.left: folderLabel.left
                                    anchors.right: folderLabel.right
                                    anchors.topMargin: 2
                                    text:"..."
                                    onClicked: modelFolderDialog.open()
                                    Keys.onTabPressed: txtInputModelWidth.forceActiveFocus()
                                }
                            }
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
                                    text: network.folder
                                    readOnly: true
                                    inputMethodHints: Qt.ImhUrlCharactersOnly
                                    validator: RegExpValidator{regExp:/^(\S.*)$/}
                                    wrapMode: TextInput.WordWrap
                                    font.pointSize : 8
                                }
                            }
                        }

                        // Model size
                        RowLayout {
                            Layout.preferredHeight: 25
                            Layout.maximumHeight: 25
                            Layout.minimumHeight: 25
                            Label{
                                Layout.preferredWidth: 75
                                Layout.fillHeight: true
                                text:"Width"
                                color:"darkblue"
                                font.pixelSize: 12
                                font.bold: true
                                horizontalAlignment: Text.AlignRight
                                verticalAlignment: Text.AlignVCenter
                            }
                            Rectangle{
                                Layout.preferredWidth: 100
                                Layout.fillHeight: true
                                anchors.rightMargin: 5
                                color: "white"
                                border.color: "red"
                                radius: 5
                                border.width: txtInputModelWidth.acceptableInput ? 0 : 2
                                TextInput{
                                    id:txtInputModelWidth
                                    anchors.fill:parent
                                    anchors.leftMargin: 10
                                    text: network.size.width
                                    selectByMouse:true
                                    inputMethodHints: Qt.ImhDigitsOnly
                                    validator: IntValidator{bottom: 500; top:25000}
                                    wrapMode: TextInput.NoWrap
                                    font.pointSize : 10
                                    verticalAlignment: Text.AlignVCenter
                                    onAccepted: okAction.trigger()
                                    Keys.onTabPressed: txtInputModelHeight.forceActiveFocus()
                                }
                            }

                            Label{
                                Layout.preferredWidth: 75
                                Layout.fillHeight: true
                                text:"Height"
                                color:"darkblue"
                                font.pixelSize: 12
                                font.bold: true
                                horizontalAlignment: Text.AlignRight
                                verticalAlignment: Text.AlignVCenter
                            }

                            Rectangle{
                                Layout.preferredWidth: 100
                                Layout.fillHeight: true
                                color: "white"
                                border.color: "red"
                                radius: 5
                                border.width: txtInputModelHeight.acceptableInput ? 0 : 2
                                TextInput{
                                    id:txtInputModelHeight
                                    anchors.fill:parent
                                    anchors.leftMargin: 10
                                    text: network.size.height
                                    selectByMouse:true
                                    inputMethodHints: Qt.ImhDigitsOnly
                                    validator: IntValidator{bottom: 500; top:25000}
                                    wrapMode: TextInput.NoWrap
                                    font.pointSize : 10
                                    verticalAlignment: Text.AlignVCenter
                                    onAccepted: okAction.trigger()
                                    Keys.onTabPressed: txtInputModelAlias.forceActiveFocus()
                                }
                            }
                        }

                    }
                }
            }
            // Model group 2
            RowLayout {
                GroupBox {
                    id:group2
                    title: "Composite properties"
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    ColumnLayout {
                        anchors.fill: parent
                        spacing:5

                        // Composite alias
                        RowLayout {
                            Layout.preferredHeight: 25
                            Layout.maximumHeight: 25
                            Layout.minimumHeight: 25
                            Label{
                                Layout.preferredWidth: 75
                                Layout.fillHeight: true
                                text:"Alias"
                                color:"darkblue"
                                font.pixelSize: 12
                                font.bold: true
                                horizontalAlignment: Text.AlignRight
                                verticalAlignment: Text.AlignVCenter
                            }
                            Rectangle{
                                Layout.fillWidth: true
                                Layout.fillHeight: true
                                radius: 5
                                color: "white"
                                TextInput{
                                    id:txtInputModelAlias
                                    anchors.fill:parent
                                    anchors.leftMargin: 10
                                    text: network.alias
                                    selectByMouse:true
                                    validator: RegExpValidator{id:regAlias ; regExp:/^[\w,\s-]+$/}
                                    wrapMode: TextInput.NoWrap
                                    font.pointSize : 10
                                    verticalAlignment: Text.AlignVCenter
                                    onAccepted: okAction.trigger()
                                    Keys.onTabPressed: chkImage.forceActiveFocus()
                                }
                            }
                        }

                        // Model image
                        RowLayout {
                            Layout.preferredHeight: 40
                            Layout.maximumHeight: 40
                            Layout.minimumHeight: 40
                            Item{
                                Layout.preferredWidth: 75
                                Layout.fillHeight: true
                                CheckBox {
                                    id: chkImage
                                    anchors.right: parent.right
                                    anchors.top: parent.top
                                    anchors.topMargin: 5
                                    checked: network.imageName !== ""
                                    style: CheckBoxStyle{
                                        label: Text {
                                            color:"darkblue"
                                            font.pixelSize: 12
                                            font.bold: true
                                            text: "Image"
                                        }
                                    }
                                    Keys.onTabPressed: chkBoxedImage.forceActiveFocus()
                                }
                                CheckBox {
                                    id: chkBoxedImage
                                    anchors.top: chkImage.bottom
                                    anchors.topMargin: 5
                                    anchors.left: chkImage.left
                                    checked: network.boxedImage
                                    style: CheckBoxStyle{
                                        label: Text {
                                            color:"darkblue"
                                            font.pixelSize: 10
                                            text: "boxed?"
                                        }
                                    }
                                    Keys.onTabPressed: imageList.forceActiveFocus()
                                }
                            }

                            Rectangle{
                                id:imageRect
                                Layout.fillWidth: true
                                Layout.fillHeight: true
                                color: chkImage.checked ? "white" : "lightgrey"
                                radius: 5
                                border.color: chkImage.checked ? "darkblue" : "darkgrey"
                                border.width: 2
                                ListView {
                                    id:imageList
                                    anchors.fill: imageRect
                                    contentWidth: 50
                                    contentHeight: 50
                                    anchors.margins: 2
                                    enabled: chkImage.checked
                                    orientation: ListView.Horizontal
                                    model: symbols
                                    snapMode: ListView.SnapOneItem
                                    highlightFollowsCurrentItem: true
                                    flickDeceleration: 200
                                    clip:true
                                    highlight:
                                        Rectangle {
                                        width: imageList.height
                                        height:imageList.height
                                        radius: 5
                                        z:1
                                        color: "transparent"
                                        border.color: "cyan"
                                        border.width: 3
                                        visible: chkImage.checked
                                    }
                                    delegate:
                                        Image {
                                        height: imageList.height
                                        width: imageList.height
                                        source: Qt.resolvedUrl(fileURL)
                                        fillMode: Image.PreserveAspectFit
                                        opacity: chkImage.checked ? 1.0 : 0.25
                                        MouseArea {
                                            anchors.fill: parent
                                            onClicked: imageList.currentIndex = index
                                        }
                                    }
                                    Keys.onTabPressed: okButton.forceActiveFocus()
                                }
                            }
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
                    id:cancelButton
                    action:cancelAction
                    tooltip: ""
                    Keys.onTabPressed: txtInputModelFileName.forceActiveFocus()
                }
                Button {
                    id:okButton
                    isDefault: true
                    action: okAction
                    tooltip: ""
                    Keys.onTabPressed: cancelButton.forceActiveFocus()
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
            enabled: txtInputModelFileName.acceptableInput
                     & txtInputModelFolder.acceptableInput
                     & txtInputModelWidth.acceptableInput
                     & txtInputModelHeight.acceptableInput
            onTriggered: {
                network.fileName = txtInputModelFileName.text
                network.folder = txtInputModelFolder.text
                network.alias = txtInputModelAlias.text
                network.size = Qt.size(txtInputModelWidth.text,txtInputModelHeight.text)
                // FIXME: cannot assign [undefined] to QString
                // network.imageName = symbols.get(imageList.currentIndex,"fileName")
                network.boxedImage = chkBoxedImage.checked
                dialog.setupAccepted()
                dialog.close()
            }
        }

        // Image model
        FolderListModel {
            id: symbols
            showDirs: false
            folder: "qrc:/symbols/content/symbols/"
            nameFilters: [ "*.png", "*.jpg", "*.ico", "*.svg" ]
        }

        // Model path dialog
        FileDialog {
            id: modelFolderDialog
            selectExisting: true
            selectFolder: true
            selectMultiple: false
            onAccepted: network.folder = modelFolderDialog.folder
        }
    }
}
