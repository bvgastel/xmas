import QtQuick 2.4
import XMAS 1.0

XComponent {
    id:queue
    width: 100
    height: 100
    type: "queue"
    property int size: 0
    XPort {x:0; y:45; name: "a1"; type:Xmas.Target}
    XPort {x:90; y:45; name: "a2"; type:Xmas.Initiator}
    Canvas {
        anchors.fill: parent
        onPaint: {
            var ctx = getContext('2d')
            ctx.strokeStyle = "black"
            ctx.lineWidth = 3.0
            ctx.rect(20,35,30,30)
            ctx.rect(50,35,30,30)
            ctx.moveTo(10 ,50)
            ctx.lineTo(20,50)
            ctx.moveTo(80,50)
            ctx.lineTo(90,50)
            ctx.stroke()
        }
    }

    TextInput {
        text: size
        rotation: -parent.rotation
        color: size == 0 ? "red" : "blue"
        inputMethodHints: Qt.ImhDigitsOnly
        validator: IntValidator{bottom: 0}
        wrapMode: TextInput.NoWrap
        font.pointSize : 12
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        onEditingFinished: {queue.size = text; focus = false}
        onFocusChanged: if(focus)selectAll()
    }



}
