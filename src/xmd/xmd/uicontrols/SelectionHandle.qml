import QtQuick 2.0

MouseArea {
    id: handle
    width: 10
    height: 10
    property int cursor: Qt.ArrowCursor
    property int axis: Drag.XAndYAxis

    hoverEnabled: true
    preventStealing: true
    acceptedButtons: Qt.LeftButton | Qt.RightButton
    drag.axis: axis
    onEntered: cursorShape = cursor
    onExited: cursorShape = Qt.ArrowCursor
    Rectangle {
        anchors.fill: parent
        color:"white"
        border.color:"steelblue"
        border.width: 1
    }
}
