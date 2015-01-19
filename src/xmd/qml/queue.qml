import QtQuick 2.4

XComponent {
    width: 200
    height: 200
    type: "queue"
    XConnector {x: 0; y: 90; name: "a1"}
    XConnector {x: 180; y:90; name: "a2"}
    Image {source: "../content/images/svgs/queue.svg"}
}
