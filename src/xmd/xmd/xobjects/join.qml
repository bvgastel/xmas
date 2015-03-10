import QtQuick 2.4
import "../uicontrols"
import XMAS.model 1.0 as Model

XComponent {
    id:component
    width: 100
    height: 100
    type: Model.XComponent.Join
    prefix: "j"
    XPort {x: 0; y: 15; name: "a"; type:Model.XPort.Target}
    XPort {x: 0; y:75; name: "b"; type:Model.XPort.Target}
    XPort {x: 90; y:45; name: "o"; type:Model.XPort.Initiator}
    Canvas {
        id:canvas
        anchors.fill: parent
        antialiasing: false
        smooth:false
        property int token: 0
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
            case 1:
                ctx.lineTo(40,20)
                break
            case 2:
                ctx.lineTo(40,80)
                break
            }
            ctx.stroke()
        }
    }

    onExpressionChanged: {
        var result = 0
        if(!isNaN(this.expression)){
           result = parseInt(this.expression)
            // limit to two target ports
           if(result < 1 || result > 2) result = 0
        }
         canvas.token = result
         canvas.requestPaint()
    }

    withDialog: true
    onShowDialog: dialog.visible = true
    XDialog {
        id: dialog
        title: "Enter expression for join " + name
        help:"Insert expression for an unrestrictive join as a subset of c with:\n"
             + "\t-math operators +,-,*,/,%\n"
             + "\t-logical operators &&,||,!\n"
             + "\t-equality operators ==,<=,>=,<,>\n"
             + "E.g.:\n\t"
             + "Or enter a numeric value for a restrictive join\n"
             + " with 0 for the first input, 1 for the second."
        //TODO : implement packet depend help
        // +  (GlobalVars.packetType != null && GlobalVars.packetType.Count > 0 ? "ret_X = 10; ret_Y = p_Y + 1;" : "ret = 0;");
        validator: /^(\S.*)$/
        onAccepted: component.expression = dialog.expression

    }

}

