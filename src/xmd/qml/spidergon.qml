import QtQuick 1.1
import XMAS 1.0

Component {
    id: comp
    width: 200
    height: 400
    name: "test composite"
    Image {source: "../images/svgs/composite.svg"}
    connectors: [
        Connector {
            x: 5; y: 50
            name: "a1"
        },
        Connector {
            x: 5; y:90
            name: "a2"

        },
        Connector {
            x: 5; y:130
            name: "a3"
        },
        Connector {
            x: 195; y:50
            name: "a4"

        },
        Connector {
            x: 195; y:90
            name: "a5"
        }
    ]
}
