#-------------------------------------------------
#
# Project created by QtCreator 2015-01-17T11:27:50
#
#-------------------------------------------------

QT       -= gui
QT       += qml

TARGET = model
TEMPLATE = lib

DEFINES += MODEL_LIBRARY

SOURCES += \
        component.cpp \
    network.cpp \
    channel.cpp \
    port.cpp

HEADERS +=\
        model_global.h \
        component.h \
    network.h \
    channel.h \
    port.h

unix {
    target.path = /usr/local/lib64
    INSTALLS += target
}
