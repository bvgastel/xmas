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
import XMAS.model 1.0 as Model

XComponent {
    id:component
    width: 100
    height: 100
    type: Model.XComponent.Merge
    prefix: "m"
    XPort{x:0 ; y:20; name:"a"; type:Model.XPort.INPORT}
    XPort{x:0 ; y:80; name:"b"; type:Model.XPort.INPORT}
    XPort{x:100 ; y:50; name:"o"; type:Model.XPort.OUTPORT}
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
            //left top port with arrow
            ctx.moveTo(0,20)
            ctx.lineTo(40,20)
            ctx.lineTo(40,15)
            ctx.lineTo(50-ctx.lineWidth,20)
            ctx.lineTo(40,25)
            ctx.lineTo(40,20)
            ctx.fill()
            //left bottom port with arrow
            ctx.moveTo(0,80)
            ctx.lineTo(40,80)
            ctx.lineTo(40,75)
            ctx.lineTo(50-ctx.lineWidth,80)
            ctx.lineTo(40,85)
            ctx.lineTo(40,80)
            ctx.fill()
            //right port
            ctx.moveTo(50,50)
            ctx.lineTo(100,50)
            ctx.stroke()
        }
    }
}
