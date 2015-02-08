import QtQuick 2.4
import "../content"
import XMAS 1.0

XComponent {
    width: 100
    height: 100
    type: "fork"
    XPort {x: 0; y: 45; name: "t0"; type:Xmas.Target}
    XPort {x: 90; y:15; name: "t1"; type:Xmas.Initiator}
    XPort {x: 90; y:75; name: "i0"; type:Xmas.Initiator}
    Canvas {
        anchors.fill: parent
        antialiasing: false
        smooth: false
        onPaint: {
            var ctx = getContext('2d')
            ctx.strokeStyle = "black"
            ctx.lineWidth = 3.0
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

    onShowDialog: dialog.visible = true
    XDialog {
        id: dialog
        title: name + " properties"

    }
}


