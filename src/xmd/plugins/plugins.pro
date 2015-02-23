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

win32: CONFIG += static
unix: CONFIG += static dll

TARGET = plugins

DEFINES += PLUGINS_LIBRARY

SOURCES += \
    validplugin.cpp \
    logger.cpp

HEADERS +=\
    vtplugininterface.h \
    validplugin.h \
    logger.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

unix|win32: LIBS += -L$$PWD/../../../lib/bitpowder -lbitpowder
unix|win32: LIBS += -L$$PWD/../../../lib/datamodel -ldatamodel

INCLUDEPATH += $$PWD/../../../include/bitpowder
DEPENDPATH += $$PWD/../../../include/bitpowder

INCLUDEPATH += $$PWD/../../../include/datamodel
DEPENDPATH += $$PWD/../../../include/datamodel

DISTFILES += \
    vtplugin.json
