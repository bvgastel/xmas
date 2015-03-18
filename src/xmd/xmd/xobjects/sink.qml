import QtQuick 2.4
import XMAS.model 1.0 as Model

XComponent {
    id:component
    width: 50
    height: 50
    type: Model.XComponent.Sink
    prefix: "snk"
    topLabel: false

    // NOTE: Stefan, inports and outports are not known until component is fully constructed.
    XPort{x:20 ; y:0; rotation:180; //name:inputports[0].name; type:inputports[0].type

        onNameChanged: {        // NOTE: We need some way to indicatie inputports was constructed
            name: inputports[0].name;
        }
        onTypeChanged: {        // NOTE: We need some way to indicatie inputports was constructed
            type: inputports[0].type;
        }
    }
    Canvas {
        anchors.fill: parent
        antialiasing: false
        smooth:false
        onPaint: {
            var ctx = getContext('2d')
            ctx.strokeStyle = "black"
            ctx.lineWidth = 4.0
            ctx.moveTo(25,5)
            ctx.lineTo(25,35)
            ctx.moveTo(5,35)
            ctx.lineTo(45,35)
            ctx.moveTo(10,40)
            ctx.lineTo(40,40)
            ctx.moveTo(15,45)
            ctx.lineTo(35,45)
            ctx.stroke()
        }
    }
}
