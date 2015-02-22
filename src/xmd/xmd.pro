! include( ../common.pri ) {
        error( Could not find the common.pri file)
}

include (defines.pri)

QT += widgets svg declarative

HEADERS       = mainwindow.h \
                projectmanager.h \
                setup.h \
    canvas/complib.h \
    canvas/component.h \
    canvas/connection.h \
    canvas/connector.h \
    canvas/modelwindow.h \
    canvas/network.h
				
SOURCES       = mainwindow.cpp \
                projectmanager.cpp \
    canvas/complib.cpp \
    canvas/component.cpp \
    canvas/connection.cpp \
    canvas/connector.cpp \
    canvas/modelwindow.cpp \
    canvas/network.cpp \
    main.cpp
    main.cpp \

RESOURCES     = xmd.qrc

unix {
    target.path = /usr/lib
    INSTALLS += target
}

win32 {
    target.path = xmd
    INSTALLS += target
}

INCLUDEPATH += canvas

TEMPLATE = lib

DISTFILES += XMAS_LIBRARY \
    qml/fork.qml \
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
    qml/QComponent.qml
