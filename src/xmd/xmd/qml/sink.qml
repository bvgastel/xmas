import QtQuick 2.4
import XMAS 1.0

XComponent {
    width: 50
    height: 50
    type: "sink"
    prefix: "snk"
    XPort {x:20; y:0; name: "t"; type:Xmas.Target}
    topLabel: false
    Canvas {
        anchors.fill: parent
        onPaint: {
            var ctx = getContext('2d')
            ctx.strokeStyle = "black"
            ctx.lineWidth = 3.0
            ctx.moveTo(25,10)
            ctx.lineTo(25,35)
            ctx.moveTo(5,35)
            ctx.lineTo(45,35)
            ctx.moveTo(10,40)
            ctx.lineTo(40,40)
            ctx.moveTo(15,45)
            ctx.lineTo(35,45)
            ctx.stroke()
        }
    }
}
