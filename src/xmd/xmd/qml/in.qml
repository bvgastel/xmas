import QtQuick 2.4
import XMAS 1.0

XComponent {
    width: 100
    height: 100
    type: "input"
    XPort {x: 90; y: 45; name: "t"; type:Xmas.Initiator}
    Canvas {
        anchors.fill: parent
        onPaint: {
            var ctx = getContext('2d')
            ctx.strokeStyle = "black"
            ctx.lineWidth = 3.0
            ctx.moveTo(90,50)
            ctx.lineTo(60,50)
            ctx.lineTo(40,40)
            ctx.lineTo(40,60)
            ctx.lineTo(60,50)
            ctx.fill()
            ctx.stroke()
        }
    }
}
