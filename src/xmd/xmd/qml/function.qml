import QtQuick 2.4
import "../content"
import XMAS 1.0

XComponent {
    width: 100
    height: 100
    type: "function"
    XPort {x: 0; y: 45; name: "t"; type:Xmas.Target}
    XPort {x: 90; y:45; name: "i"; type:Xmas.Initiator}
    Canvas {
        anchors.fill: parent
        onPaint: {
            var ctx = getContext('2d')
            ctx.strokeStyle = "black"
            ctx.lineWidth = 3.0
            ctx.moveTo(10 ,50)
            ctx.lineTo(90,50)
            ctx.moveTo(30 ,30)
            ctx.lineTo(70,70)
            ctx.stroke()
        }
    }

    withDialog: true
    onShowDialog: dialog.visible = true
    XDialog {
        id: dialog
        title: name + " properties"

    }
}
