#-------------------------------------------------
#
# Project created by QtCreator 2015-01-17T11:27:50
#
#-------------------------------------------------

QT       -= gui
QT       += qml

TARGET = model
TEMPLATE = lib

CONFIG += C++11

DEFINES += MODEL_LIBRARY

SOURCES += \
    network.cpp \
    channel.cpp \
    port.cpp \
    portlist.cpp \
    chipcomponent.cpp \
    borderpos.cpp \
    utils.cpp \
    grid.cpp \
    gridpoint.cpp

HEADERS +=\
        model_global.h \
    network.h \
    channel.h \
    port.h \
    portlist.h \
    chipcomponent.h \
    borderpos.h \
    utils.h \
    grid.h \
    gridpoint.h

unix {
    target.path = /usr/local/lib64
    INSTALLS += target
}
