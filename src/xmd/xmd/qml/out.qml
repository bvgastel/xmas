import QtQuick 2.4
import XMAS 1.0

XComponent {
    width: 50
    height: 30
    type: "out"
    prefix: "o"
    XPort {x: 0; y: 10; name: "o"; type:Xmas.Target}
    Canvas {
        anchors.fill: parent
        onPaint: {
            var ctx = getContext('2d')
            ctx.strokeStyle = "black"
            ctx.lineWidth = 3.0
            ctx.moveTo(10 ,15)
            ctx.lineTo(30,15)
            ctx.lineTo(30,5)
            ctx.lineTo(45,15)
            ctx.lineTo(30,25)
            ctx.lineTo(30,15)
            ctx.fill()
            ctx.stroke()
        }
    }
}
