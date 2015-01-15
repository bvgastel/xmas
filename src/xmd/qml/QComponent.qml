import QtQuick 2.4
import XMAS 1.0

//todo : implement common xmas component style

XComponent {

    property bool created: false

    id: comp
    width: 200
    height: 200

    MouseArea {
        anchors.fill: comp
        anchors.margins: 10
        drag.target: comp

        onWheel: {
            if (wheel.modifiers & Qt.ControlModifier) {
                comp.rotation -= wheel.angleDelta.y /120 * 90;
                if (Math.abs(comp.rotation) < 45)
                    comp.rotation = 0;
            }
        }

    }

}

