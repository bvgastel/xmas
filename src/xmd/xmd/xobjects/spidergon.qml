import QtQuick 2.4
import XMAS.model 1.0 as Model
//this is a test composite

XComponent {
    id:component
    width: 100
    height: 90
    type: Model.XComponent.Composite
    prefix: "c"
    XPort {x: 0; y: 15; name: "t0"; type:Model.XPort.Target}
    XPort {x: 0; y: 35; name: "t1"; type:Model.XPort.Target}
    XPort {x: 0; y: 55; name: "t2"; type:Model.XPort.Target}
    XPort {x: 90; y: 15; name: "i0"; type:Model.XPort.Initiator}
    XPort {x: 90; y: 35; name: "i1"; type:Model.XPort.Initiator}
    Rectangle {
        anchors.horizontalCenter: parent.horizontalCenter
        y:5
        width: 60
        height: 80
        border.color: "black"
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
        antialiasing: false
        smooth:false
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
