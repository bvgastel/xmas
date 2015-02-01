import QtQuick 2.4

XComponent {
    width: 200
    height: 200
    type: "join"
    XPort {x: 0; y: 30; name: "a1"}
    XPort {x: 0; y:150; name: "a2"}
    XPort {x: 180; y:90; name: "a3"}
    Image {source: "../content/images/svgs/join.svg"}
}
