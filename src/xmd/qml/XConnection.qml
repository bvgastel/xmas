import QtQuick 2.4

//todo : implement common xmas connection style

Item {
    id: connection
    width: 1
    height: 1

    property XComponent comp1
    property XComponent comp2
    property XConnector conn1
    property XConnector conn2

    Canvas {
        id: canvas
        anchors.fill: connection.parent

        property real lastX
        property real lastY
        property color color: "black"

        onPaint: {
            var ctx = getContext('2d')
            ctx.lineWidth = 1.5
            ctx.strokeStyle = canvas.color
            ctx.beginPath()
            ctx.moveTo(lastX, lastY)
            lastX = area.mouseX
            lastY = area.mouseY
            ctx.lineTo(lastX, lastY)
            ctx.stroke()
        }
        MouseArea {
            id: area
            anchors.fill: parent
            onPressed: {
                canvas.lastX = mouseX
                canvas.lastY = mouseY
            }
            onPositionChanged: {
                canvas.requestPaint()
            }
        }
    }




}

