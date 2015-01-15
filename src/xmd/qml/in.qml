import QtQuick 2.4
import XMAS 1.0

QComponent {
    id: comp
    width: 200
    height: 200
    name: "input"
    connectors: [
        Connector {
            x: 90; y: 160
            name: "a1"
        }
    ]
    Image {source: "../images/svgs/in.svg"}

}

