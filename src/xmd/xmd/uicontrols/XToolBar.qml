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
            action: loadCompositeAction
        }
        Rectangle{
            id:compositeListRect
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.minimumWidth: 150
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
                header:Item{width:10; height: parent.height}
                headerPositioning:ListView.OverlayHeader
                footer:Item{width:10; height: parent.height}
                footerPositioning:ListView.OverlayFooter
                delegate:
                    XToolBarCompositeItem {
                    id:item
                    height:30
                    width:40
                    fillMode: Image.PreserveAspectFit
                    filename: modelData.filename
                    alias:modelData.alias
                    image: modelData.symbol
                    boxed: modelData.boxed
                    onRemove: if(!network.unloadComposite(modelData.filename)) unloadCompositeFailedDialog.open()
                }
            }
        }

        //dummy filler
       Item{Layout.preferredWidth: 10}
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
        id: loadCompositeAction
        text: "Load composite"
        shortcut: ""
        iconSource: "qrc:/icons/content/add_composite.ico"
        iconName: "load-composite"
        onTriggered: loadCompositeDialog.open()
    }

    XPacketDialog {
        id: packetDialog
        expression:network.packet
        onExpressionChanged: {
            network.packet = packetDialog.expression
        }
    }

    // Load composite dialog
    FileDialog {
        id: loadCompositeDialog
        selectExisting: true
        selectFolder: false
        selectMultiple: false
        folder: network.folder
        nameFilters: [
            "Model files (*.json)",
            "All files (*)"]
        onAccepted: {
            if(folder.toString()!==network.folder){
                wrongFolderDialog.open()
            } else {
                if(!network.loadComposite(fileUrl))
                    loadCompositeFailedDialog.open()
            }
        }
    }

    // Load composite failed dialog
    MessageDialog{
        id:loadCompositeFailedDialog
        title: "Load composite"
        icon: StandardIcon.Warning
        text:  "Cannot load composite network!"
               + "  Make sure file is a correct model."
        standardButtons: StandardButton.Ok
        onApply: this.destroy()
    }

    // Remove composite failed dialog
    MessageDialog{
        id:unloadCompositeFailedDialog
        title: "Remove composite"
        icon: StandardIcon.Warning
        text:  "Cannot unload composite!"
               + "  Make sure composite is not used in this model."
        standardButtons: StandardButton.Ok
        onApply: this.destroy()
    }

    // wrong composite folder dialog
    MessageDialog{
        id:wrongFolderDialog
        title: "Load composite"
        icon: StandardIcon.Warning
        text:  "Cannot load composite!"
               + "  Make sure composite is in the same folder of this model."
        standardButtons: StandardButton.Ok
        onApply:this.destroy()
    }
}
