import QtQuick 2.4
import XMAS 1.0

XComponent {
    width: 50
    height: 30
    type: "in"
    prefix: "i"
    XPort {x: 40; y: 10; name: "i"; type:Xmas.Initiator}
    Canvas {
        anchors.fill: parent
        onPaint: {
            var ctx = getContext('2d')
            ctx.strokeStyle = "black"
            ctx.lineWidth = 3.0
            ctx.moveTo(40,15)
            ctx.lineTo(20,15)
            ctx.lineTo(5,5)
            ctx.lineTo(5,25)
            ctx.lineTo(20,15)
            ctx.fill()
            ctx.stroke()
        }
    }
}
