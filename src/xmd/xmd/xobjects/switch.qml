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
import "qrc:/ui/uicontrols/"

XComponent {
    id:component
    width: 100
    height: 100
    type: Model.XComponent.Switch
    prefix: "s"
    XPort{x:0 ; y:50; name:"i"; type:Model.XPort.INPORT}
    XPort{x:100 ; y:20; name:"a"; type:Model.XPort.OUTPORT}
    XPort{x:100 ; y:80; name:"b"; type:Model.XPort.OUTPORT}
    Canvas {
        anchors.fill: parent
        antialiasing: false
        smooth:false
        onPaint: {
            var ctx = getContext('2d')
            ctx.strokeStyle = "black"
            ctx.lineWidth = 4.0
            ctx.moveTo(50,0)
            ctx.lineTo(50,100)
            //left port with arrow
            ctx.moveTo(0,50)
            ctx.lineTo(40,50)
            ctx.lineTo(40,45)
            ctx.lineTo(50-ctx.lineWidth,50)
            ctx.lineTo(40,55)
            ctx.lineTo(40,50)
            ctx.fill()
            //right ports
            ctx.moveTo(50,20)
            ctx.lineTo(100,20)
            ctx.moveTo(50,80)
            ctx.lineTo(100,80)
            ctx.stroke()
        }
    }

    withValidMarker: true
    onShowDialog: dialog.show()
    XExpressionDialog {
        id: dialog
        title: "Enter expression for switch " + name
        help:   "Insert switching function\n"
                + "The integer operators +,-,*,/,%,^ logical operators &&,||,! and equality operators ==,<=,>=,<,>,!= can be used.\n"
                + "For additional operators, see the syntax for matching expressions.\n"
                + "E.g.: assume incoming packets have fields dst, src and colour. The following expression:\n"
                + "(dst > 4 ? src <= 4 : colour == 0) && src % 2 == 0"
    }

    Component.onCompleted: {
        //insert on top
        insertMenuSeparator()
        insertMenuItem(menuExpression)
    }

    // context menu expression item
    MenuItem {
        id:menuExpression
        text: "Expression..."
        onTriggered: dialog.show()
    }
}
