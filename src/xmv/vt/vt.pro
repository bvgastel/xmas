#-------------------------------------------------
#
# Project created by QtCreator 2015-04-06T08:01:11
#
#-------------------------------------------------

QT       -= gui

TARGET = vt
TEMPLATE = lib

WARNINGS += -Wall
CONFIG += C++11
CONFIG += create_prl
CONFIG += link_prl
win32: CONFIG += static
linux: CONFIG += static dll
macx: CONFIG += staticlib

DEFINES += VT_LIBRARY

SOURCES += \
    cycle.cpp \
    deadlock.cpp \
    vt.cpp \
    feedback-interface.cpp

HEADERS += \
    cycle.h \
    deadlock.h \
    vt.h \
    vt_global.h \
    feedback-interface.h

################################################
# INSTALL instructions
################################################
unix|win32|macx {
    target.path = $$PWD/../../../lib
    INSTALLS += target
}

DISTFILES += \
    readme.md

################################################
# Dependencies
################################################
include(../../xmv/datamodel/datamodel.pri)

