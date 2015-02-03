import QtQuick 2.4

XComponent {
    width: 200
    height: 200
    type: "sink"
    XPort {x: 90; y: 40; name: "a1"}
    Canvas {
        anchors.fill: parent
        antialiasing: false
        smooth: false
        onPaint: {
            var ctx = getContext('2d')
            ctx.strokeStyle = "black"
            ctx.lineWidth = 5.0
            ctx.moveTo(100 ,130)
            ctx.lineTo(100,60)
            ctx.moveTo(60,130)
            ctx.lineTo(140,130)
            ctx.moveTo(70,145)
            ctx.lineTo(130,145)
            ctx.moveTo(85,160)
            ctx.lineTo(115,160)
            ctx.stroke()
        }
    }
}
