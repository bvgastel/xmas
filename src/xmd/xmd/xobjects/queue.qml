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
    height: 60
    type: Model.XComponent.Queue
    prefix: "q"
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
            ctx.rect(20,15,30,30)
            ctx.rect(50,15,30,30)
            ctx.moveTo(0 ,30)
            ctx.lineTo(20,30)
            ctx.moveTo(80,30)
            ctx.lineTo(100,30)
            ctx.stroke()
        }
    }


    //size input label & validator
    Rectangle{
        id:functionPlaceholder
        rotation: Math.abs(parent.rotation) > 135 && Math.abs(parent.rotation) < 225 ? -parent.rotation : 0
        anchors.top: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        height: fx.contentHeight
        width: Math.max(parent.width,fx.contentWidth)
        color: fx.acceptableInput ? "transparent" : "red"
        border.width: 0
        TextInput {
            id:fx
            text: xdata["capacity"] ? xdata["capacity"] : 0
            color: acceptableInput ? "green" : "white"
            inputMethodHints: Qt.ImhDigitsOnly
            validator: IntValidator{bottom: 1}
            wrapMode: TextInput.NoWrap
            font.pointSize : 12
            font.bold: true
            anchors.fill: parent
            horizontalAlignment: Qt.AlignRight
            verticalAlignment: Qt.AlignVCenter
            onEditingFinished: {doAccept(fx.acceptableInput) ; focus = false}
            onFocusChanged: focus ? selectAll(): editingFinished()
        }
    }

    function doAccept(ok){
       if(ok){
           xdata["capacity"] = fx.text
           console.log("text = " + fx.text + " xdata = " + xdata["capacity"])

       } else {
           fx.text = xdata["capacity"]
           console.log("text = " + fx.text + " xdata = " + xdata["capacity"])
       }
    }


}
