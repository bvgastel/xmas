import QtQuick 2.4
import XMAS 1.0

XComponent {
    width: 200
    height: 200
    type: "output"
    XPort {x: 0; y: 90; name: "a1"; type:Xmas.Target}
    Canvas {
        anchors.fill: parent
        antialiasing: false
        smooth: false
        onPaint: {
            var ctx = getContext('2d')
            ctx.strokeStyle = "black"
            ctx.lineWidth = 5.0
            ctx.moveTo(20 ,100)
            ctx.lineTo(75,100)
            ctx.lineTo(75,75)
            ctx.lineTo(125,100)
            ctx.lineTo(75,125)
            ctx.lineTo(75,100)
            ctx.fill()
            ctx.stroke()
        }
    }
}
