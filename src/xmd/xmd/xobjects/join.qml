import QtQuick 2.4
import "../uicontrols"
import XMAS.model 1.0 as Model

XComponent {
    id:component
    width: 100
    height: 100
    type: Model.XComponent.Join
    prefix: "j"
    XPort{x:0 ; y:15; name:"a"; type:Model.XPort.INPORT}
    XPort{x:0 ; y:75; name:"b"; type:Model.XPort.INPORT}
    XPort{x:90 ; y:45; name:"o"; type:Model.XPort.OUTPORT}
    Canvas {
        id:canvas
        anchors.fill: parent
        antialiasing: false
        smooth:false
        property int token: -1
        onPaint: {
            var ctx = getContext('2d')
            ctx.strokeStyle = "black"
            ctx.lineWidth = 4.0
            ctx.clearRect(0, 0, 100, 100)
            ctx.beginPath()
            ctx.moveTo(40,0)
            ctx.lineTo(40,100)
            ctx.moveTo(60,0)
            ctx.lineTo(60,100)
            //left top port with arrow
            ctx.moveTo(5,20)
            ctx.lineTo(30,20)
            ctx.lineTo(30,15)
            ctx.lineTo(40-ctx.lineWidth,20)
            ctx.lineTo(30,25)
            ctx.lineTo(30,20)
            ctx.fill()
            //left bottom port with arrow
            ctx.moveTo(5 ,80)
            ctx.lineTo(30,80)
            ctx.lineTo(30,75)
            ctx.lineTo(40-ctx.lineWidth,80)
            ctx.lineTo(30,85)
            ctx.lineTo(30,80)
            ctx.fill()
            //right port
            ctx.moveTo(60,50)
            ctx.lineTo(95,50)
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

    onExpressionChanged: {
        var result = -1
        if(!isNaN(this.expression)){
           result = parseInt(this.expression)
            // limit to two target ports
           if(result < 0 || result > 1) result = -1
        }
         canvas.token = result
         canvas.requestPaint()
    }

    withDialog: true
    onShowDialog: dialog.visible = true
    XDialog {
        id: dialog
        title: "Enter expression for join " + name
        help: "Type 0 or 1, indicating which in-port of the restrictive join is used for tokens,"
             + "or insert an expression for an unrestrictive join.\n"
        validator: /^(\S.*)$/
        onAccepted: component.expression = dialog.expression

    }

}

