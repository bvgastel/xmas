import QtQuick 2.4
import XMAS 1.0

XComponent {
    width: 100
    height: 100
    type: "join"
    XPort {x: 0; y: 15; name: "a1"; type:Xmas.Target}
    XPort {x: 0; y:75; name: "a2"; type:Xmas.Target}
    XPort {x: 90; y:45; name: "a3"; type:Xmas.Initiator}
    Canvas {
        anchors.fill: parent
        onPaint: {
            var ctx = getContext('2d')
            ctx.strokeStyle = "black"
            ctx.lineWidth = 3.0
            ctx.moveTo(40,0)
            ctx.lineTo(40,100)
            ctx.moveTo(60,0)
            ctx.lineTo(60,100)
            //left top port with arrow
            ctx.moveTo(10,20)
            ctx.lineTo(30,20)
            ctx.lineTo(30,15)
            ctx.lineTo(40-ctx.lineWidth,20)
            ctx.lineTo(30,25)
            ctx.lineTo(30,20)
            ctx.fill()
            //left bottom port with arrow
            ctx.moveTo(10 ,80)
            ctx.lineTo(30,80)
            ctx.lineTo(30,75)
            ctx.lineTo(40-ctx.lineWidth,80)
            ctx.lineTo(30,85)
            ctx.lineTo(30,80)
            ctx.fill()
            //right port
            ctx.moveTo(60,50)
            ctx.lineTo(90,50)
            ctx.stroke()
        }
    }
}
