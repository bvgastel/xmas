import QtQuick 2.4
import XMAS 1.0

XConnector {
    id:connector
    width: 20; height:20
    x:-10; y:-10
    z: 1
    Rectangle{
        color: connected ? "black" : "red"
        anchors.fill: parent
    }
    MouseArea {
        anchors.fill: parent
        hoverEnabled: true
        cursorShape: containsMouse ? Qt.CrossCursor : Qt.ArrowCursor
    }

}
