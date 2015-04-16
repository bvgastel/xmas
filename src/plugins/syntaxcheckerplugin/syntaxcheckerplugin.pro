#-------------------------------------------------
#
# Project created by QtCreator 2015-04-06T08:47:13
#
#-------------------------------------------------

QT       -= gui
QT       += widgets

CONFIG += plugin
CONFIG += dll
CONFIG += create_prl
CONFIG += C++11

TARGET = syntaxcheckerplugin
TEMPLATE = lib

WARNINGS += -Wall

DEFINES += PLUGINS_LIBRARY

SOURCES += \
    syntaxcheckerplugin.cpp \
    syntaxcheckworker.cpp \
    loggerfactory.cpp

HEADERS += \
    syntaxcheckerplugin.h \
    syntaxcheckworker.h \
    loggerfactory.h

################################################
# INSTALL instructions
################################################
unix|win32|macx {
    target.path = $$PWD/../../../lib/plugins
    INSTALLS += target
}

################################################
# Dependencies
################################################
include(../../interfaces/interfaces.pri)
