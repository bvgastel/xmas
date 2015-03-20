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
import Qt.labs.settings 1.0
import "../content"

ColumnLayout{
    spacing:0
    id:output
    property bool open
    property string status: "Ready"
    property int lastHeight
    property int headerHeight:25
    state: "down"

    signal writeLog(string text, color clr)

    function log(text,color){
        //TODO : check for valid color (As QColor.isValidColor(x))
        if(color === "" || color === null || color === undefined) color ="black"
       logList.append("<font color=" + color + ">" + text + "</color>")
     }

    Settings {
        category: "outputLog"
        property alias lastHeight: output.lastHeight
        property alias open: output.open
    }

    Rectangle
    {
        id:logHeader
        height: headerHeight
        Layout.fillWidth: true
        color:"darkgray"
        gradient: Gradient {
            GradientStop { position: 0.0; color: "black" }
            GradientStop { position: 0.4; color: "grey" }
            GradientStop { position: 1.0; color: "black" }
        }
        RowLayout{
            anchors.fill: parent
            anchors.leftMargin: 10
            anchors.rightMargin: 10
            Text {
                id:status
                Layout.alignment: Qt.AlignLeft
                color: "white"
                text:output.status
            }
            Image {
                id:arrow
                source: "../content/arrow.png"
                Layout.alignment: Qt.AlignRight
                MouseArea{
                    anchors.fill:parent
                    hoverEnabled: true
                    onEntered: arrow.source = "../content/arrowLit.png"
                    onExited: arrow.source = "../content/arrow.png"
                    onClicked: open = !open
                }
            }
        }
    }

    TextArea {
        id:logList
        z:-1 //to hide scrollbar when height is 0
        anchors.margins: 5
        Layout.fillWidth: true
        Layout.fillHeight: true
        readOnly: true
        font.pointSize: 10
        textFormat: Qt.RichText
        style: TextAreaStyle {
            backgroundColor: "lightgrey"
        }
    }

    states: [
        State {
            name: "up"
            when: !open
            PropertyChanges { target: arrow; rotation: 180 }
            PropertyChanges { target: output; height: headerHeight }
        }
        ,
        State {
            name: "down"
            when: open
            PropertyChanges { target: arrow; rotation: 0 }
            PropertyChanges { target: output; height: lastHeight }
        }
    ]

    transitions:
        Transition {
        PropertyAnimation { target: arrow; properties: "rotation"; duration: 250 }
        PropertyAnimation { target: output; properties: "height"; duration: 100 }
    }
}

