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
    visible: false
    modality: Qt.WindowModal
    flags: Qt.Dialog
    color: "darkgrey"

    property string help:""
    property string expression:""
    property alias validator: regex.regExp

    signal accepted()

    width:500
    height: 300
    minimumHeight: 200
    minimumWidth: 400
    maximumHeight: 500
    maximumWidth: 700

    onVisibleChanged: expr.text = expression
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
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.preferredHeight: 100
            Layout.minimumHeight: 50
            color: "white"
            TextInput{
                id:expr
                anchors.fill:parent
                anchors.margins: 10
                text: expression
                focus:true
                selectByMouse:true
                validator: RegExpValidator{id:regex ; regExp:/^(\S.*)$/}
                wrapMode: TextInput.WordWrap
                font.pointSize : 10
                onFocusChanged: if(focus)selectAll()
                onAccepted: okAction.trigger()
                Keys.onEscapePressed:cancelAction.trigger()
            }
        }
        RowLayout{
            spacing:10
            Layout.fillWidth: true
            Layout.preferredHeight: 30
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

    Action {
        id:cancelAction
        text: "Cancel"
        onTriggered: {dialog.close()
        }
    }
    Action {
        id: okAction
        text: "Ok"
        enabled: expr.acceptableInput
        onTriggered: {
            dialog.expression = expr.text
            dialog.accepted()
            dialog.close()
        }
    }
}






