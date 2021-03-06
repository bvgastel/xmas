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
import XMAS.model 1.0 as Model

XComponent {
    id:component
    width: 60
    height: 60
    type: Model.XComponent.Sink
    prefix: "snk"
    topLabel: false
    XPort{x:30 ; y:0; rotation:180; name:"i"; type:Model.XPort.INPORT}
    Canvas {
        anchors.fill: parent
        antialiasing: false
        smooth:false
        onPaint: {
            var ctx = getContext('2d')
            ctx.strokeStyle = "black"
            ctx.lineWidth = 4.0
            ctx.moveTo(30,0)
            ctx.lineTo(30,35)
            ctx.moveTo(10,35)
            ctx.lineTo(50,35)
            ctx.moveTo(20,40)
            ctx.lineTo(40,40)
            ctx.moveTo(25,45)
            ctx.lineTo(35,45)
            ctx.stroke()
        }
    }

    Component.onCompleted: {
        //insert on top
        insertMenuSeparator()
        insertMenuItem(menuRequired)
    }

    // Sink context menu item
    MenuItem {
        id:menuRequired
        text: "Required"
        checkable: true
        checked: component.required
        onCheckedChanged: component.required = checked
    }
}
