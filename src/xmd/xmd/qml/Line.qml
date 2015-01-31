import QtQuick 2.4

// line
Rectangle {
    id: line
       width: 1 // default line thickness
       property alias length: line.height = 1 // length
       property enumeration orientation: Qt.Horizontal
       rotation: line.orientation === Qt.Horizontal ? 90 : 0
}

