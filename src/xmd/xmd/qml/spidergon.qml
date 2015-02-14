import QtQuick 2.4
import XMAS 1.0
//this is a test composite

XComponent {
    width: 100
    height: 90
    type: "composite"
    XPort {x: 0; y: 15; name: "t0"; type:Xmas.Target}
    XPort {x: 0; y: 35; name: "t1"; type:Xmas.Target}
    XPort {x: 0; y: 55; name: "t2"; type:Xmas.Target}
    XPort {x: 90; y: 15; name: "i0"; type:Xmas.Initiator}
    XPort {x: 90; y: 35; name: "i1"; type:Xmas.Initiator}
    Rectangle {
        anchors.horizontalCenter: parent.horizontalCenter
        y:5
        width: 60
        height: 80
        border.color: black
        border.width: 3
        radius: 10
        color: "white"
        Text {
            anchors.centerIn: parent
            font.pixelSize: 48
            font.bold: true
            text:"C"
        }
    }
    Canvas {
        anchors.fill: parent
        onPaint: {
            var ctx = getContext('2d')
            ctx.strokeStyle = "black"
            ctx.lineWidth = 3.0
            //left ports
            ctx.moveTo(10,20)
            ctx.lineTo(20,20)
            ctx.moveTo(10,40)
            ctx.lineTo(20,40)
            ctx.moveTo(10,60)
            ctx.lineTo(20,60)
            //right ports
            ctx.moveTo(80,20)
            ctx.lineTo(90,20)
            ctx.moveTo(80,40)
            ctx.lineTo(90,40)
            ctx.stroke()
        }
    }
}
