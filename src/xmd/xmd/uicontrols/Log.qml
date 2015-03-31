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

Rectangle
{
    id:log
    color:"lightgrey"

    signal clear
    signal write(var message, var color)

    TextArea {
        id:textArea
        anchors.fill: parent
        readOnly: true
        font.pointSize: 10
        textFormat: Qt.RichText
        clip: true
        style: TextAreaStyle {
            backgroundColor: "transparent"
        }
        Connections{
            target:log
            onClear:{
                textArea.select(0,0)
                textArea.text = ""
            }

            onWrite:{
                if(color===undefined)color = "black"
                textArea.append("<font color=" + color + ">" + message + "</color>")
            }
        }
    }
}



