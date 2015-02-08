import QtQuick 2.4
import XMAS 1.0

XComponent {
    width: 100
    height: 100
    type: "merge"
    XPort {x: 0; y: 15; name: "t0"; type:Xmas.Target}
    XPort {x: 0; y:75; name: "t1"; type:Xmas.Target}
    XPort {x: 90; y:45; name: "i"; type:Xmas.Initiator}
    Canvas {
        anchors.fill: parent
        onPaint: {
            var ctx = getContext('2d')
            ctx.strokeStyle = "black"
            ctx.lineWidth = 3.0
            ctx.moveTo(50,0)
            ctx.lineTo(50,100)
            //left top port with arrow
            ctx.moveTo(10,20)
            ctx.lineTo(40,20)
            ctx.lineTo(40,15)
            ctx.lineTo(50-ctx.lineWidth,20)
            ctx.lineTo(40,25)
            ctx.lineTo(40,20)
            ctx.fill()
            //left bottom port with arrow
            ctx.moveTo(10,80)
            ctx.lineTo(40,80)
            ctx.lineTo(40,75)
            ctx.lineTo(50-ctx.lineWidth,80)
            ctx.lineTo(40,85)
            ctx.lineTo(40,80)
            ctx.fill()
            //right port
            ctx.moveTo(50,50)
            ctx.lineTo(90,50)
            ctx.stroke()
        }
    }
}
