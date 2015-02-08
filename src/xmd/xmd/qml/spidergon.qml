import QtQuick 2.4
import XMAS 1.0
//this is a test composite

XComponent {
    width: 100
    height: 100
    type: "composite"
    XPort {x: 0; y: 30; name: "a1"; type:Xmas.Target}
    XPort {x: 0; y: 50; name: "a2"; type:Xmas.Target}
    XPort {x: 0; y: 70; name: "a3"; type:Xmas.Target}
    XPort {x: 90; y: 30; name: "a4"; type:Xmas.Initiator}
    XPort {x: 90; y: 50; name: "a5"; type:Xmas.Initiator}
    Canvas {
        anchors.fill: parent
        antialiasing: false
        smooth: false
        onPaint: {
            var ctx = getContext('2d')
            ctx.strokeStyle = "black"
            ctx.lineWidth = 3.0
            ctx.rect(20,20,60,80-ctx.lineWidth)
            //left ports
            ctx.moveTo(10,35)
            ctx.lineTo(20,35)
            ctx.moveTo(10,55)
            ctx.lineTo(20,55)
            ctx.moveTo(10,75)
            ctx.lineTo(20,75)
            //right ports
            ctx.moveTo(80,35)
            ctx.lineTo(90,35)
            ctx.moveTo(80,55)
            ctx.lineTo(90,55)
            //C
            ctx.font = "32pt impact, sans serif"
            ctx.textAlign = "center"
            ctx.fillStyle = "black"
            ctx.fillText("C",50,70)
            ctx.stroke()
        }
    }
}
