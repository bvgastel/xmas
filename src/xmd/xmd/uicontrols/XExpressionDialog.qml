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
    width:500
    height: 300
    minimumHeight: 300
    minimumWidth: 400
    maximumHeight: 500
    maximumWidth: 700
    property string help:""

    // Signals
    signal accepted()

    // Event handling
    onVisibleChanged: {
        expressionTextInput.text = expression
        expressionTextInput.forceActiveFocus()
    }

    // Content
    FocusScope{
        anchors.fill: parent
        focus:true
        Keys.onEscapePressed:cancelAction.trigger()

        ColumnLayout {
            id: column
            anchors.fill: parent
            anchors.margins: 10
            spacing:5
            Rectangle{
                Layout.fillWidth: true
                Layout.preferredHeight: 150
                color: "darkgrey"
                Label {
                    anchors.fill:parent
                    anchors.margins: 10
                    text: help
                    wrapMode: Text.WordWrap
                    color: "black"
                }
            }
            Rectangle{
                id:textBackground
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.preferredHeight: 100
                Layout.minimumHeight: 50
                color: expressionValid ? "#CCFFFF" : "#FFCCCC"
                TextInput{
                    id:expressionTextInput
                    anchors.fill:parent
                    anchors.margins: 10
                    focus:true
                    selectByMouse:true
                    validator: RegExpValidator{id:regex ; regExp:/^(\S.*)$/}
                    wrapMode: TextInput.WordWrap
                    font.pointSize : 10
                    onFocusChanged: if(focus)selectAll()
                    onAccepted: okAction.trigger()
                    KeyNavigation.tab: okButton
                }
            }
            RowLayout{
                spacing:10
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignRight
                Button{
                    id:cancelButton
                    action:cancelAction
                    tooltip: ""
                    Keys.onReturnPressed: cancelAction.trigger()
                    KeyNavigation.tab: expressionTextInput
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
            onTriggered: {
                expression = expression //restore
                dialog.close()
            }
            tooltip: ""
        }
        Action {
            id: okAction
            text: "Ok"
            enabled: expressionTextInput.acceptableInput
            onTriggered: {
                expression = expressionTextInput.text
                if(expressionValid){
                    dialog.accepted()
                    dialog.close()
                } else {
                    var pos = expressionErrorPosition
                    expressionTextInput.select(pos,pos+1)
                    textBackground.color = "#FFCCCC"
                }
            }
            tooltip: ""
        }
    }
}
