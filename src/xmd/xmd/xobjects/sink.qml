import QtQuick 2.4
import QtQuick.Controls 1.3
import XMAS.model 1.0 as Model

XComponent {
    id:component
    width: 60
    height: 60
    type: Model.XComponent.Sink
    prefix: "snk"
    topLabel: false
    property bool required: true
    XPort{x:30 ; y:0; rotation:180; name:"i"; type:Model.XPort.INPORT}
    Canvas {
        anchors.fill: parent
        antialiasing: false
        smooth:false
        onPaint: {
            var ctx = getContext('2d')
            ctx.strokeStyle = "black"
            ctx.lineWidth = 4.0
            ctx.moveTo(30,0)
            ctx.lineTo(30,35)
            ctx.moveTo(10,35)
            ctx.lineTo(50,35)
            ctx.moveTo(20,40)
            ctx.lineTo(40,40)
            ctx.moveTo(25,45)
            ctx.lineTo(35,45)
            ctx.stroke()
        }
    }

    Component.onCompleted: {
        //insert on top
        insertMenuSeparator()
        insertMenuItem(menuRequired)
    }

    // Sink context menu item
    MenuItem {
        id:menuRequired
        text: "Required"
        checkable: true
        checked: component.required
        onCheckedChanged: component.required = checked
    }
}
