import QtQuick 2.4

XComponent {
    width: 200
    height: 200
    type: "output"
    XConnector {x: 90; y: 20; name: "a1"}
    Image {source: "../content/images/svgs/out.svg"}
}
