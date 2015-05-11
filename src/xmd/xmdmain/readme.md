xmdmain
=======

The main program of XMD is a Qt program that starts a QtQuick2 GUI
plus a Controller.

The controller connects the GUI with the verification tools through plugins.

The main program registers the controllers as a contect property so the Qml
can access the program from the javascript code in Qml.
