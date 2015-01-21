import Model 1.0

Network {
    name : "network1"
//    board :
//        Board {
//            network: "network1"
//            width:5
//            height:5
//        }

    components : [
        ChipComponent {

            name : "testcomp1"
            network : "network1"
            x : 1
            y : 1
            orientation : Up
            func : "function"
            connectors : [

                Port {
                    name : "in1"
                    rdy : "rdy1"
                    compName : "compName1"
                },

                Port {
                    name : "out1"
                    rdy : "rdy2"
                    compName : "compName2"
                }
            ]
        },
        ChipComponent {

            name : "testcomp2"
            network : "network1"
            x : 3
            y : 1
            orientation : Up
            func : "function"
            connectors : [

                Port {
                    name : "in1"
                    rdy : "rdy1"
                    compName : "compName1"
                },

                Port {
                    name : "out1"
                    rdy : "rdy2"
                    compName : "compName2"
                }
            ]
        }
    ]

//    channels: [
//        Channel {
//            name : "testchannel1"
//            network : "network1"
//            initiator : "testcomp1"
//            init_port : "out1"
//            target : "testcomp2"
//            target_port : "in1"
//            datatype : "test_datatype1"

//            ptList : [
//                GridPoint { x:2; y:2 },
//                GridPoint { x:3; y:2 }
//            ]

//        }
//    ]

}
