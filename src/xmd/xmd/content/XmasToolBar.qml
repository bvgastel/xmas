import QtQuick 2.4
import QtQuick.Layouts 1.1

Rectangle {
    id:toolbox
    z:1
    gradient: Gradient {
        GradientStop { position: 0.0; color: "black" }
        GradientStop { position: 0.4; color: "grey" }
        GradientStop { position: 1.0; color: "black" }
    }

    RowLayout {
        id: palette
        anchors {left: parent.left ; top: parent.top ; bottom: parent.bottom}
        anchors.margins: {top: 5; bottom: 5}
        spacing: 10
        ToolBarItem {
            componentFile: "../qml/queue.qml"
            image: "images/queue.png"
        }
        ToolBarItem {
            componentFile: "../qml/function.qml"
            image: "images/function.png"
        }
        ToolBarItem {
            componentFile: "../qml/fork.qml"
            image: "images/fork.png"
        }
        ToolBarItem {
            componentFile: "../qml/join.qml"
            image: "images/join.png"
        }
        ToolBarItem {
            componentFile: "../qml/switch.qml"
            image: "images/switch.png"
        }
        ToolBarItem {
            componentFile: "../qml/merge.qml"
            image: "images/merge.png"
        }
        ToolBarItem {
            componentFile: "../qml/sink.qml"
            image: "images/sink.png"
        }
        ToolBarItem {
            componentFile: "../qml/source.qml"
            image: "images/source.png"
        }
        ToolBarItem {
            componentFile: "../qml/in.qml"
            image: "images/in.png"
        }
        ToolBarItem {
            componentFile: "../qml/out.qml"
            image: "images/out.png"
        }
        ToolBarItem {
            componentFile: "../qml/spidergon.qml"
            image: "images/composite.png"
        }

    }

}

