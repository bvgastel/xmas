import QtQuick 2.4
import XMAS.model 1.0 as Model

XComponent {
    id:component
    width: 100
    height: 80
    type: Model.XComponent.Queue
    prefix: "q"
    property int size: 0
    XPort{x:0 ; y:40; name:"i"; type:Model.XPort.INPORT}
    XPort{x:100 ; y:40; name:"o"; type:Model.XPort.OUTPORT}
    Canvas {
        anchors.fill: parent
        antialiasing: false
        smooth:false
        onPaint: {
            var ctx = getContext('2d')
            ctx.strokeStyle = "black"
            ctx.lineWidth = 4.0
            ctx.rect(20,25,30,30)
            ctx.rect(50,25,30,30)
            ctx.moveTo(0 ,40)
            ctx.lineTo(20,40)
            ctx.moveTo(80,40)
            ctx.lineTo(100,40)
            ctx.stroke()
        }
    }


    //size input label & validator
    Rectangle{
        id:functionPlaceholder
        rotation: Math.abs(parent.rotation) > 135 && Math.abs(parent.rotation) < 225 ? -parent.rotation : 0
        anchors.top: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        height: fx.contentHeight
        width: Math.max(parent.width,fx.contentWidth)
        color: fx.acceptableInput ? "transparent" : "red"
        border.width: 0
        TextInput {
            id:fx
            text: size
            color: acceptableInput ? "green" : "white"
            inputMethodHints: Qt.ImhDigitsOnly || Qt.ImhMultiLine
            validator: IntValidator{bottom: 1}
            wrapMode: TextInput.NoWrap
            font.pointSize : 12
            font.bold: true
            anchors.fill: parent
            horizontalAlignment: Qt.AlignRight
            verticalAlignment: Qt.AlignVCenter
            onAccepted: {component.size = text; component.expression = text; focus = false}
            onFocusChanged: if(focus)selectAll()
        }
    }

}
