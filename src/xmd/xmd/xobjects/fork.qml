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
    type: Model.XComponent.Fork
    prefix: "frk"

    XPort{x:0; y:50; name:"i" ; type:Model.XPort.INPORT}
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
            ctx.moveTo(40 ,0)
            ctx.lineTo(40,100)
            ctx.moveTo(60 ,0)
            ctx.lineTo(60,100)
            //left port with arrow
            ctx.moveTo(0 ,50)
            ctx.lineTo(30,50)
            ctx.lineTo(30,45)
            ctx.lineTo(40-ctx.lineWidth,50)
            ctx.lineTo(30,55)
            ctx.lineTo(30,50)
            ctx.fill()
            //right ports
            ctx.moveTo(60 ,20)
            ctx.lineTo(100,20)
            ctx.moveTo(60 ,80)
            ctx.lineTo(100,80)
            ctx.stroke()
        }
    }
}






