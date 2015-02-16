import QtQuick 2.4
import "../content"
import XMAS 1.0

XComponent {
    width: 50
    height: 50
    type: "source"
    prefix: "src"
    property string fx:""
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
        title: "Enter expression for source " + name
        help:"Insert types of packets injected at this source.\n"
        //TODO : implement packet depend help
        //             (GlobalVars.packetType != null && GlobalVars.packetType.Count > 0 ? "The domain of all packets is available through PacketDomain.\nE.g.:\n\t{p in PacketDomain | p_X < 5 && p_Y > 2}" :
        //                                                                                 "The domain of all packets is available through PacketDomain.\nE.g.:\n\t{p in PacketDomain | p < 100}");
        onAccepted: fx = dialog.expression
    }
}
