import QtQuick 2.4
import "../content"
import XMAS 1.0

XComponent {
    width: 100
    height: 50
    type: "function"
    prefix: "f"
    XPort {x: 0; y: 20; name: "i"; type:Xmas.Target}
    XPort {x: 90; y:20; name: "o"; type:Xmas.Initiator}
    Canvas {
        anchors.fill: parent
        onPaint: {
            var ctx = getContext('2d')
            ctx.strokeStyle = "black"
            ctx.lineWidth = 3.0
            ctx.moveTo(10 ,25)
            ctx.lineTo(90,25)
            ctx.moveTo(30 ,5)
            ctx.lineTo(70,45)
            ctx.stroke()
        }
    }

    withDialog: true
    onShowDialog: dialog.visible = true
    XDialog {
        id: dialog
        title: "Enter expression for function " + name
        help:"Insert function. The language is a subset of C with:\n"
             + "\t-math operators +,-,*,/,%\n"
             + "\t-logical operators &&,||,!\n"
             + "\t-equality operators ==,<=,>=,<,>\n"
             + "E.g.:\n\t"
        //TODO : implement packet depend help
        // +  (GlobalVars.packetType != null && GlobalVars.packetType.Count > 0 ? "ret_X = 10; ret_Y = p_Y + 1;" : "ret = 0;");
        validator: /^(\S.*)$/
        onAccepted: param = dialog.expression

    }
}
