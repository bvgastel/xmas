import QtQuick 1.1
import XMAS 1.0

XConnector {
    id:connector
    width: 20
    height: 20
    Rectangle{
        color: connected ? "black" : "red"
        anchors.fill: parent
        MouseArea {
            hoverEnabled: true
            //cursorShape: containsMouse ? Qt.CrossCursor : Qt.ArrowCursor
        }

    }

}

