import QtQuick 2.4
import "../uicontrols"
import XMAS.model 1.0 as Model

XComponent {
    id:component
    width: 50
    height: 50
    type: Model.XComponent.Source
    prefix: "src"
    XPort{x:20 ; y:40; rotation:0; name:outputports[0].name; type:outputports[0].type}
    Canvas {
        anchors.fill: parent
        antialiasing: false
        smooth:false
        onPaint: {
            var ctx = getContext('2d')
            ctx.strokeStyle = "black"
            ctx.lineWidth = 4.0
            ctx.moveTo(25,45)
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
        help: "Insert types of packets injected at this source.\n"
            + "The integer operators +,-,*,/,%,^ logical operators &&,||,! and equality operators ==,<=,>=,<,>,!= can be used.\n"
            + "For additional operators, see the syntax for matching expressions.\n"
            + "E.g.: assume incoming packets have fields dst, src and colour. The following expression:\n"
            + "(dst > 4 ? src <= 4 : colour == 0) && src % 2 == 0"
        validator: /^(\S.*)$/
        onAccepted: component.expression = dialog.expression
    }
}
