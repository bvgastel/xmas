.pragma library

//testing javascript json library



function save(network) {


}




function toXjson(model){

    for (var child in model.children) {
        if(model.children[child].objectName==="component") {

        }
        if(model.children[child].objectName==="channel") {

        }
    }
}


function componentToXJsonObject(component){
    var obj = {
        "id":component.name,
        "type":component.type,
        "x":component.x,
        "y":component.y,
        "orientation":component.rotation,
        "scale":component.scale,
        "outs":[],
        "fields":[{"size":2}]
    }
    return obj
}

function jsonTest(){

    var object = {
        "VARS":[],
        "PACKET_TYPE":{},
        "COMPOSITE_OBJECTS":[],
        "NETWORK":[],
    }

    object.NETWORK.push(
                {
                    "id":"q1",
                    "type":"queue",
                    "x":100,
                    "y":50,
                    "orientation":90,
                    "scale":0.5,
                    "outs":[],
                    "fields":[{"size":2}]
                }
                )

    var jsonOrder = ["VARS","PACKET_TYPE","COMPOSITE_OBJECTS","NETWORK","id","type","x","y","orientation","scale","outs","fields","size"]

    var jsonFileText = JSON.stringify(object,jsonOrder,1)

    //throws exception if incorrect json
    var objFromJsonFile = JSON.parse(jsonFileText);


    console.log("======================================================================================")
    console.log("= Json text created from object")
    console.log("======================================================================================")
    console.log(jsonFileText)
    console.log("======================================================================================")
    console.log("= Object created from JSON text")
    console.log("======================================================================================")
    console.log("id=" + objFromJsonFile.NETWORK[0].id
                + ", type=" + objFromJsonFile.NETWORK[0].type
                + ",(x,y)=(" + objFromJsonFile.NETWORK[0].x + "," + objFromJsonFile.NETWORK[0].y + ")"
                + ",fields.size=" + objFromJsonFile.NETWORK[0].fields[0].size
                + ",...")
}

