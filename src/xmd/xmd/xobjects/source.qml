import QtQuick 2.4
import QtQuick.Controls 1.3
import "qrc:/ui/uicontrols/"
import XMAS.model 1.0 as Model

XComponent {
    id:component
    width: 60
    height: 60
    type: Model.XComponent.Source
    prefix: "src"
    property bool required: true
    XPort{x:30 ; y:40; name:"o"; type:Model.XPort.OUTPORT}
    Canvas {
        anchors.fill: parent
        antialiasing: false
        smooth:false
        onPaint: {
            var ctx = getContext('2d')
            ctx.strokeStyle = "black"
            ctx.lineWidth = 4.0
            ctx.moveTo(30,40)
            ctx.lineTo(30,5)
            ctx.moveTo(10,5)
            ctx.lineTo(50,5)
            ctx.stroke()
        }
    }

    withValidMarker: true
    onShowDialog: dialog.show()
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

    Component.onCompleted: {
        //insert on top
        insertMenuSeparator()
        insertMenuItem(menuExpression)
        insertMenuItem(menuRequired)
    }

    // context menu required item
    MenuItem {
        id:menuRequired
        text: "Required"
        checkable: true
        checked: component.required
        onCheckedChanged: component.required = checked
    }
    // context menu expression item
    MenuItem {
        id:menuExpression
        text: "Expression..."
        onTriggered: dialog.show()
    }
}
