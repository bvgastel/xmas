import QtQuick 2.4

XComponent {
    width: 200
    height: 200
    type: "switch"
    XPort {x: 0; y: 90; name: "a1"}
    XPort {x: 180; y: 30; name: "a2"}
    XPort {x: 180; y: 150; name: "a3"}
    Canvas {
        anchors.fill: parent
        onPaint: {
            var ctx = getContext('2d')
            ctx.strokeStyle = "black"
            ctx.lineWidth = 5.0
            ctx.moveTo(100 ,0)
            ctx.lineTo(100,200)
            //left port with arrow
            ctx.moveTo(20 ,100)
            ctx.lineTo(80,100)
            ctx.lineTo(80,90)
            ctx.lineTo(100-ctx.lineWidth,100)
            ctx.lineTo(80,110)
            ctx.lineTo(80,100)
            ctx.fill()
            //right ports
            ctx.moveTo(100 ,40)
            ctx.lineTo(180,40)
            ctx.moveTo(100 ,160)
            ctx.lineTo(180,160)
            ctx.stroke()
        }
    }
}
