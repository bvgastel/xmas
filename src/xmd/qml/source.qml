import QtQuick 2.4

XComponent {
    width: 200
    height: 200
    type: "source"
    XConnector {x: 90; y: 140; name: "a1"}
    Image {source: "../content/images/svgs/source.svg"}
}
