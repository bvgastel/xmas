import Model 1.0
//import QtQuick 2.0 // for QPoint

ChipComponent {

    name : "testcomp1"
    network : "testnetwork1"
    x : 1
    y : 1
    orientation : Up
    func : "function"
    connectors : [

        Port {
            name : "testport1"
            rdy : "rdy1"
            compName : "compName1"
        },

        Port {
            name : "testport2"
            rdy : "rdy2"
            compName : "compName2"
        }
    ]
}
