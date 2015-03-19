import QtQuick 2.4
import "../uicontrols"
import XMAS.model 1.0 as Model

XComponent {
    id:component
    width: 100
    height: 50
    type: Model.XComponent.Function
    prefix: "f"
    XPort{x:0 ; y:20; name:"i"; type:Model.XPort.INPORT}
    XPort{x:90 ; y:20; name:"o"; type:Model.XPort.OUTPORT}
    Canvas {
        anchors.fill: parent
        antialiasing: false
        smooth:false
        onPaint: {
            var ctx = getContext('2d')
            ctx.strokeStyle = "black"
            ctx.lineWidth = 4.0
            ctx.moveTo(5 ,25)
            ctx.lineTo(95,25)
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
        help:"Insert function.\n"
             + "The integer operators +,-,*,/ can be used. "
             + "For additional operators, see the syntax for modifying expressions.\n"
             + "E.g.: assume incoming packets have fields dst, src and colour. The following expression:\n"
             + "\t dst := src, colour := 1\n"
             + "transforms an incoming packet by setting the destination to its original source,"
             + " by setting its colour to 1, and by removing the src field."
        validator: /^(\S.*)$/
        onAccepted: component.expression = dialog.expression
    }
}
