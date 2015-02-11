import QtQuick 2.4
import "../content"
import XMAS 1.0

XComponent {
    width: 100
    height: 100
    type: "source"
    XPort {x:45; y:70; name: "i"; type:Xmas.Initiator}
    Canvas {
        anchors.fill: parent
        onPaint: {
            var ctx = getContext('2d')
            ctx.strokeStyle = "black"
            ctx.lineWidth = 3.0
            ctx.moveTo(50,80)
            ctx.lineTo(50,25)
            ctx.moveTo(25,25)
            ctx.lineTo(75,25)
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
