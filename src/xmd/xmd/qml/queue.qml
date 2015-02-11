import QtQuick 2.4
import XMAS 1.0

XComponent {
    id:queue
    width: 100
    height: 50
    type: "queue"
    property int size: 0
    XPort {x:0; y:20; name: "t"; type:Xmas.Target}
    XPort {x:90; y:20; name: "i"; type:Xmas.Initiator}
    Canvas {
        anchors.fill: parent
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



    TextInput {
        text: size
        rotation: Math.abs(parent.rotation) > 135 && Math.abs(parent.rotation) < 225 ? -parent.rotation : 0
        color: size == 0 ? "red" : "blue"
        inputMethodHints: Qt.ImhDigitsOnly
        validator: IntValidator{bottom: 0}
        wrapMode: TextInput.NoWrap
        font.pointSize : 12
        anchors.left: parent.left
        anchors.top: parent.bottom
        onEditingFinished: {queue.size = text; focus = false}
        onFocusChanged: if(focus)selectAll()
    }



}
