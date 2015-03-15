import QtQuick 2.4
import XMAS.model 1.0 as Model

XComponent {
    id:component
    width: 50
    height: 50
    type: Model.XComponent.Sink
    prefix: "snk"
    topLabel: false
    Canvas {
        anchors.fill: parent
        antialiasing: false
        smooth:false
        onPaint: {
            var ctx = getContext('2d')
            ctx.strokeStyle = "black"
            ctx.lineWidth = 4.0
            ctx.moveTo(10,25)
            ctx.lineTo(35,25)
            ctx.moveTo(35,5)
            ctx.lineTo(35,45)
            ctx.moveTo(40,10)
            ctx.lineTo(40,40)
            ctx.moveTo(45,15)
            ctx.lineTo(45,35)
            ctx.stroke()
        }
    }
}
