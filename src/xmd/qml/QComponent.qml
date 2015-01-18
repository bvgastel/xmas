import QtQuick 2.4
import XMAS 1.0

//todo : implement common xmas component style

// Remark g.: File name starts with Q: that is not advisable ....

XComponent {

    property bool created: false
    property bool selected: false

    id: comp
    width: 200
    height: 200

//    Keys.onDeletePressed: comp.destroy()
//    Keys.onLeftPressed: x=x-10
//    Keys.onRightPressed: x=x+10
//    Keys.onDownPressed: y=y+10
//    Keys.onUpPressed: y=y-10

    MouseArea {
        anchors.fill: comp
        anchors.margins: 10
        hoverEnabled: true
        acceptedButtons: Qt.LeftButton | Qt.RightButton
        drag.target: comp

        onClicked: {
            selected = !selected
            comp.focus = selected
        }

        //onExited: selection.visible = false

        //testing
        onDoubleClicked: comp.destroy()
        onWheel: {
            if (wheel.modifiers & Qt.ControlModifier) {
                comp.rotation -= wheel.angleDelta.y /120 * 90;
                if (Math.abs(comp.rotation) < 45)
                    comp.rotation = 0;
            }
        }
    }

    Rectangle {
        id: selection
        anchors.fill: comp
        anchors.margins: -2
        color: "lightsteelblue"
        border.color: "blue"
        border.width: 2
        visible: selected
        //opacity: 0.5
        z:-1
    }



}

