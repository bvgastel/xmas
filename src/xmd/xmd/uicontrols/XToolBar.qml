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

    property bool runEnabled: true
    property bool stopEnabled: true

    signal run
    signal stop

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
        XToolBarItem {
            componentFile: "../xobjects/in.qml"
            image: "../content/in.png"
        }
        XToolBarItem {
            componentFile: "../xobjects/out.qml"
            image: "../content/out.png"
        }
        XToolBarItem {
            componentFile: "../xobjects/spidergon.qml"
            image: "../content/composite.png"
        }
        Item { Layout.fillWidth: true } // empty space

        Image {
            id:run
            source: runEnabled ? "../content/run_on.ico" : "../content/run_off.ico"
            Layout.alignment: Qt.AlignRight
            width: 32; height: 32
            Layout.preferredHeight: parent.height
            Layout.preferredWidth: parent.height
            MouseArea{
                anchors.fill:parent
                hoverEnabled: runEnabled
                onEntered: parent.source = "../content/run_glow.ico"
                onExited: parent.source = "../content/run_on.ico"
                onClicked: run
            }
        }

        Image {
            id:stop
            source: stopEnabled ? "../content/stop_on.ico" : "../content/stop_off.ico"
            Layout.alignment: Qt.AlignRight
            fillMode: Image.PreserveAspectFit
            width: 32; height: 32
            Layout.preferredHeight: parent.height
            Layout.preferredWidth: parent.height
            MouseArea{
                anchors.fill:parent
                hoverEnabled: stopEnabled
                onEntered: parent.source = "../content/stop_glow.ico"
                onExited: parent.source = "../content/stop_on.ico"
                onClicked: stop
            }
        }

        //add right aligned items here

    }

}

