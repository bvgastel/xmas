import QtQuick 1.1
import XMAS 1.0

QComponent {
    id: comp
    width: 200
    height: 200
    name: "function"
    connectors: [
        Connector {
            x: 0; y: 90
            name: "a1"
        },
        Connector {
            x: 180; y:90
            name: "a2"

        }
    ]
    Image {source: "../images/svgs/function.svg"}
}

