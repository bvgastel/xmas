import QtQuick 2.4
import XMAS.model 1.0 as Model
import "../uicontrols"

XComponent {
    id:component
    width: 100
    height: 100
    type: Model.XComponent.Switch
    prefix: "s"
    XPort{x:0 ; y:45; name:inputports[0].name; type:inputports[0].type}
    XPort{x:90 ; y:15; name:outputports[0].name; type:outputports[0].type}
    XPort{x:90 ; y:75; name:outputports[1].name; type:outputports[1].type}
    Canvas {
        anchors.fill: parent
        antialiasing: false
        smooth:false
        onPaint: {
            var ctx = getContext('2d')
            ctx.strokeStyle = "black"
            ctx.lineWidth = 4.0
            ctx.moveTo(50,0)
            ctx.lineTo(50,100)
            //left port with arrow
            ctx.moveTo(5,50)
            ctx.lineTo(40,50)
            ctx.lineTo(40,45)
            ctx.lineTo(50-ctx.lineWidth,50)
            ctx.lineTo(40,55)
            ctx.lineTo(40,50)
            ctx.fill()
            //right ports
            ctx.moveTo(50,20)
            ctx.lineTo(95,20)
            ctx.moveTo(50,80)
            ctx.lineTo(95,80)
            ctx.stroke()
        }
    }

    withDialog: true
    onShowDialog: dialog.visible = true
    XDialog {
        id: dialog
        title: "Enter expression for switch " + name
        help:   "Insert switching function\n"
                + "The integer operators +,-,*,/,%,^ logical operators &&,||,! and equality operators ==,<=,>=,<,>,!= can be used.\n"
                + "For additional operators, see the syntax for matching expressions.\n"
                + "E.g.: assume incoming packets have fields dst, src and colour. The following expression:\n"
                + "(dst > 4 ? src <= 4 : colour == 0) && src % 2 == 0"
        validator: /^(\S.*)$/
        onAccepted: component.expression = dialog.expression
    }
}
