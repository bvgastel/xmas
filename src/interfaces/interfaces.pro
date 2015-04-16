#-------------------------------------------------
#
# Project created by QtCreator 2015-04-06T07:41:19
#
#-------------------------------------------------

QT       += widgets

QT       -= gui

TARGET = interfaces
TEMPLATE = lib

WARNINGS += -Wall
CONFIG += C++11
CONFIG += create_prl
CONFIG += link_prl
win32|linux: CONFIG += dll
macx: CONFIG += staticlib

DEFINES += INTERFACES_LIBRARY

SOURCES += \
    logger.cpp \
    result.cpp \
    workerinterface.cpp

HEADERS += \
    commoninterface.h \
    logger.h \
    loggerinterface.h \
    result.h \
    resultinterface.h \
    vtplugininterface.h \
    workerinterface.h

DISTFILES += \
    readme.md

################################################
# INSTALL instructions
################################################
unix|win32|macx {
    target.path = $$PWD/../../lib
    INSTALLS += target
}

################################################
# Dependencies
################################################
BASE=..
include(../xmv/datamodel/datamodel.pri)



