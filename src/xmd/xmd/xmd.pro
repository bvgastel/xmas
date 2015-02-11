! include( ../common.pri ) {
        error( Could not find the common.pri file)
}

include (defines.pri)

QT += widgets svg quick qml quickwidgets

HEADERS       = \ 
    controller.h
				
SOURCES       = \ 
    controller.cpp
    
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
    content/ScrollBar.qml \
    content/MenuBar.qml \
    content/XmasToolBar.qml \
    content/ToolBarItem.qml \
    Sheet.qml \
    qml/XComponent.qml \
    qml/Line.qml \
	controller.js \
    qml/XChannel.qml \
    qml/XPort.qml \
    content/channelCreation.js \
    content/componentCreation.js \
    content/Selection.qml \
    content/XDialog.qml \
    content/XDialogItem.qml \
    content/OutputLog.qml \
    content/XTag.qml

RESOURCES += \
    xmd.qrc

FORMS +=

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../build-bitpowder-Desktop_Qt_5_4_0_MinGW_32bit-Debug/release/ -lbitpowder
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../build-bitpowder-Desktop_Qt_5_4_0_MinGW_32bit-Debug/debug/ -lbitpowder
else:unix: LIBS += -L$$PWD/../../build-bitpowder-Desktop_Qt_5_4_0_MinGW_32bit-Debug/ -lbitpowder

INCLUDEPATH += $$PWD/../../bitpowder
DEPENDPATH += $$PWD/../../bitpowder
