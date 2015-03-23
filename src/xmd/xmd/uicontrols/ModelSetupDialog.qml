import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Controls.Styles 1.3
import QtQuick.Layouts 1.1
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2

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


//    property string modelFileName: "?"
//    property string modelFilePath: ""
//    property string modelAlias:""
    property string modelImage:""
//    property bool modelImageAsSymbol: false
    property size modelSize: Qt.size(800,400)

    ColumnLayout {
        id: column
        anchors.fill: parent
        anchors.margins: 10
        spacing:10

        RowLayout {
            id:rowName
            spacing:5
            Label{
                Layout.preferredWidth: 100
                Layout.fillHeight: true
                text:"Model name"
                color:"darkblue"
                font.pixelSize: 12
                font.bold: true
                horizontalAlignment: Text.AlignRight
            }
            Rectangle{
                Layout.fillWidth: true
                height: 30
                color: "white"
                border.color: "red"
                border.width: txtInputModelFileName.acceptableInput ? 0 : 2
                TextInput{
                    id:txtInputModelFileName
                    anchors.fill:parent
                    anchors.margins: 10
                    text: network.fileName
                    focus:true
                    selectByMouse:true
                    validator: RegExpValidator{id:regex ; regExp:/^[\w,\s-]+$/}
                    wrapMode: TextInput.NoWrap
                    font.pointSize : 10
                    onFocusChanged: if(focus)selectAll()
//                    onAccepted: okAction.trigger()
//                    Keys.onEscapePressed:cancelAction.trigger()
                }
            }
        }

        RowLayout {
            spacing:5
            Label{
                Layout.preferredWidth: 100
                Layout.fillHeight: true
                text:"Model file path"
                color:"darkblue"
                font.pixelSize: 12
                font.bold: true
                horizontalAlignment: Text.AlignRight
            }
            Button{
                text:"..."
                onClicked: modelFolderDialog.open()
            }
            Rectangle{
                Layout.fillWidth: true
                height: 60
                border.color: "red"
                border.width: txtInputModelFolder.acceptableInput ? 0 : 2
                color: "white"
                TextInput{
                    id:txtInputModelFolder
                    anchors.fill:parent
                    anchors.margins: 10
                    text: network.folder
                    readOnly: true
                    validator: RegExpValidator{id:regFolder ; regExp:/^(\S.*)$/}
                    wrapMode: TextInput.WordWrap
                    font.pointSize : 10
 //                    onAccepted: okAction.trigger()
//                    Keys.onEscapePressed:cancelAction.trigger()
                }
            }

        }
        RowLayout {
            spacing:5
            Layout.preferredHeight: 15
            Label{
                text:"Alias"
                color:"darkblue"
                font.pixelSize: 12
                font.bold: true
                width: 100
            }
            Rectangle{
                Layout.fillWidth: true
                height: 30
                color: "white"
                TextInput{
                    id:txtInputModelAlias
                    anchors.fill:parent
                    anchors.margins: 10
                    text: network.alias
                    focus:true
                    selectByMouse:true
//                    validator: RegExpValidator{id:regAlias ; regExp:/^(\S.*)$/}
                    wrapMode: TextInput.NoWrap
                    font.pointSize : 10
                    onFocusChanged: if(focus)selectAll()
//                    onAccepted: okAction.trigger()
//                    Keys.onEscapePressed:cancelAction.trigger()
                }
            }
        }
        RowLayout {
            spacing:5
            Label{
                text:"Model size"
                color:"darkblue"
                font.pixelSize: 12
                font.bold: true
                width: 100
            }
            ComboBox {
                id: comboSize
                height: 30
                editable: false
                model: ListModel {
                    id: model
                    ListElement { text: "Very large"}
                    ListElement { text: "Large"}
                    ListElement { text: "Normal"}
                    ListElement { text: "Small"}
                    ListElement { text: "Very small"}
                }
            }
        }

        RowLayout {
            spacing:5
            Layout.preferredHeight: 15
            Label{
                text:"Image"
                color:"darkblue"
                font.pixelSize: 12
                font.bold: true
                width: 100
            }
        }
        RowLayout {
            spacing:5
            Layout.preferredHeight: 15
            CheckBox {
                id: chkImageAsSymbol
                text: "as symbol"
                checked: network.asSymbol
            }
        }

        RowLayout{
            spacing:10
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignRight

            Label{
                text:"UNDER CONSTRUCTION!!"
                color:"red"
                font.pixelSize: 18
                font.bold: true
                width: 100
            }

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

    Action {
        id:cancelAction
        text: "Cancel"
        onTriggered: dialog.close()
    }
    Action {
        id: okAction
        text: "Ok"
        enabled: txtInputModelFileName.acceptableInput
        onTriggered: {
            //dialog.expression = expr.text
            network.fileName = txtInputModelFileName.text
            network.folder = txtInputModelFolder.text
            network.alias = txtInputModelAlias.text
            //network.size = Qt.size(??,??)
            //network.imageName = ""
            network.asSymbol = chkImageAsSymbol.checked
            dialog.close()
        }
    }

    //Model path dialog
    FileDialog {
        id: modelFolderDialog
        selectExisting: true
        selectFolder: true
        selectMultiple: false
        onAccepted: network.folder = modelFolderDialog.folder
    }

}

