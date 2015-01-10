import QtQuick 1.1
import XMAS 1.0

Component {
    id: comp
    width: 200
    height: 200
    name: "sink"
    Image {source: "../images/svgs/sink.svg"}
    connectors: [
        Connector {
            x: 100; y: 50
            name: "a1"
        }
    ]
}

