import QtQuick 2.4

XComponent {
    width: 200
    height: 200
    type: "sink"
    XPort {x: 90; y: 40; name: "a1"}
    Image {source: "../content/images/svgs/sink.svg"}
}
