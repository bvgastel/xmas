import QtQuick 2.4
import XMAS 1.0
//this is a test composite

XComponent {
    width: 200
    height: 200
    type: "composite"
    XPort {x: 0; y: 40; name: "a1"; type:Xmas.Target}
    XPort {x: 0; y: 80; name: "a2"; type:Xmas.Target}
    XPort {x: 0; y: 120; name: "a3"; type:Xmas.Target}
    XPort {x: 180; y: 40; name: "a4"; type:Xmas.Initiator}
    XPort {x: 180; y: 80; name: "a5"; type:Xmas.Initiator}
    Canvas {
        anchors.fill: parent
        antialiasing: false
        smooth: false
        onPaint: {
            var ctx = getContext('2d')
            ctx.strokeStyle = "black"
            ctx.lineWidth = 5.0
            ctx.rect(50,25,100,150)
            //left ports
            ctx.moveTo(20 ,50)
            ctx.lineTo(50,50)
            ctx.moveTo(20,90)
            ctx.lineTo(50,90)
            ctx.moveTo(20,130)
            ctx.lineTo(50,130)
            //right ports
            ctx.moveTo(150 ,50)
            ctx.lineTo(180,50)
            ctx.moveTo(150,90)
            ctx.lineTo(180,90)
            //C
            ctx.font = "64pt Impact, sans serif"
            ctx.textAlign = "center"
            ctx.fillStyle = "black"
            ctx.fillText("C", 100, 130)
            ctx.stroke()
        }
    }
}
