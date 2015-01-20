import QtQuick 2.4
import "../content/connectionCreation.js" as Code

Item {
    id:connector
    width: 20; height:20
    x:-10; y:-10
    z: 1
    property bool connected: false
    property string name: "a"

    Rectangle{
        color: connected ? "black" : "red"
        anchors.fill: parent
    }

    MouseArea {
        anchors.fill: parent
        hoverEnabled: true
        cursorShape: containsMouse ? Qt.CrossCursor : Qt.ArrowCursor
        acceptedButtons: Qt.LeftButton | Qt.RightButton
        onPressed: {
            if (mouse.button == Qt.LeftButton && !connected) {
                Code.beginConnecting(connector)
                connected = true
            }
        }
        onPositionChanged: if (connected) Code.continueConnecting(mouse);
        onReleased: {
            Code.endConnecting(mouse);
        }
    }
}



