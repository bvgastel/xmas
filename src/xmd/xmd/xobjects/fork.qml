import QtQuick 2.4
import XMAS.model 1.0 as Model

XComponent {
    id:component
    width: 100
    height: 100
    type: Model.XComponent.Fork
    prefix: "frk"
    Canvas {
        anchors.fill: parent
        antialiasing: false
        smooth:false
        onPaint: {
            var ctx = getContext('2d')
            ctx.strokeStyle = "black"
            ctx.lineWidth = 4.0
            ctx.moveTo(40 ,0)
            ctx.lineTo(40,100)
            ctx.moveTo(60 ,0)
            ctx.lineTo(60,100)
            //left port with arrow
            ctx.moveTo(10 ,50)
            ctx.lineTo(30,50)
            ctx.lineTo(30,45)
            ctx.lineTo(40-ctx.lineWidth,50)
            ctx.lineTo(30,55)
            ctx.lineTo(30,50)
            ctx.fill()
            //right ports
            ctx.moveTo(60 ,20)
            ctx.lineTo(90,20)
            ctx.moveTo(60 ,80)
            ctx.lineTo(90,80)
            ctx.stroke()
        }
    }
}






