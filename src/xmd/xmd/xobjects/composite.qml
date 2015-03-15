import QtQuick 2.4
import XMAS.model 1.0 as Model


//This is just a Test composite!!!!!!!!!!
XComponent {
    id:component
    width: 200
    type: Model.XComponent.Composite
    prefix: "test"
    boxVisible: true
    property string symbol: "../content/cut.png"

    inputports:[
        XPort {name: "t0"; type:Model.XPort.Target},
        XPort {name: "t1"; type:Model.XPort.Target},
        XPort {name: "t2"; type:Model.XPort.Target},
        XPort {name: "t3"; type:Model.XPort.Target},
        XPort {name: "t4"; type:Model.XPort.Target},
        XPort {name: "t5"; type:Model.XPort.Target},
        XPort {name: "t6"; type:Model.XPort.Target},
        XPort {name: "t7"; type:Model.XPort.Target},
        XPort {name: "t8"; type:Model.XPort.Target},
        XPort {name: "t9"; type:Model.XPort.Target},
        XPort {name: "t10"; type:Model.XPort.Target},
        XPort {name: "t11"; type:Model.XPort.Target},
        XPort {name: "t12"; type:Model.XPort.Target},
        XPort {name: "t13"; type:Model.XPort.Target},
        XPort {name: "t14"; type:Model.XPort.Target},
        XPort {name: "t15"; type:Model.XPort.Target},
        XPort {name: "t16"; type:Model.XPort.Target},
        XPort {name: "t17"; type:Model.XPort.Target},
        XPort {name: "t18"; type:Model.XPort.Target},
        XPort {name: "t19"; type:Model.XPort.Target},
        XPort {name: "t20"; type:Model.XPort.Target},
        XPort {name: "t21"; type:Model.XPort.Target},
        XPort {name: "t22"; type:Model.XPort.Target},
        XPort {name: "t23"; type:Model.XPort.Target},
        XPort {name: "t24"; type:Model.XPort.Target},
        XPort {name: "t25"; type:Model.XPort.Target},
        XPort {name: "t26"; type:Model.XPort.Target},
        XPort {name: "t27"; type:Model.XPort.Target},
        XPort {name: "t28"; type:Model.XPort.Target},
        XPort {name: "t29"; type:Model.XPort.Target},
        XPort {name: "t30"; type:Model.XPort.Target},
        XPort {name: "t31"; type:Model.XPort.Target},
        XPort {name: "t32"; type:Model.XPort.Target},
        XPort {name: "t33"; type:Model.XPort.Target},
        XPort {name: "t34"; type:Model.XPort.Target},
        XPort {name: "t35"; type:Model.XPort.Target},
        XPort {name: "t36"; type:Model.XPort.Target},
        XPort {name: "t37"; type:Model.XPort.Target},
        XPort {name: "t38"; type:Model.XPort.Target},
        XPort {name: "t39"; type:Model.XPort.Target},
        XPort {name: "t40"; type:Model.XPort.Target},
        XPort {name: "t41"; type:Model.XPort.Target},
        XPort {name: "t42"; type:Model.XPort.Target},
        XPort {name: "t43"; type:Model.XPort.Target},
        XPort {name: "t44"; type:Model.XPort.Target},
        XPort {name: "t45"; type:Model.XPort.Target},
        XPort {name: "t46"; type:Model.XPort.Target},
        XPort {name: "t47"; type:Model.XPort.Target},
        XPort {name: "t48"; type:Model.XPort.Target},
        XPort {name: "t49"; type:Model.XPort.Target}
    ]
    outputports: [
        XPort {name: "i0"; type:Model.XPort.Initiator},
        XPort {name: "i1"; type:Model.XPort.Initiator}
    ]
}
