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
        onAccepted: mainwindow.modelfolder = modelFolderDialog.folder
    }
}

