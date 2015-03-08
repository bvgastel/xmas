import QtQuick 2.4
import XMAS.model 1.0 as Model

XComponent {
    id:queue
    width: 100
    height: 50
    comptype: "queue"
    prefix: "q"
    property int size: isNaN(param) ? 0 : param
    XPort {x:0; y:20; name: "i"; type:Model.XPort.Target}
    XPort {x:90; y:20; name: "o"; type:Model.XPort.Initiator}
    Canvas {
        anchors.fill: parent
        antialiasing: false
        smooth:false
        onPaint: {
            var ctx = getContext('2d')
            ctx.strokeStyle = "black"
            ctx.lineWidth = 3.0
            ctx.rect(20,10,30,30)
            ctx.rect(50,10,30,30)
            ctx.moveTo(10 ,25)
            ctx.lineTo(20,25)
            ctx.moveTo(80,25)
            ctx.lineTo(90,25)
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
            onAccepted: {queue.size = text; param = text; focus = false}
            onFocusChanged: if(focus)selectAll()
        }
    }

}
