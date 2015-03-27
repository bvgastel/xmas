#-------------------------------------------------
#
# Project created by QtCreator 2015-03-23T16:46:55
#
#-------------------------------------------------

QT       -= gui

TARGET = workerinterfaces
TEMPLATE = lib

WARNINGS += -Wall

CONFIG += C++11
CONFIG += dll
##CONFIG += create_prl

CONFIG(debug, debug|release) {
    macx: TARGET = $$join(TARGET,,,_debug)
    win32: TARGET = $$join(TARGET,,,d)
}


DEFINES += WORKERINTERFACES_LIBRARY

SOURCES += workerinterface.cpp

HEADERS += workerinterface.h

################################################
# INSTALL instructions
################################################
unix {
    target.path = $$PWD/../../../lib/plugins
    INSTALLS += target

    headerfiles.path=$$PWD/../../../include/plugins
    headerfiles.files = $$HEADERS
    INSTALLS += headerfiles
}

################################################
# Internal dependencies
################################################

################################################
# External dependencies
################################################
macx:CONFIG(debug, debug|release): LIBS += \
    -L$$PWD/../../../lib/bitpowder/ -lbitpowder_debug \
    -L$$PWD/../../../lib/datamodel/ -ldatamodel_debug \
    -L$$PWD/../../../lib/interfaces/ -linterfaces_debug \


else:win32:CONFIG(debug, debug|release): LIBS += \
    -L$$PWD/../../../lib/bitpowder/ -lbitpowderd \
    -L$$PWD/../../../lib/datamodel/ -ldatamodeld \
    -L$$PWD/../../../lib/interfaces/ -linterfacesd \


else:unix|CONFIG(debug, debug|release): LIBS += \
    -L$$PWD/../../../lib/bitpowder/ -lbitpowder \
    -L$$PWD/../../../lib/datamodel/ -ldatamodel \
    -L$$PWD/../../../lib/interfaces/ -linterfaces \


INCLUDEPATH += $$PWD/../../../include/bitpowder
DEPENDPATH += $$PWD/../../../include/bitpowder

INCLUDEPATH += $$PWD/../../../include/datamodel
DEPENDPATH += $$PWD/../../../include/datamodel

INCLUDEPATH += $$PWD/../../../include/interfaces
DEPENDPATH +=  $$PWD/../../../include/interfaces

