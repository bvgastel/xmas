import QtQuick 2.4

XComponent {
    width: 200
    height: 200
    type: "function"
    XPort {x: 0; y: 90; name: "a1"}
    XPort {x: 180; y:90; name: "a2"}
    Canvas {
        anchors.fill: parent
        antialiasing: false
        smooth: false
        onPaint: {
            var ctx = getContext('2d')
            ctx.strokeStyle = "black"
            ctx.lineWidth = 5.0
            ctx.moveTo(20 ,100)
            ctx.lineTo(180,100)
            ctx.moveTo(65 ,125)
            ctx.lineTo(135,75)
            ctx.stroke()
        }
    }
}
