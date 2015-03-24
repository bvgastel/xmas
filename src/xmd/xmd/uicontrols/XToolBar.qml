import QtQuick 2.4
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.3
import "qrc:/ui/uicontrols/"

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
        XToolBarItem {
            //TODO : this is just a test "XToolBarItem" an can be removed later
            componentFile: "qrc:/xmas/xobjects/composite.qml"
            image: "qrc:/icons/content/composite.png"
        }
        ToolButton {
            iconSource: "qrc:/icons/content/composite.png"
            iconName: "Composite"
            text: "Open Composite"
            onClicked: mainwindow.openComposite()
        }
        Item { Layout.fillWidth: true } // empty space

        //add right aligned items here

    }

}

