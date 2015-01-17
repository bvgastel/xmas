import QtQuick 2.4


Rectangle {

    property string target

    id: toolbox

    height: 48
    gradient: Gradient {
             GradientStop { position: 0.0; color: "grey" }
             GradientStop { position: 1.0; color: "lightgrey" }
         }
    //color: "lightgrey"
    anchors { right: parent.right; top: parent.top; left: parent.left}

    Column {
        //anchors.fill: parent
        spacing: 8

        Rectangle {
            //width: palette.width + 10; height: palette.height + 10
            width: parent.width - 100 ; height: palette.height + 10
            border.color: "black"

            Row {
                id: palette
                //anchors.centerIn: parent

                spacing: 15

                ToolBarItem {
                    anchors.verticalCenter: parent.verticalCenter
                    componentFile: "../qml/queue.qml"
                    source: "images/svgs/queue.svg"
                }
                ToolBarItem {
                    anchors.verticalCenter: parent.verticalCenter
                    componentFile: "../qml/function.qml"
                    source: "images/svgs/function.svg"
                }
                ToolBarItem {
                    anchors.verticalCenter: parent.verticalCenter
                    componentFile: "../qml/fork.qml"
                    source: "images/svgs/fork.svg"
                }
                ToolBarItem {
                    anchors.verticalCenter: parent.verticalCenter
                    componentFile: "../qml/join.qml"
                    source: "images/svgs/join.svg"
                }
                ToolBarItem {
                    anchors.verticalCenter: parent.verticalCenter
                    componentFile: "../qml/switch.qml"
                    source: "images/svgs/switch.svg"
                }
                ToolBarItem {
                    anchors.verticalCenter: parent.verticalCenter
                    componentFile: "../qml/merge.qml"
                    source: "images/svgs/merge.svg"
                }
                ToolBarItem {
                    anchors.verticalCenter: parent.verticalCenter
                    componentFile: "../qml/sink.qml"
                    source: "images/svgs/sink.svg"
                }
                ToolBarItem {
                    anchors.verticalCenter: parent.verticalCenter
                    componentFile: "../qml/source.qml"
                    source: "images/svgs/source.svg"
                }
                ToolBarItem {
                    anchors.verticalCenter: parent.verticalCenter
                    componentFile: "../qml/in.qml"
                    source: "images/svgs/in.svg"
                }
                ToolBarItem {
                    anchors.verticalCenter: parent.verticalCenter
                    componentFile: "../qml/out.qml"
                    source: "images/svgs/out.svg"
                }
                ToolBarItem {
                    anchors.verticalCenter: parent.verticalCenter
                    componentFile: "../qml/spidergon.qml"
                    source: "images/svgs/composite.svg"
                }

            }

        }


    }
}

