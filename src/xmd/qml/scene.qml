import QtQuick 2.4
import QtQuick.Controls 1.0

Item {
    id: window
    height: 600; width: 800

    MouseArea {
        anchors.fill: window
        onClicked: window.focus = false;
    }

    ScrollView {
        anchors.fill: parent
        Item{
            id: scene
            z: 10
            //height: childrenRect.height
            //width: childrenRect.width

        }
    }
}

