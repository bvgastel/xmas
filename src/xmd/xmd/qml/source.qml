import QtQuick 2.4

XComponent {
    width: 200
    height: 200
    type: "source"
    XPort {x: 90; y: 140; name: "a1"}
    Canvas {
        anchors.fill: parent
        onPaint: {
            var ctx = getContext('2d')
            ctx.strokeStyle = "black"
            ctx.lineWidth = 5.0
            ctx.moveTo(100 ,160)
            ctx.lineTo(100,50)
            ctx.moveTo(50,50)
            ctx.lineTo(150,50)
            ctx.stroke()
        }
    }
}
