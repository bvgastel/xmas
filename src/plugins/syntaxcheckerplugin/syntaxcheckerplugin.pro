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
CONFIG += dll
#CONFIG += build_all

TARGET = syntaxcheckerplugin
CONFIG(debug, debug|release) {
    macx: TARGET = $$join(TARGET,,,_debug)
    win32: TARGET = $$join(TARGET,,,d)
}

DEFINES += PLUGINS_LIBRARY

SOURCES += \
    syntaxcheckerplugin.cpp \
    syntaxcheckworker.cpp

HEADERS +=\
    syntaxcheckerplugin.h \
    syntaxcheckworker.h \
    loggerfactory.h

################################################
# INSTALL instructions
################################################
unix|win32|macx {
    target.path = $$PWD/../../../lib/plugins
    INSTALLS += target

    # Plugin should never need to export header files
}

DISTFILES += \
    vtplugin.json \
    readme_todo.md

################################################
# Internal dependencies
################################################

macx:CONFIG(debug, debug|release): LIBS += \
    -L$$OUT_PWD/../workerinterfaces/debug/ -lworkerinterfaces_debug \

else:win32:CONFIG(debug, debug|release): LIBS += \
    -L$$OUT_PWD/../workerinterfaces/debug/ -lworkerinterfacesd \


else:unix:CONFIG(debug, debug|release): LIBS += \
    -L$$OUT_PWD/../workerinterfaces/ -lworkerinterfaces \


INCLUDEPATH += $$PWD/../workerinterfaces
DEPENDPATH += $$PWD/../workerinterfaces

################################################
# External dependencies
################################################
macx:CONFIG(debug, debug|release): LIBS += \
    -L$$PWD/../../../lib/bitpowder/ -lbitpowder_debug \
    -L$$PWD/../../../lib/datamodel/ -ldatamodel_debug \
    -L$$PWD/../../../lib/vt/ -lvt_debug \
    -L$$PWD/../../../lib/interfaces/ -linterfaces_debug

else:win32:CONFIG(debug, debug|release): LIBS += \
    -L$$PWD/../../../lib/bitpowder/ -lbitpowderd \
    -L$$PWD/../../../lib/datamodel/ -ldatamodeld \
    -L$$PWD/../../../lib/vt/ -lvtd \
    -L$$PWD/../../../lib/interfaces/ -linterfacesd

else:unix|CONFIG(debug, debug|release): LIBS += \
    -L$$PWD/../../../lib/bitpowder/ -lbitpowder \
    -L$$PWD/../../../lib/datamodel/ -ldatamodel \
    -L$$PWD/../../../lib/vt/ -lvt \
    -L$$PWD/../../../lib/interfaces/ -linterfaces

INCLUDEPATH += $$PWD/../../../include/bitpowder
DEPENDPATH += $$PWD/../../../include/bitpowder

INCLUDEPATH += $$PWD/../../../include/datamodel
DEPENDPATH += $$PWD/../../../include/datamodel

INCLUDEPATH += $$PWD/../../../include/vt
DEPENDPATH += $$PWD/../../../include/vt

INCLUDEPATH += $$PWD/../../../include/interfaces
DEPENDPATH += $$PWD/../../../include/interfaces

