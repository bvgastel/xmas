import QtQuick 1.1
import XMAS 1.0

Component {
    id: comp
    width: 200
    height: 200
    name: "source"
    Image {source: "../images/svgs/source.svg"}
    connectors: [
        Connector {
            x: 100; y: 150
            name: "a1"
        }
    ]
}

