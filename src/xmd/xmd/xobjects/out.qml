import QtQuick 2.4
import XMAS.model 1.0 as Model

XComponent {
    id:component
    width: 50
    height: 30
    type: Model.XComponent.Out
    prefix: "o"
    //TODO : remove outputports once they can be drived from xmas
    inputports: [
        XPort {name: "i"; type:Model.XPort.Target}
    ]
    Canvas {
        anchors.fill: parent
        antialiasing: false
        smooth:false
        onPaint: {
            var ctx = getContext('2d')
            ctx.strokeStyle = "black"
            ctx.lineWidth = 4.0
            ctx.moveTo(10 ,15)
            ctx.lineTo(30,15)
            ctx.lineTo(30,5)
            ctx.lineTo(45,15)
            ctx.lineTo(30,25)
            ctx.lineTo(30,15)
            ctx.fill()
            ctx.stroke()
        }
    }
}
