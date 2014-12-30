#-------------------------------------------------
#
# Project created by QtCreator 2014-12-27T16:51:57
#
#-------------------------------------------------

QT       -= gui

TARGET = xmas
TEMPLATE = lib

TEMPLATE=subdirs

CONFIG += C++11

DEFINES += XMAS_LIBRARY

SOURCES += component.cpp \
    noc.cpp \
    channel.cpp

HEADERS += component.h\
        xmas_global.h \
    noc.h \
    channel.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

SUBDIRS += \
    TestXmasLib
