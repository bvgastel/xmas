import QtQuick 2.4
import XMAS 1.0

XComponent {
    width: 100
    height: 100
    type: "sink"
    XPort {x:45; y:20; name: "t"; type:Xmas.Target}
    Canvas {
        anchors.fill: parent
        onPaint: {
            var ctx = getContext('2d')
            ctx.strokeStyle = "black"
            ctx.lineWidth = 3.0
            ctx.moveTo(50,30)
            ctx.lineTo(50,60)
            ctx.moveTo(30,60)
            ctx.lineTo(70,60)
            ctx.moveTo(35,65)
            ctx.lineTo(65,65)
            ctx.moveTo(40,70)
            ctx.lineTo(60,70)
            ctx.stroke()
        }
    }
}
