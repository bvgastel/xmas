import QtQuick 2.4

//this is a test composite

XComponent {
    width: 200
    height: 200
    type: "composite"
    XConnector {x: 0; y: 40; name: "a1"}
    XConnector {x: 0; y: 80; name: "a2"}
    XConnector {x: 0; y: 120; name: "a3"}
    XConnector {x: 180; y: 40; name: "a4"}
    XConnector {x: 180; y: 80; name: "a5"}
    Image {source: "../content/images/svgs/composite.svg"}
}
