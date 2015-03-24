import QtQuick 2.4
import XMAS.model 1.0 as Model

XComponent {
    id:component
    width: 50
    height: 30
    type: Model.XComponent.In
    prefix: "i"
    XPort {x:40; y:10; name: "o"; type:Model.XPort.OUTPORT}
    Canvas {
        anchors.fill: parent
        antialiasing: false
        smooth:false
        onPaint: {
            var ctx = getContext('2d')
            ctx.strokeStyle = "black"
            ctx.lineWidth = 4.0
            ctx.moveTo(45,15)
            ctx.lineTo(20,15)
            ctx.lineTo(5,5)
            ctx.lineTo(5,25)
            ctx.lineTo(20,15)
            ctx.fill()
            ctx.stroke()
        }
    }
}
