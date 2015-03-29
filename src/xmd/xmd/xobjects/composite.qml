import QtQuick 2.4
import XMAS.model 1.0 as Model

XComponent {
    id:component
    width: boxed ? 200 : 100
    height: Math.max(portsLeft.count,portsRight.count) * (portSpace + XPort.width) + 20
    type: Model.XComponent.Composite
    prefix: "C"
    property string url:""
    property string alias:"Composite"
    property string image:""
    property bool boxed: true
    property int portSpace: 20

//    inputports:[
    ports: [
        XPort {name: "t0"; type:Model.XPort.INPORT},
        //XPort {name: "t1"; type:Model.XPort.Target},
        //        XPort {name: "t2"; type:Model.XPort.Target},
        //        XPort {name: "t3"; type:Model.XPort.Target},
        //        XPort {name: "t4"; type:Model.XPort.Target},
        //        XPort {name: "t5"; type:Model.XPort.Target},
        //        XPort {name: "t6"; type:Model.XPort.Target},
        //        XPort {name: "t7"; type:Model.XPort.Target},
        //        XPort {name: "t8"; type:Model.XPort.Target},
        //        XPort {name: "t9"; type:Model.XPort.Target},
        //        XPort {name: "t10"; type:Model.XPort.Target},
        //        XPort {name: "t11"; type:Model.XPort.Target},
        //        XPort {name: "t12"; type:Model.XPort.Target},
        //        XPort {name: "t13"; type:Model.XPort.Target},
        //        XPort {name: "t14"; type:Model.XPort.Target},
        //        XPort {name: "t15"; type:Model.XPort.Target},
        //        XPort {name: "t16"; type:Model.XPort.Target},
        //        XPort {name: "t17"; type:Model.XPort.Target},
        //        XPort {name: "t18"; type:Model.XPort.Target},
        //        XPort {name: "t19"; type:Model.XPort.Target},
        //        XPort {name: "t20"; type:Model.XPort.Target},
        //        XPort {name: "t21"; type:Model.XPort.Target},
        //        XPort {name: "t22"; type:Model.XPort.Target},
        //        XPort {name: "t23"; type:Model.XPort.Target},
        //        XPort {name: "t24"; type:Model.XPort.Target},
        //        XPort {name: "t25"; type:Model.XPort.Target},
        //        XPort {name: "t26"; type:Model.XPort.Target},
        //        XPort {name: "t27"; type:Model.XPort.Target},
        //        XPort {name: "t28"; type:Model.XPort.Target},
        //        XPort {name: "t29"; type:Model.XPort.Target},
        //        XPort {name: "t30"; type:Model.XPort.Target},
        //        XPort {name: "t31"; type:Model.XPort.Target},
        //        XPort {name: "t32"; type:Model.XPort.Target},
        //        XPort {name: "t33"; type:Model.XPort.Target},
        //        XPort {name: "t34"; type:Model.XPort.Target},
        //        XPort {name: "t35"; type:Model.XPort.Target},
        //        XPort {name: "t36"; type:Model.XPort.Target},
        //        XPort {name: "t37"; type:Model.XPort.Target},
        //        XPort {name: "t38"; type:Model.XPort.Target},
        //        XPort {name: "t39"; type:Model.XPort.Target},
        //        XPort {name: "t40"; type:Model.XPort.Target},
        //        XPort {name: "t41"; type:Model.XPort.Target},
        //        XPort {name: "t42"; type:Model.XPort.Target},
        //        XPort {name: "t43"; type:Model.XPort.Target},
        //        XPort {name: "t44"; type:Model.XPort.Target},
        //        XPort {name: "t45"; type:Model.XPort.Target},
        //        XPort {name: "t46"; type:Model.XPort.Target},
        //        XPort {name: "t47"; type:Model.XPort.Target},
        //        XPort {name: "t48"; type:Model.XPort.Target},
        //        XPort {name: "t49"; type:Model.XPort.Target}
//    ]
//    outputports: [
        XPort {name: "i0"; type:Model.XPort.OUTPORT}
        //        XPort {name: "i1"; type:Model.XPort.Initiator}
    ]


    Rectangle {
        id: box
        anchors.fill: parent
        anchors.leftMargin: boxed ? 18 : 0
        anchors.rightMargin: boxed ? 18 : 0
        z:-1
        border.color: "black"
        border.width: boxed ? 4 : 0
        radius: 10
        color: boxed ? "white" : "transparent"
        Image {
            id:symbol
            fillMode: Image.Stretch
            source: image
            anchors.centerIn: parent
            onStatusChanged: if(symbol.status === Image.Error) boxed = true
            Text {
                text: symbol.status === Image.Error ? ("???\n" + alias) : alias
                visible: boxed
                anchors.centerIn: parent
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
        }
    }

    // Input ports (left)
    Column {
        id:columnLeft
        spacing: portSpace
        anchors.left: component.left
        anchors.verticalCenter: box.verticalCenter
        Repeater{
            id:portsLeft
            model:component.inputports
            delegate: XPort {
                name:modelData.name
                type:modelData.type
                nameAlignCenter: boxed
                Rectangle {
                    id:leftWire
                    color:"black"
                    z:-1
                    border.width: 0
                    height: 4
                    width: 15
                    visible: boxed
                    anchors.left: parent.horizontalCenter
                    anchors.verticalCenter: parent.verticalCenter
                }
            }
        }
    }

    // Output ports (right)
    Column {
        id:columnRight
        spacing: portSpace
        anchors.right: component.right
        anchors.verticalCenter: box.verticalCenter
        Repeater{
            id: portsRight
            model:component.outputports
            delegate: XPort {
                name:modelData.name
                type:modelData.type
                nameAlignCenter: boxed
                Rectangle {
                    id:rightWire
                    color:"black"
                    z:-1
                    border.width: 0
                    height: 4
                    width: 15
                    visible: boxed
                    anchors.left: parent.horizontalCenter
                    anchors.verticalCenter: parent.verticalCenter
                }
            }
        }
    }
}
