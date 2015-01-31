import QtQuick 2.4

XComponent {
    width: 200
    height: 200
    type: "switch"
    XConnector {x: 0; y: 90; name: "a1"}
    XConnector {x: 180; y: 30; name: "a2"}
    XConnector {x: 180; y: 150; name: "a3"}
    Image {source: "../content/images/svgs/switch.svg"}
}

