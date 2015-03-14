import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Layouts 1.1
import XMAS.model 1.0 as Model

XComponent {
    id:component
    width: 200
    height: calcHeight()
    type: Model.XComponent.Composite
    prefix: "test"
    property string symbol: "../content/cut.png"

    property list<XPort> lPorts: [
        XPort {name: "t0"; type:Model.XPort.Target},
        XPort {name: "t1"; type:Model.XPort.Target},
        XPort {name: "t2"; type:Model.XPort.Target},
        XPort {name: "t3"; type:Model.XPort.Target},
        XPort {name: "t4"; type:Model.XPort.Target},
        XPort {name: "t5"; type:Model.XPort.Target},
        XPort {name: "t6"; type:Model.XPort.Target},
        XPort {name: "t7"; type:Model.XPort.Target}
    ]
    property list<XPort> rPorts: [
        XPort {name: "i0"; type:Model.XPort.Initiator},
        XPort {name: "i1"; type:Model.XPort.Initiator}
    ]


//    property var lPorts: [
//        {name: "t0", type:Model.XPort.Target},
//        {name: "t1", type:Model.XPort.Target},
//        {name: "t2", type:Model.XPort.Target},
//        {name: "t3", type:Model.XPort.Target},
//        {name: "t4", type:Model.XPort.Target},
//        {name: "t5", type:Model.XPort.Target},
//        {name: "t6", type:Model.XPort.Target}
//    ]
//    property var rPorts: [
//        {name: "i0", type:Model.XPort.Initiator},
//        {name: "i1", type:Model.XPort.Initiator}
//    ]

    function calcHeight(){
        var cnt =  Math.max(rpLeft.count,rpRight.count)
        return cnt * 30 + 20
    }

    Column {
        id:columnLeft
        spacing: 20
        anchors.left: parent.left
        anchors.verticalCenter: parent.verticalCenter
        Repeater{
            id:rpLeft
            model:lPorts
            delegate: XPort {
                name:modelData.name
                type:modelData.type
                Rectangle {
                    id:wire
                    color:"black"
                    z:-1
                    border.width: 0
                    height: 4
                    width: 15
                    anchors.left: parent.horizontalCenter
                    anchors.verticalCenter: parent.verticalCenter
                }
                Text {
                    text:modelData.name
                    anchors.left: wire.right
                    anchors.leftMargin: 5
                }

            }
        }
    }

    Column {
        id:columnRight
        spacing: 20
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter
        Repeater{
            id: rpRight
            model:rPorts
            delegate: XPort {
                name:modelData.name
                type:modelData.type
                Rectangle {
                    color:"black"
                    z:-1
                    border.width: 0
                    height: 4
                    width: 15
                    anchors.left: parent.horizontalCenter
                    anchors.verticalCenter: parent.verticalCenter
                }

            }
        }
    }

    Rectangle {
        id:body
        anchors {left:columnLeft.right; right:columnRight.left; top: parent.top; bottom:parent.bottom}
        anchors.leftMargin: 10
        anchors.rightMargin: 10
        border.color: "black"
        border.width: 4
        radius: 10
        color: "white"
        Image{
           source: symbol
           anchors.centerIn: parent
           Layout.preferredHeight: 128
           Layout.preferredWidth: 128
        }
//        Text {
//            anchors.centerIn: parent
//            font.pixelSize: 12
//            font.bold: true
//            wrapMode: Text.Wrap
//            text:"Test"
//        }
    }
}
