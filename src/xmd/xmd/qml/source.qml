import QtQuick 2.4
import "../content"
import XMAS 1.0

XComponent {
    width: 50
    height: 50
    type: "source"
    XPort {x:20; y:40; name: "i"; type:Xmas.Initiator}
    Canvas {
        anchors.fill: parent
        onPaint: {
            var ctx = getContext('2d')
            ctx.strokeStyle = "black"
            ctx.lineWidth = 3.0
            ctx.moveTo(25,40)
            ctx.lineTo(25,5)
            ctx.moveTo(5,5)
            ctx.lineTo(45,5)
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
