import QtQuick 2.4
import XMAS 1.0

XComponent {
    width: 100
    height: 100
    type: "switch"
    XPort {x:0; y:45; name: "a1"; type:Xmas.Target}
    XPort {x:90; y:15; name: "a2"; type:Xmas.Initiator}
    XPort {x:90; y:75; name: "a3"; type:Xmas.Initiator}
    Canvas {
        anchors.fill: parent
        onPaint: {
            var ctx = getContext('2d')
            ctx.strokeStyle = "black"
            ctx.lineWidth = 3.0
            ctx.moveTo(50,0)
            ctx.lineTo(50,100)
            //left port with arrow
            ctx.moveTo(10,50)
            ctx.lineTo(40,50)
            ctx.lineTo(40,45)
            ctx.lineTo(50-ctx.lineWidth,50)
            ctx.lineTo(40,55)
            ctx.lineTo(40,50)
            ctx.fill()
            //right ports
            ctx.moveTo(50,20)
            ctx.lineTo(90,20)
            ctx.moveTo(50,80)
            ctx.lineTo(90,80)
            ctx.stroke()
        }
    }
}
