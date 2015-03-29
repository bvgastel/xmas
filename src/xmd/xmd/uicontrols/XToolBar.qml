import QtQuick 2.4
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.3
import QtQuick.Dialogs 1.2
import "qrc:/ui/uicontrols/"

Rectangle {
    id:toolbox
    z:1
    height:48
    gradient: Gradient {
        GradientStop { position: 0.0; color: "black" }
        GradientStop { position: 0.4; color: "grey" }
        GradientStop { position: 1.0; color: "black" }
    }

    RowLayout {
        //id: palette
        anchors.fill: parent
        anchors.margins: {top: 5; bottom: 5}
        spacing: 5
        ToolButton {
            action: packetAction
        }
        ToolBarSeparator {}
        XToolBarPrimitiveItem {
            componentFile: "qrc:/xmas/xobjects/queue.qml"
            image: "qrc:/icons/content/queue.png"
        }
        XToolBarPrimitiveItem {
            componentFile: "qrc:/xmas/xobjects/function.qml"
            image: "qrc:/icons/content/function.png"
        }
        XToolBarPrimitiveItem {
            componentFile: "qrc:/xmas/xobjects/fork.qml"
            image: "qrc:/icons/content/fork.png"
        }
        XToolBarPrimitiveItem {
            componentFile: "qrc:/xmas/xobjects/join.qml"
            image: "qrc:/icons/content/join.png"
        }
        XToolBarPrimitiveItem {
            componentFile: "qrc:/xmas/xobjects/switch.qml"
            image: "qrc:/icons/content/switch.png"
        }
        XToolBarPrimitiveItem {
            componentFile: "qrc:/xmas/xobjects/merge.qml"
            image: "qrc:/icons/content/merge.png"
        }
        XToolBarPrimitiveItem {
            componentFile: "qrc:/xmas/xobjects/sink.qml"
            image: "qrc:/icons/content/sink.png"
        }
        XToolBarPrimitiveItem {
            componentFile: "qrc:/xmas/xobjects/source.qml"
            image: "qrc:/icons/content/source.png"
        }
        ToolBarSeparator {}

        ToolButton {
            action: addLibraryCompositeAction
        }
        Rectangle{
            id:compositeListRect
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: "#F8F8F8"
            radius: 5
            border.color: "darkgrey"
            border.width: 2
            ListView {
                id:compositeLibrary
                anchors.fill: compositeListRect
                contentWidth: 40
                contentHeight: 40
                anchors.margins: 2
                orientation: ListView.Horizontal
                model: network.compositeLibrary
                snapMode: ListView.SnapOneItem
                highlightFollowsCurrentItem: true
                flickDeceleration: 200
                clip:true
                delegate:
                    XToolBarCompositeItem {
                    id:item
                    height:30
                    width:40
                    fillMode: Image.PreserveAspectFit
                    image: modelData.symbol
                    onRemove: if(!network.removeLibraryComposite(modelData.url)) removeCompositeFailedDialog.open()
                }
            }
        }

        //dummy filler
        Item{Layout.preferredWidth: 200}
    }

    Action {
        id: packetAction
        text: "Packet"
        shortcut: ""
        iconSource: "qrc:/icons/content/packet.ico"
        iconName: "Packet"
        onTriggered: packetDialog.show()
    }

    Action {
        id: addLibraryCompositeAction
        text: "Add composite"
        shortcut: ""
        iconSource: "qrc:/icons/content/add_composite.ico"
        iconName: "add-composite"
        onTriggered: addLibraryCompositeDialog.open()
    }

    XPacketDialog {
        id: packetDialog
        expression:network.packet
        onExpressionChanged: {
            network.packet = packetDialog.expression
        }
    }

    // Add composite dialog
    FileDialog {
        id: addLibraryCompositeDialog
        selectExisting: true
        selectFolder: false
        selectMultiple: false
        nameFilters: [
            "Model files (*.json)",
            "All files (*)"]
        onAccepted: if(!network.addLibraryComposite(fileUrl)) addLibraryCompositeFailedDialog.open()
    }

    // Add composite failed dialog
    MessageDialog{
        id:addLibraryCompositeFailedDialog
        title: "Add composite error"
        icon: StandardIcon.Warning
        text:  "Cannot add composite!"
               + "  Make sure file is a correct model."
        standardButtons: StandardButton.Ok
        onApply: this.destroy()
    }

    // Remove composite failed dialog
    MessageDialog{
        id:removeLibraryCompositeFailedDialog
        title: "Remove composite error"
        icon: StandardIcon.Warning
        text:  "Cannot remove composite!"
               + "  Make sure composite is not used in this model."
        standardButtons: StandardButton.Ok
        onApply: this.destroy()
    }

}
