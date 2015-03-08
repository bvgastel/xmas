import QtQuick 2.4
import XMAS.model 1.0 as Model

XComponent {
    width: 50
    height: 50
    type: Model.XComponent.Sink
    prefix: "snk"
    XPort {x:20; y:0; name: "i"; type:Model.XPort.Target}
    topLabel: false
    Canvas {
        anchors.fill: parent
        antialiasing: false
        smooth:false
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
