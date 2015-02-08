import QtQuick 2.4
import XMAS 1.0

XComponent {
    width: 100
    height: 100
    type: "output"
    XPort {x: 0; y: 45; name: "a1"; type:Xmas.Target}
    Canvas {
        anchors.fill: parent
        antialiasing: false
        smooth: false
        onPaint: {
            var ctx = getContext('2d')
            ctx.strokeStyle = "black"
            ctx.lineWidth = 3.0
            ctx.moveTo(10 ,50)
            ctx.lineTo(40,50)
            ctx.lineTo(40,40)
            ctx.lineTo(60,50)
            ctx.lineTo(40,60)
            ctx.lineTo(40,50)
            ctx.fill()
            ctx.stroke()
        }
    }
}
