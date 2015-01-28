! include( ../common.pri ) {
        error( Could not find the common.pri file)
}

include (defines.pri)

QT += widgets svg quick qml quickwidgets

HEADERS       = \ 
				
SOURCES       = \ 
    
unix {
    target.path = /usr/lib
    INSTALLS += target
}

win32 {
    target.path = xmd
    INSTALLS += target
}

INCLUDEPATH += content qml

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
	mainWindow.qml \
    content/itemCreation.js \
    content/ScrollBar.qml \
    content/MenuBar.qml \
    content/XmasToolBar.qml \
    content/ToolBarItem.qml \
    Sheet.qml \
    qml/XComponent.qml \
    qml/XConnector.qml \
    qml/XConnection.qml \
    content/connectionCreation.js \
    qml/Line.qml

RESOURCES += \
    xmd.qrc

FORMS +=
