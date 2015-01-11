import QtQuick 1.1
import XMAS 1.0

Component {
    id: comp
    width: 200
    height: 200
    name: "switch"
    Image {source: "../images/svgs/switch.svg"}
    connectors: [
        Connector {
            x: 5; y: 100
            name: "a1"
        },
        Connector {
            x: 195; y:40
            name: "a2"

        },
        Connector {
            x: 195; y:160
            name: "a3"
        }
    ]    
}

