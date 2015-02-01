import QtQuick 2.4

XComponent {
    width: 200
    height: 200
    type: "queue"
    XPort {x: 0; y: 90; name: "a1"}
    XPort {x: 180; y:90; name: "a2"}
    Canvas {
        anchors.fill: parent
        onPaint: {
            var ctx = getContext('2d')
            ctx.strokeStyle = "black"
            ctx.lineWidth = 5.0
            ctx.rect(40,70,60,60)
            ctx.rect(100,70,60,60)
            ctx.moveTo(20 ,100)
            ctx.lineTo(40,100)
            ctx.moveTo(160,100)
            ctx.lineTo(180,100)
            ctx.stroke()
        }
    }
}
