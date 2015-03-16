import QtQuick 2.4
import XMAS.model 1.0 as Model

XComponent {
    id:component
    width: 100
    height: 100
    type: Model.XComponent.Merge
    prefix: "m"
    XPort{x:0 ; y:15; name:inputports[0].name; type:inputports[0].type}
    XPort{x:0 ; y:75; name:inputports[1].name; type:inputports[1].type}
    XPort{x:90 ; y:45; name:outputports[0].name; type:outputports[0].type}
    Canvas {
        anchors.fill: parent
        antialiasing: false
        smooth:false
        onPaint: {
            var ctx = getContext('2d')
            ctx.strokeStyle = "black"
            ctx.lineWidth = 4.0
            ctx.moveTo(50,0)
            ctx.lineTo(50,100)
            //left top port with arrow
            ctx.moveTo(5,20)
            ctx.lineTo(40,20)
            ctx.lineTo(40,15)
            ctx.lineTo(50-ctx.lineWidth,20)
            ctx.lineTo(40,25)
            ctx.lineTo(40,20)
            ctx.fill()
            //left bottom port with arrow
            ctx.moveTo(5,80)
            ctx.lineTo(40,80)
            ctx.lineTo(40,75)
            ctx.lineTo(50-ctx.lineWidth,80)
            ctx.lineTo(40,85)
            ctx.lineTo(40,80)
            ctx.fill()
            //right port
            ctx.moveTo(50,50)
            ctx.lineTo(95,50)
            ctx.stroke()
        }
    }
}
