import QtQuick 2.4

XComponent {
    width: 200
    height: 200
    type: "join"
    XPort {x: 0; y: 30; name: "a1"}
    XPort {x: 0; y:150; name: "a2"}
    XPort {x: 180; y:90; name: "a3"}
    Canvas {
        anchors.fill: parent
        antialiasing: false
        smooth: false
        onPaint: {
            var ctx = getContext('2d')
            ctx.strokeStyle = "black"
            ctx.lineWidth = 5.0
            ctx.moveTo(80 ,0)
            ctx.lineTo(80,200)
            ctx.moveTo(120 ,0)
            ctx.lineTo(120,200)
            //left top port with arrow
            ctx.moveTo(20 ,40)
            ctx.lineTo(60,40)
            ctx.lineTo(60,30)
            ctx.lineTo(80-ctx.lineWidth,40)
            ctx.lineTo(60,50)
            ctx.lineTo(60,40)
            ctx.fill()
            //left bottom port with arrow
            ctx.moveTo(20 ,160)
            ctx.lineTo(60,160)
            ctx.lineTo(60,150)
            ctx.lineTo(80-ctx.lineWidth,160)
            ctx.lineTo(60,170)
            ctx.lineTo(60,160)
            ctx.fill()
            //right port
            ctx.moveTo(120 ,100)
            ctx.lineTo(180,100)
            ctx.stroke()
        }
    }
}
