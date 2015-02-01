import QtQuick 2.4

//this is a test composite

XComponent {
    width: 200
    height: 200
    type: "composite"
    XPort {x: 0; y: 40; name: "a1"}
    XPort {x: 0; y: 80; name: "a2"}
    XPort {x: 0; y: 120; name: "a3"}
    XPort {x: 180; y: 40; name: "a4"}
    XPort {x: 180; y: 80; name: "a5"}
    Image {source: "../content/images/svgs/composite.svg"}
}
