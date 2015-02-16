import QtQuick 2.4
import "../content"
import XMAS 1.0

XComponent {
    width: 100
    height: 100
    type: "join"
    prefix: "j"
    property int token: 0
    XPort {x: 0; y: 15; name: "t0"; type:Xmas.Target}
    XPort {x: 0; y:75; name: "t1"; type:Xmas.Target}
    XPort {x: 90; y:45; name: "i"; type:Xmas.Initiator}
    Canvas {
        id:canvas
        anchors.fill: parent
        onPaint: {
            var ctx = getContext('2d')
            ctx.strokeStyle = "black"
            ctx.lineWidth = 3.0
            ctx.clearRect(0, 0, 100, 100)
            ctx.beginPath()
            ctx.moveTo(40,0)
            ctx.lineTo(40,100)
            ctx.moveTo(60,0)
            ctx.lineTo(60,100)
            //left top port with arrow
            ctx.moveTo(10,20)
            ctx.lineTo(30,20)
            ctx.lineTo(30,15)
            ctx.lineTo(40-ctx.lineWidth,20)
            ctx.lineTo(30,25)
            ctx.lineTo(30,20)
            ctx.fill()
            //left bottom port with arrow
            ctx.moveTo(10 ,80)
            ctx.lineTo(30,80)
            ctx.lineTo(30,75)
            ctx.lineTo(40-ctx.lineWidth,80)
            ctx.lineTo(30,85)
            ctx.lineTo(30,80)
            ctx.fill()
            //right port
            ctx.moveTo(60,50)
            ctx.lineTo(90,50)
            //token
            ctx.moveTo(60 ,50)
            switch (token){
            case 0:
                ctx.lineTo(40,20)
                break
            case 1:
                ctx.lineTo(40,80)
                break
            }
            ctx.stroke()
        }
    }

    onShowDialog:{
        ++token
        if(token > 1)token=0
        canvas.requestPaint()
    }

}

