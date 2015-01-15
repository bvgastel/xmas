import QtQuick 2.4
import XMAS 1.0

QComponent {
    id: comp
    width: 200
    height: 200
    name: "sink"
    connectors: [
        Connector {
            x: 90; y: 40
            name: "a1"
        }
    ]
    Image {source: "../content/images/svgs/sink.svg"}
}

