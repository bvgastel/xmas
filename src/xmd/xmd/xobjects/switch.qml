import QtQuick 2.4
import XMAS.model 1.0 as Model
import "../uicontrols"

XComponent {
    width: 100
    height: 100
    type: Model.XComponent.Switch
    prefix: "s"
    XPort {x:0; y:45; name: "i"; type:Model.XPort.Target}
    XPort {x:90; y:15; name: "a"; type:Model.XPort.Initiator}
    XPort {x:90; y:75; name: "b"; type:Model.XPort.Initiator}
    Canvas {
        anchors.fill: parent
        antialiasing: false
        smooth:false
        onPaint: {
            var ctx = getContext('2d')
            ctx.strokeStyle = "black"
            ctx.lineWidth = 3.0
            ctx.moveTo(50,0)
            ctx.lineTo(50,100)
            //left port with arrow
            ctx.moveTo(10,50)
            ctx.lineTo(40,50)
            ctx.lineTo(40,45)
            ctx.lineTo(50-ctx.lineWidth,50)
            ctx.lineTo(40,55)
            ctx.lineTo(40,50)
            ctx.fill()
            //right ports
            ctx.moveTo(50,20)
            ctx.lineTo(90,20)
            ctx.moveTo(50,80)
            ctx.lineTo(90,80)
            ctx.stroke()
        }
    }

    withDialog: true
    onShowDialog: dialog.visible = true
    XDialog {
        id: dialog
        title: "Enter expression for switch " + name
        help:   "Insert function. The language is a subset of C with:\n"
                + "\t-math operators +,-,*,/,%\n"
                + "\t-logical operators &&,||,!\n"
                + "\t-equality operators ==,<=,>=,<,>\n"
                + "E.g.:\n\treturn "
        //TODO : implement packet depend help
        //+ (GlobalVars.packetType != null && GlobalVars.packetType.Count > 0 ? "ret_X = 10; ret_Y = p_Y + 1;" : "ret = 0;");
        validator: /^(\S.*)$/
        onAccepted: param = dialog.expression
    }
}
