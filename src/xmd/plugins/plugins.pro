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
    validplugin.cpp \
    logger.cpp \
    pluginthread.cpp

HEADERS +=\
    vtplugininterface.h \
    validplugin.h \
    logger.h \
    pluginthread.h

unix|win32 {
    target.path = $$PWD/../../../lib/plugins
    INSTALLS += target

    headerfiles.path=$$PWD/../../../include/plugins
    headerfiles.files = $$PWD/*.h
    INSTALLS += headerfiles
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../xmd/release/ -lxmd
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../xmd/debug/ -lxmd
else:unix: LIBS += -L$$OUT_PWD/../xmd/ -lxmd

INCLUDEPATH += $$PWD/../xmd
DEPENDPATH += $$PWD/../xmd

unix|win32: LIBS += -L$$PWD/../../../lib/bitpowder -lbitpowder
unix|win32: LIBS += -L$$PWD/../../../lib/datamodel -ldatamodel

INCLUDEPATH += $$PWD/../../../include/bitpowder
DEPENDPATH += $$PWD/../../../include/bitpowder

INCLUDEPATH += $$PWD/../../../include/datamodel
DEPENDPATH += $$PWD/../../../include/datamodel

DISTFILES += \
    vtplugin.json

