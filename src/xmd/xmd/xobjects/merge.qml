import QtQuick 2.4
import XMAS.model 1.0 as Model

XComponent {
    id:component
    width: 100
    height: 100
    type: Model.XComponent.Merge
    prefix: "m"
    XPort {x: 0; y: 15; name: "a"; type:Model.XPort.Target}
    XPort {x: 0; y:75; name: "b"; type:Model.XPort.Target}
    XPort {x: 90; y:45; name: "o"; type:Model.XPort.Initiator}
    Canvas {
        anchors.fill: parent
        antialiasing: false
        smooth:false
        onPaint: {
            var ctx = getContext('2d')
            ctx.strokeStyle = "black"
            ctx.lineWidth = 3.0
            ctx.moveTo(50,0)
            ctx.lineTo(50,100)
            //left top port with arrow
            ctx.moveTo(10,20)
            ctx.lineTo(40,20)
            ctx.lineTo(40,15)
            ctx.lineTo(50-ctx.lineWidth,20)
            ctx.lineTo(40,25)
            ctx.lineTo(40,20)
            ctx.fill()
            //left bottom port with arrow
            ctx.moveTo(10,80)
            ctx.lineTo(40,80)
            ctx.lineTo(40,75)
            ctx.lineTo(50-ctx.lineWidth,80)
            ctx.lineTo(40,85)
            ctx.lineTo(40,80)
            ctx.fill()
            //right port
            ctx.moveTo(50,50)
            ctx.lineTo(90,50)
            ctx.stroke()
        }
    }
}
