import QtQuick 2.4
import XMAS 1.0

XComponent {
    width: 100
    height: 100
    type: "source"
    XPort {x:45; y:70; name: "a1"; type:Xmas.Initiator}
    Canvas {
        anchors.fill: parent
        antialiasing: false
        smooth: false
        onPaint: {
            var ctx = getContext('2d')
            ctx.strokeStyle = "black"
            ctx.lineWidth = 3.0
            ctx.moveTo(50,80)
            ctx.lineTo(50,25)
            ctx.moveTo(25,25)
            ctx.lineTo(75,25)
            ctx.stroke()
        }
    }
}
