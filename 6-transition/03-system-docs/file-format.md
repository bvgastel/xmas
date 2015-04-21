XMAS network file format
========================

This document describes the file format used to store XMAS network models. Both
the XMAS designer application and the standalone verification tools use this
file format. The structure is based on the flat json format used by the checker
application provided at the start of this project. Several new fields have been
added in order to support hierarchical networks and information specific to the
designer. As the new file format is able to support both flat and hierachical
networks, the (recommended) file extension has been changed from fjson to json.



Root JSON object
----------------

Properties:

* "CANVAS"              : **CANVAS** (optional) *NEW*
* "COMPOSITE_NETWORK"   : **COMPOSITE_NETWORK** (optional) *NEW*
* "VARS"                : **VARS**
* "PACKET_TYPE"         : **PACKET_TYPE**
* "NETWORK"             : array of **COMPONENT**



CANVAS
------

***NEW***
Used by the XMAS designer application to define properties of the designer canvas.
When no CANVAS information is specified, default values are used.

Properties:

* "width"       : width of the canvas in logical units
* "height"      : height of the canvas in logical units


COMPOSITE_NETWORK
-----------------

***NEW***
Contains designer specific information to use this network as a composite object.
If this information is not present, the network cannot be used as a composite object.

Properties:

* "alias"       : displayed inside a composite object to denote the objects type (e.g. mesh)
* "image-name"  : name of graphical resource used as a symbol to denote the objects type (e.g. mesh.ico)
* "boxed-image" : set to 1 to use the image as a symbol inside a generic composite object (drawn boxed)
                  or set to 0 to use the image as the graphical representation of the entire component,
                  this can be used to draw common macro's like credit counters and delays using the symbols
                  commonly used in literature to denote these macro's.
* "packet"      : ???


VARS
----

*TODO*


PACKET_TYPE
-----------

*TODO*

COMPOSITE_OBJECTS
-----------------

***NEW***

Description:
Array of references to subnetworks used as composite objects by this network.

A reference indicates the relative location of the subnetwork on the filesystem excluding the extension.
E.g. "mesh" refers to the network defined in "mesh.xmas" in the same directory as this network and
"spidergon/node" refers to the network defined in "node.xmas" in the subdirectoy "spidergon".



COMPONENT
---------

Description:
Describes a component in the network.

Properties:

* "id"          : **String** - unique identifier
* "type"        : **Component_type** - type of the component
* "outs"        : array of **OUT**
* "fields"      : array of **FIELD** *(optional)*
* "pos"         : **POSITION** - *new* - position of the component on the canvas


Component_type
--------------

Enumeration:

* source
* sink
* function
* queue
* xfork
* join
* xswitch
* merge


COMPONENT [type="composite"]
----------------------------

Properties:

* "subnetwork"  : **String** - name of the subnetwork

See also field **COMPOSITE_OBJECTS**

OUT
---

Description:
Describes channels between the components in the network. For each output port of a component, an **OUT** object describes to which input port of which component it is connected.

Properties:

* "id"          : **String** - reference to the target component
* "in_port"     : **Number** - index of the input port on the target component

FIELD
-----

*TODO*


POSITION
--------

***New object type introduced by xMAS designer***

Properties:

* "x"           : **Number** - x or horizontal position on the canvas
* "y"           : **Number** - y or vertical position on the canvas
* "orientation" : **Number** - Orientation of the component, measured in degrees (counter?) clockwise.
* "scale"       : **Number** - scale factor of the component
