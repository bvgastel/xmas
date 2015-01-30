# Interface Qml to Xmas datamodel

## Modification history

* 29-01-2015 Guus : creation

## Problem definition

1. A requirement is to use the implementation of the xmas datamodel
as shown in `xmas.h` and `xmas.cpp`. 
2. We decided to use `QtQuick2`. 
3. A requirement is to have the console program not use Qt.

The first two points imply the need to interface the differing
models of data that `qml` and `xmas.h` use. The third point 
implies that we cannot use Qt in the data model itself, so 
we need an interface between the `qml` user interface and the
non `Qt` data model. We decided to concentrate the interface
into a class and use this class to convert `qml` data to `xmas`
data and vise versa.

### Qml - C++ interaction

There are two ways of interacting. We need to chose which one we
will use.

#### Register the class with the engine

After registering the interfacing class with the `Qml` engine, the javascript in
`Qml` is able to use methods from the registered class. An example code below:

~~~~~~~~~~~~~~~~~~~~~~~
// Using QQmlComponent
QQmlEngine engine;
QQmlComponent component(&engine,
        QUrl::fromLocalFile("MyItem.qml"));
QObject *object = component.create();
...
delete object;
~~~~~~~~~~~~~~~~~~~~~~~



> The advantages are 
> 
> 1. ease of use from the user interface. 
> 
> 2. connect to standard signals from C++ class not necessary,
	Connects are done transparantly and automatically.

  3. The need to search `qml` trees is absent.


The disadvantages are 

1. coupling between user interface and the data model because of the use
	of the data interface object in the `qml` code. (`qml` is a rewrite of 
	javascript engine `v8`).

#### Dynamic coupling from datamodel to qml only

The other approach is to not register the interfacing class with
`MetaObject::InvokeMethod`. An example follows:

~~~~~~~~~~~~~~~~~
	// main.cpp
	QQmlEngine engine;
	QQmlComponent component(&engine, "MyItem.qml");
	QObject *object = component.create();

	QVariant returnedValue;
	QVariant msg = "Hello from C++";
	QMetaObject::invokeMethod(object, "myQmlFunction",
        	Q_RETURN_ARG(QVariant, returnedValue),
	        Q_ARG(QVariant, msg));

	qDebug() << "QML function returned:" << returnedValue.toString();
	delete object;
~~~~~~~~~~~~~~~~~

>

The advantages of this approach are

1. more complete uncoupling of user interface with data model. Only the
interfacing class understands both the datamodel and the user interface.

The disadvantages of this approach are

1. More work to call slots in `Qml`.
2. The need to delve into the object tree of the visual `qml` structures.
   See the warning from the `Qt` documentation:

>
> Warning: While it is possible to use C++ to access and manipulate QML objects deep into the
> object tree, we recommend that you do not take this approach outside of application testing 
> and prototyping. One strength of QML and C++ integration is the ability to implement the QML 
> user interface separately from the C++ logic and dataset backend, and this strategy breaks if
>  the C++ side reaches deep into the QML components to manipulate them directly. This would make 
> it difficult to, for example, swap a QML view component for another view, if the new component 
> was missing a required objectName. It is better for the C++ implementation to know as little as 
> possible about the QML user interface implementation and the composition of the QML object tree.
>



## Proposed conclusion

We propose to use the approach of coupling the interface class with the user interface
through registration. This creates a signal/slot based coupling, but
prevents the interface class from reaching too deep into the user interface
qml. That way, should we need to replace the qml with a standard 2D user interface
we only have to adapt the user interface, not the data interface object. For
reference the warning from the qml docs on page 



## Motivation

Although decoupling of user interface and data model are important, decreasing
the amount of code necessary is also important. The interface class is already 
tightly coupled to `qml`, so that is not a consideration. However, complete decoupling 
would require the interface object to have intimate knowledge of the user interface.
This implies that any change in the user interface might impact the data interface class
without the impact being obvious. 

For this reason, the option where the data interface class registers seems to result
in less coupling than the alternative approach.

As the interface class is the only
registered object that communicates with the `qml` files, and every `signal` and `slot`
would be necessary with or without `qml`

