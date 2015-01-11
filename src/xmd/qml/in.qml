import QtQuick 1.1
import XMAS 1.0

Component {
    id: comp
    width: 200
    height: 200
    name: "input"
    Image {source: "../images/svgs/in.svg"}
    connectors: [
        Connector {
            x: 100; y: 170
            name: "a1"
        }
    ]
}

