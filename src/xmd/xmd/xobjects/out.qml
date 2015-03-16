import QtQuick 2.4
import XMAS.model 1.0 as Model

XComponent {
    id:component
    width: 50
    height: 30
    type: Model.XComponent.Out
    prefix: "o"
     //TODO : remove xport once it derives from xmas
    XPort {x:0; y:10; name: "i"; type:Model.XPort.Target}
    Canvas {
        anchors.fill: parent
        antialiasing: false
        smooth:false
        onPaint: {
            var ctx = getContext('2d')
            ctx.strokeStyle = "black"
            ctx.lineWidth = 4.0
            ctx.moveTo(5 ,15)
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
