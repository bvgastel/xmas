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
    chipcomponent.cpp \
    borderpos.cpp \
    utils.cpp \
    gridpoint.cpp \
    board.cpp

HEADERS +=\
        model_global.h \
    network.h \
    channel.h \
    port.h \
    chipcomponent.h \
    borderpos.h \
    utils.h \
    gridpoint.h \
    board.h

unix {
    target.path = /usr/local/lib64
    INSTALLS += target
}
