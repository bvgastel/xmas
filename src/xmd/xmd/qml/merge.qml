import QtQuick 2.4
import XMAS 1.0

XComponent {
    width: 200
    height: 200
    type: "merge"
    XPort {x: 0; y: 30; name: "a1"; type:Xmas.Target}
    XPort {x: 0; y:150; name: "a2"; type:Xmas.Target}
    XPort {x: 180; y:90; name: "a3"; type:Xmas.Initiator}
    Canvas {
        anchors.fill: parent
        antialiasing: false
        smooth: false
        onPaint: {
            var ctx = getContext('2d')
            ctx.strokeStyle = "black"
            ctx.lineWidth = 5.0
            ctx.moveTo(100 ,0)
            ctx.lineTo(100,200)
            //left top port with arrow
            ctx.moveTo(20 ,40)
            ctx.lineTo(80,40)
            ctx.lineTo(80,30)
            ctx.lineTo(100-ctx.lineWidth,40)
            ctx.lineTo(80,50)
            ctx.lineTo(80,40)
            ctx.fill()
            //left bottom port with arrow
            ctx.moveTo(20 ,160)
            ctx.lineTo(80,160)
            ctx.lineTo(80,150)
            ctx.lineTo(100-ctx.lineWidth,160)
            ctx.lineTo(80,170)
            ctx.lineTo(80,160)
            ctx.fill()
            //right port
            ctx.moveTo(100 ,100)
            ctx.lineTo(180,100)
            ctx.stroke()
        }
    }
}
