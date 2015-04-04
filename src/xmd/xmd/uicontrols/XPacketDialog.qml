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
import QtQuick.Layouts 1.1
import QtQuick.Window 2.2

Window {
    id:dialog

    // Properties
    visible: false
    modality: Qt.WindowModal
    flags: Qt.Dialog
    color: "darkgrey"
    title: "Packet Dialog"
    width: 300
    height: 260
    minimumHeight: height
    minimumWidth: width
    maximumHeight: height
    maximumWidth: width

    property string expression:""

    // Signals
    signal accepted

    // Event handling
    onVisibleChanged:{
        expressionTextEdit.text = expression
        expressionTextEdit.forceActiveFocus()
    }

    FocusScope{
        anchors.fill: parent
        focus:true
        Keys.onEscapePressed:cancelAction.trigger()

        // Content
        ColumnLayout {
            anchors.fill: parent
            spacing:5
            anchors.margins: 5
            // help
            RowLayout{
                Rectangle{
                    Layout.fillWidth: true
                    Layout.minimumHeight: 25
                    Layout.maximumHeight: 25
                    color: "darkgrey"
                    Label {
                        anchors.margins: 10
                        text: "Add one field<range per line."
                        wrapMode: Text.WordWrap
                        color: "black"
                    }
                }
            }
            // packet input
            RowLayout{
                Rectangle{
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    Layout.minimumHeight: 50
                    color: "white"
                    TextEdit{
                        id:expressionTextEdit
                        anchors.fill:parent
                        anchors.margins: 10
                        text: expression
                        focus:true
                        selectByMouse:true
                        wrapMode: TextInput.NoWrap
                        font.pointSize : 10
                        KeyNavigation.priority: KeyNavigation.BeforeItem
                        KeyNavigation.tab: okButton
                    }
                }
            }
            // buttons
            RowLayout{
                spacing:10
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignRight
                Button{
                    id:cancelButton
                    action:cancelAction
                    tooltip: ""
                    Keys.onReturnPressed: cancelAction.trigger()
                    KeyNavigation.tab: expressionTextEdit
                }
                Button {
                    id:okButton
                    isDefault: true
                    focus:true
                    action: okAction
                    tooltip: ""
                    Keys.onReturnPressed: okAction.trigger()
                    KeyNavigation.tab: cancelButton
                }
            }
        }

        // Actions
        Action {
            id:cancelAction
            text: "Cancel"
            onTriggered: dialog.close()
            tooltip: ""
        }
        Action {
            id: okAction
            text: "Ok"
            enabled: expressionTextEdit.text !== ""
            onTriggered: {
                dialog.expression = expressionTextEdit.text
                dialog.accepted()
                dialog.close()
            }
            tooltip: ""
        }
    }
}
