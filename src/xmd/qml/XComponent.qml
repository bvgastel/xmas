import QtQuick 2.4

Item {
    id: comp
    width: 200
    height: 200
    focus: true
    property bool created: false
    property bool selected: false
    property string type: "default"
    property string name: ""


    property int rightBound: comp.parent.width - comp.width
    property int bottomBound: comp.parent.height - comp.height
    property int step: 10

    Keys.onDeletePressed: comp.destroy()
    Keys.onLeftPressed: x - step < 0 ? x = 0 : x = x - step
    Keys.onRightPressed: x + step > rightBound ? x = rightBound : x = x + step
    Keys.onDownPressed: y + step > bottomBound ? y = bottomBound : y = y + step
    Keys.onUpPressed: y - step < 0 ? y = 0 : y = y - step

    MouseArea {
        anchors.fill: comp
        anchors.margins: 10
        hoverEnabled: true
        acceptedButtons: Qt.LeftButton | Qt.RightButton
        drag.target: comp
        drag.minimumX: 0
        drag.minimumY: 0
        drag.maximumX: comp.parent.width - comp.width
        drag.maximumY: comp.parent.height - comp.height

        onClicked: {
            selected = !selected
            scope.focus = !scope.focus
        }

        //onExited: selection.visible = false

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
        visible: scope.focus
        //opacity: 0.5
        z:-1
    }

    FocusScope {
        id: scope
        x: comp.x; y: comp.y
        width: comp.width; height: comp.height
    }
}
