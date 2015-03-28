import QtQuick 2.4
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.3
import "qrc:/ui/uicontrols/"

Rectangle {
    id:toolbox
    z:1
    height:48
    gradient: Gradient {
        GradientStop { position: 0.0; color: "black" }
        GradientStop { position: 0.4; color: "grey" }
        GradientStop { position: 1.0; color: "black" }
    }

    RowLayout {
        //id: palette
        anchors.fill: parent
        anchors.margins: {top: 5; bottom: 5}
        spacing: 5
        ToolButton {
            action: packetAction
        }
        ToolBarSeparator {}
        XToolBarItem {
            componentFile: "qrc:/xmas/xobjects/queue.qml"
            image: "qrc:/icons/content/queue.png"
        }
        XToolBarItem {
            componentFile: "qrc:/xmas/xobjects/function.qml"
            image: "qrc:/icons/content/function.png"
        }
        XToolBarItem {
            componentFile: "qrc:/xmas/xobjects/fork.qml"
            image: "qrc:/icons/content/fork.png"
        }
        XToolBarItem {
            componentFile: "qrc:/xmas/xobjects/join.qml"
            image: "qrc:/icons/content/join.png"
        }
        XToolBarItem {
            componentFile: "qrc:/xmas/xobjects/switch.qml"
            image: "qrc:/icons/content/switch.png"
        }
        XToolBarItem {
            componentFile: "qrc:/xmas/xobjects/merge.qml"
            image: "qrc:/icons/content/merge.png"
        }
        XToolBarItem {
            componentFile: "qrc:/xmas/xobjects/sink.qml"
            image: "qrc:/icons/content/sink.png"
        }
        XToolBarItem {
            componentFile: "qrc:/xmas/xobjects/source.qml"
            image: "qrc:/icons/content/source.png"
        }
        ToolBarSeparator {}

        ToolButton {
            action: addCompositeAction
        }
        Rectangle{
            id:compositeListRect
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: "#F8F8F8"
            radius: 5
            border.color: "darkgrey"
            border.width: 2
            ListView {
                id:compositeList
                anchors.fill: compositeListRect
                contentWidth: 40
                contentHeight: 40
                anchors.margins: 2
                orientation: ListView.Horizontal
                model: network.composites
                snapMode: ListView.SnapOneItem
                highlightFollowsCurrentItem: true
                flickDeceleration: 200
                clip:true
                delegate:
                    XToolBarItem {
                    id:item
                    height:30
                    width:40
                    fillMode: Image.PreserveAspectFit
                    image: getImage(modelData.symbol)
                    componentFile: "qrc:/xmas/xobjects/composite.qml"
                }
            }
        }

        //dummy filler
        Item{Layout.preferredWidth: 200}
    }

    function getImage(symbol){
        if(symbol==="") {
            return "qrc:/icons/content/composite.png"
        } else {
            return "qrc:/symbols/content/symbols/" + symbol
        }
    }
}
