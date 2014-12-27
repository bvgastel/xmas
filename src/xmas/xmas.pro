#-------------------------------------------------
#
# Project created by QtCreator 2014-12-27T16:51:57
#
#-------------------------------------------------

QT       -= gui

TARGET = xmas
TEMPLATE = lib

CONFIG += C++11

DEFINES += XMAS_LIBRARY

SOURCES += component.cpp \
    port.cpp \
    noc.cpp \
    channel.cpp \
    inport.cpp \
    outport.cpp

HEADERS += component.h\
        xmas_global.h \
    port.h \
    noc.h \
    channel.h \
    inport.h \
    outport.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
