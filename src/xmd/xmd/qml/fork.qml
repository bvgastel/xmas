import QtQuick 2.4

XComponent {
    width: 200
    height: 200
    type: "fork"
    XPort {x: 0; y: 90; name: "t0"}
    XPort {x: 180; y:30; name: "t1"}
    XPort {x: 180; y:150; name: "i0"}
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
            //left port with arrow
            ctx.moveTo(20 ,100)
            ctx.lineTo(60,100)
            ctx.lineTo(60,90)
            ctx.lineTo(80-ctx.lineWidth,100)
            ctx.lineTo(60,110)
            ctx.lineTo(60,100)
            ctx.fill()
            //right ports
            ctx.moveTo(120 ,40)
            ctx.lineTo(180,40)
            ctx.moveTo(120 ,160)
            ctx.lineTo(180,160)
            ctx.stroke()
        }
    }
}


