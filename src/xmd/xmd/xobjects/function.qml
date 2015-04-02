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
import "qrc:/ui/uicontrols/"
import XMAS.model 1.0 as Model

XComponent {
    id:component
    width: 100
    height: 60
    type: Model.XComponent.Function
    prefix: "f"
    XPort{x:0 ; y:30; name:"i"; type:Model.XPort.INPORT}
    XPort{x:100 ; y:30; name:"o"; type:Model.XPort.OUTPORT}
    Canvas {
        anchors.fill: parent
        antialiasing: false
        smooth:false
        onPaint: {
            var ctx = getContext('2d')
            ctx.strokeStyle = "black"
            ctx.lineWidth = 4.0
            ctx.moveTo(0 ,30)
            ctx.lineTo(100,30)
            ctx.moveTo(30 ,10)
            ctx.lineTo(70,50)
            ctx.stroke()
        }
    }

    withValidMarker: true
    onShowDialog: dialog.show()
    XDialog {
        id: dialog
        title: "Enter expression for function " + name
        help:"Insert function.\n"
             + "The integer operators +,-,*,/ can be used. "
             + "For additional operators, see the syntax for modifying expressions.\n"
             + "E.g.: assume incoming packets have fields dst, src and colour. The following expression:\n"
             + "\t dst := src, colour := 1\n"
             + "transforms an incoming packet by setting the destination to its original source,"
             + " by setting its colour to 1, and by removing the src field."
        validator: /^(\S.*)$/
        onAccepted: component.expression = dialog.expression
    }

    Component.onCompleted: {
        //insert on top
        insertMenuSeparator()
        insertMenuItem(menuExpression)
    }

    // context menu expression item
    MenuItem {
        id:menuExpression
        text: "Function..."
        onTriggered: dialog.show()
    }
}
