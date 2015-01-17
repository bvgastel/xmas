! include( ../common.pri ) {
        error( Could not find the common.pri file)
}

include (defines.pri)

QT += widgets svg quick qml quickwidgets

HEADERS       = \
    canvas/component.h \
    canvas/connection.h \
    canvas/connector.h \
    canvas/network.h
				
SOURCES       = \
    canvas/component.cpp \
    canvas/connection.cpp \
    canvas/connector.cpp \
    canvas/network.cpp \
    main.cpp
    
unix {
    target.path = /usr/lib
    INSTALLS += target
}

win32 {
    target.path = xmd
    INSTALLS += target
}

INCLUDEPATH += canvas content qml

TEMPLATE = lib

DISTFILES += qml/fork.qml \
    qml/function.qml \
    qml/in.qml \
    qml/join.qml \
    qml/merge.qml \
    qml/out.qml \
    qml/queue.qml \
    qml/sink.qml \
    qml/source.qml \
    qml/spidergon.qml \
    qml/switch.qml \
    qml/Connection.qml \
    qml/Connector.qml \
    qml/QComponent.qml \
	mainWindow.qml \
    content/itemCreation.js \
    content/PaletteItem.qml \
    content/ScrollBar.qml \
    content/Button.qml \
    content/GenericSceneItem.qml \
    content/ToolBar.qml \
    content/MenuBar.qml \
    canvas.qml

RESOURCES += \
    xmd.qrc

FORMS +=
