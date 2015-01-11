import QtQuick 1.1
import XMAS 1.0

Component {
    id: comp
    width: 200
    height: 200
    name: "output"
    Image {source: "../images/svgs/out.svg"}
    connectors: [
        Connector {
            x: 100; y: 30
            name: "a1"
        }
    ]
}

