import QtQuick 1.1
import XMAS 1.0

QComponent {
    id: comp
    width: 200
    height: 200
    name: "source"
    connectors: [
        Connector {
            x: 90; y: 140
            name: "a1"
        }
    ]
    Image {source: "../images/svgs/source.svg"}
}

