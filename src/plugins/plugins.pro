#-------------------------------------------------
#
# Project created by QtCreator 2015-02-22T19:47:15
#
#-------------------------------------------------

TEMPLATE = lib

WARNINGS += -Wall

QT       += widgets
QT       -= gui

CONFIG += plugin
CONFIG += C++11
CONFIG += create_prl

win32: CONFIG += static
unix: CONFIG += static dll

TARGET = plugins

DEFINES += PLUGINS_LIBRARY

SOURCES += \
    pluginthread.cpp

HEADERS +=\
    pluginthread.h

unix|win32 {
    target.path = $$PWD/../../lib/plugins
    INSTALLS += target

    headerfiles.path=$$PWD/../../include/plugins
    headerfiles.files = $$PWD/*.h
    INSTALLS += headerfiles
}

unix|win32: LIBS += -L$$PWD/../../lib/bitpowder -lbitpowder
unix|win32: LIBS += -L$$PWD/../../lib/datamodel -ldatamodel
unix|win32: LIBS += -L$$PWD/../../lib/xmd -lxmd

INCLUDEPATH += $$PWD/../../include/bitpowder
DEPENDPATH += $$PWD/../../include/bitpowder

INCLUDEPATH += $$PWD/../../include/datamodel
DEPENDPATH += $$PWD/../../include/datamodel

INCLUDEPATH += $$PWD/../../include/xmd
DEPENDPATH += $$PWD/../../include/xmd

DISTFILES += \
    vtplugin.json

