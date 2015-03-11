import QtQuick 2.4
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.3

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
        XToolBarItem {
            componentFile: "../xobjects/queue.qml"
            image: "../content/queue.png"
        }
        XToolBarItem {
            componentFile: "../xobjects/function.qml"
            image: "../content/function.png"
        }
        XToolBarItem {
            componentFile: "../xobjects/fork.qml"
            image: "../content/fork.png"
        }
        XToolBarItem {
            componentFile: "../xobjects/join.qml"
            image: "../content/join.png"
        }
        XToolBarItem {
            componentFile: "../xobjects/switch.qml"
            image: "../content/switch.png"
        }
        XToolBarItem {
            componentFile: "../xobjects/merge.qml"
            image: "../content/merge.png"
        }
        XToolBarItem {
            componentFile: "../xobjects/sink.qml"
            image: "../content/sink.png"
        }
        XToolBarItem {
            componentFile: "../xobjects/source.qml"
            image: "../content/source.png"
        }
        ToolBarSeparator {}
        XToolBarItem {
            componentFile: "../xobjects/in.qml"
            image: "../content/in.png"
        }
        XToolBarItem {
            componentFile: "../xobjects/out.qml"
            image: "../content/out.png"
        }
        ToolBarSeparator {}
        ToolButton {
            iconSource: "qrc:/content/composite.png"
            iconName: "Composite"
            text: "Open Composite"
            onClicked: mainwindow.openComposite()
        }
        Item { Layout.fillWidth: true } // empty space

        //add right aligned items here

    }

}

