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
    height: 100
    type: Model.XComponent.Join
    prefix: "j"
    XPort{x:0 ; y:20; name:"a"; type:Model.XPort.INPORT}
    XPort{x:0 ; y:80; name:"b"; type:Model.XPort.INPORT}
    XPort{x:100 ; y:50; name:"o"; type:Model.XPort.OUTPORT}
    Canvas {
        id:canvas
        anchors.fill: parent
        antialiasing: false
        smooth:false
        property int token: -1
        onPaint: {
            var ctx = getContext('2d')
            ctx.strokeStyle = "black"
            ctx.lineWidth = 4.0
            ctx.clearRect(0, 0, 100, 100)
            ctx.beginPath()
            ctx.moveTo(40,0)
            ctx.lineTo(40,100)
            ctx.moveTo(60,0)
            ctx.lineTo(60,100)
            //left top port with arrow
            ctx.moveTo(0,20)
            ctx.lineTo(30,20)
            ctx.lineTo(30,15)
            ctx.lineTo(40-ctx.lineWidth,20)
            ctx.lineTo(30,25)
            ctx.lineTo(30,20)
            ctx.fill()
            //left bottom port with arrow
            ctx.moveTo(0 ,80)
            ctx.lineTo(30,80)
            ctx.lineTo(30,70)
            ctx.lineTo(40-ctx.lineWidth,80)
            ctx.lineTo(30,85)
            ctx.lineTo(30,80)
            ctx.fill()
            //right port
            ctx.moveTo(60,50)
            ctx.lineTo(100,50)
            //token
            ctx.moveTo(60 ,50)
            switch (token){
            case 0:
                ctx.lineTo(40,20)
                break
            case 1:
                ctx.lineTo(40,80)
                break
            }
            ctx.stroke()
        }
    }

    onExpressionChanged: {
        var result = -1
        if(!isNaN(this.expression)){
           result = parseInt(this.expression)
            // limit to two target ports
           if(result < 0 || result > 1) result = -1
        }
         canvas.token = result
         canvas.requestPaint()
    }

    withValidMarker: true
    onShowDialog: dialog.show()
    XDialog {
        id: dialog
        title: "Enter expression for join " + name
        help: "Type 0 or 1, indicating which in-port of the restrictive join is used for tokens,"
             + "or insert an expression for an unrestrictive join.\n"
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
        text: "Token..."
        onTriggered: dialog.show()
    }

}

