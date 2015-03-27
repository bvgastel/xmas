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
        XToolBarItem {
            componentFile: "qrc:/xmas/xobjects/in.qml"
            image: "qrc:/icons/content/in.png"
        }
        XToolBarItem {
            componentFile: "qrc:/xmas/xobjects/out.qml"
            image: "qrc:/icons/content/out.png"
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
                model: composites
                snapMode: ListView.SnapOneItem
                highlightFollowsCurrentItem: true
                flickDeceleration: 200
                clip:true
                delegate:
                    XToolBarItem {
                    height:30
                    width:40
                    fillMode: Image.PreserveAspectFit
                    image: Qt.resolvedUrl(symbol)
                    componentFile: "qrc:/xmas/xobjects/composite.qml"
                }
            }
        }

        //dummy filler
        Item{Layout.preferredWidth: 200}
    }

    // Composite list model
    //TODO remove test data
    ListModel {
        id: composites
        ListElement{url:""; alias:"a"; symbol:"qrc:/symbols/content/symbols/counter.png"}
        ListElement{url:""; alias:"b"; symbol:"qrc:/symbols/content/symbols/delay.png"}
        ListElement{url:""; alias:"c"; symbol:"qrc:/symbols/content/symbols/muxsource.png"}
        ListElement{url:""; alias:"d"; symbol:"qrc:/icons/content/composite.png"}
        ListElement{url:""; alias:"e"; symbol:"qrc:/icons/content/composite.png"}
        ListElement{url:""; alias:"f"; symbol:"qrc:/icons/content/composite.png"}
//        ListElement{url:""; alias:"g"; symbol:"qrc:/icons/content/composite.png"}
//        ListElement{url:""; alias:"h"; symbol:"qrc:/icons/content/composite.png"}
//        ListElement{url:""; alias:"i"; symbol:"qrc:/icons/content/composite.png"}
//        ListElement{url:""; alias:"j"; symbol:"qrc:/icons/content/composite.png"}
//        ListElement{url:""; alias:"k"; symbol:"qrc:/icons/content/composite.png"}
//        ListElement{url:""; alias:"l"; symbol:"qrc:/icons/content/composite.png"}
//        ListElement{url:""; alias:"m"; symbol:"qrc:/icons/content/composite.png"}
//        ListElement{url:""; alias:"n"; symbol:"qrc:/icons/content/composite.png"}
//        ListElement{url:""; alias:"o"; symbol:"qrc:/icons/content/composite.png"}
//        ListElement{url:""; alias:"p"; symbol:"qrc:/icons/content/composite.png"}
//        ListElement{url:""; alias:"q"; symbol:"qrc:/icons/content/composite.png"}
//        ListElement{url:""; alias:"r"; symbol:"qrc:/icons/content/composite.png"}
//        ListElement{url:""; alias:"s"; symbol:"qrc:/icons/content/composite.png"}
//        ListElement{url:""; alias:"t"; symbol:"qrc:/icons/content/composite.png"}
//        ListElement{url:""; alias:"u"; symbol:"qrc:/icons/content/composite.png"}
//        ListElement{url:""; alias:"v"; symbol:"qrc:/icons/content/composite.png"}
//        ListElement{url:""; alias:"w"; symbol:"qrc:/icons/content/composite.png"}
//        ListElement{url:""; alias:"x"; symbol:"qrc:/icons/content/composite.png"}
//        ListElement{url:""; alias:"y"; symbol:"qrc:/icons/content/composite.png"}
//        ListElement{url:""; alias:"z"; symbol:"qrc:/icons/content/composite.png"}
    }

}

