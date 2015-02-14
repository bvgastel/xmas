FJSON file format
=================

This document (partially) describes the flat JSON file format currently used by
checker. This description is meant as a specification that states how the xMAS
design tool should serialize a network to disk. As such, not all data in
currently availabe fjson files will be explained here. Only data required by
the design tool will be listed. In order to graphically place components on a
canvas, additional data fields have been added to the specification. These
fields will not be used by the verification tools.


Root JSON object
----------------

Properties:

* "VARS"                : **VARS**
* "PACKET_TYPE"         : **PACKET_TYPE**
* "COMPOSITIE_OBJECTS"  : **COMPOSITE_OBJECTS**
* "NETWORK"             : array of **COMPONENT**


VARS
----

*TODO*


PACKET_TYPE
-----------

*TODO*

COMPOSITE_OBJECTS
-----------------

*TODO*


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
