#-------------------------------------------------
#
# Project created by QtCreator 2015-03-23T17:09:25
#
#-------------------------------------------------

TARGET = mainplugin

TEMPLATE = app
WARNINGS += -Wall

QT       += core

CONFIG += console
CONFIG -= app_bundle
CONFIG += C++11
CONFIG += link_prl

SOURCES += main.cpp

CONFIG(debug, debug|release) {
    macx: TARGET = $$join(TARGET,,,_debug)
    win32: TARGET = $$join(TARGET,,,d)
}

################################################
# INSTALL instructions
################################################
unix|win32|macx {
 target.path=$$PWD/../../../bin
 INSTALLS += target
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
    -L$$PWD/../../../lib/vt/ -lvt_debug \


else:win32:CONFIG(debug, debug|release): LIBS += \
    -L$$PWD/../../../lib/bitpowder/ -lbitpowderd \
    -L$$PWD/../../../lib/bitdatamodel/ -ldatamodeld \
    -L$$PWD/../../../lib/vt/ -lvtd \


else:unix|CONFIG(debug, debug|release): LIBS += \
    -L$$PWD/../../../lib/bitpowder/ -lbitpowder \
    -L$$PWD/../../../lib/datamodel/ -ldatamodel \
    -L$$PWD/../../../lib/vt/ -lvt \


INCLUDEPATH += $$PWD/../../../include/bitpowder
DEPENDPATH += $$PWD/../../../include/bitpowder

INCLUDEPATH += $$PWD/../../../include/datamodel
DEPENDPATH += $$PWD/../../../include/datamodel

INCLUDEPATH += $$PWD/../../../include/datamodel
DEPENDPATH += $$PWD/../../../include/datamodel


