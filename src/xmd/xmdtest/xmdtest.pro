#-------------------------------------------------
#
# Project created by QtCreator 2015-02-18T10:17:35
#
#-------------------------------------------------
TEMPLATE = app

WARNINGS += -Wall

CONFIG += console
CONFIG -= app_bundle
CONFIG += C++11
CONFIG += link_prl

TARGET = xmdtest

HEADERS +=

SOURCES += \
    main.cpp \
    testnetwork.cpp

################################################
# INSTALL instructions
################################################
unix|win32|macx {
 target.path=$$PWD/../../bin
 INSTALLS += target
}

DISTFILES += \
    readme.md

################################################
# Internal dependencies
################################################
macx:CONFIG(debug, debug|release): LIBS += \
    -L$$OUT_PWD/../xmd/ -lxmd_debug
else:macx:CONFIG(release, debug|release): LIBS += \
    -L$$OUT_PWD/../xmd/ -lxmd
else:win32:CONFIG(debug, debug|release): LIBS += \
    -L$$OUT_PWD/../xmd/debug/ -lxmdd
else:win32:CONFIG(release, debug|release): LIBS += \
    -L$$OUT_PWD/../xmd/release/ -lxmd
else:unix: LIBS += \
    -L$$OUT_PWD/../xmd/ -lxmd

INCLUDEPATH += $$PWD/../xmd
DEPENDPATH += $$PWD/../xmd

################################################
# External dependencies
################################################
macx:CONFIG(debug, debug|release): LIBS += \
    -L$$PWD/../../../lib/bitpowder/ -lbitpowder_debug \
    -L$$PWD/../../../lib/datamodel/ -ldatamodel_debug \
    -L$$PWD/../../../lib/vt/ -lvt_debug \
    -L$$PWD/../../../lib/plugins/interfaces -linterfaces_debug

else:win32:CONFIG(debug, debug|release): LIBS += \
    -L$$PWD/../../../lib/bitpowder/ -lbitpowderd \
    -L$$PWD/../../../lib/datamodel/ -ldatamodeld \
    -L$$PWD/../../../lib/vt/ -lvtd \
    -L$$PWD/../../../lib/plugins/interfaces -linterfacesd

else:unix|CONFIG(release, debug|release): LIBS += \
    -L$$PWD/../../../lib/bitpowder/ -lbitpowder \
    -L$$PWD/../../../lib/datamodel/ -ldatamodel \
    -L$$PWD/../../../lib/vt/ -lvt \
    -L$$PWD/../../../lib/plugins/interfaces -linterfaces

# All external libraries from $$PWD/../lib[/<subdir>], no distinction win32/unix necessary
#
# Remark: 1. always using gtest and gtest_main contrary to gtestd and gtest_maind for debug.
#         2. always use gtest or gtest_main from a version compilated for your machine
#

unix|win32|macx: LIBS += -L$$PWD/../../../lib -lgtest

INCLUDEPATH += $$PWD/../../../include/bitpowder $$PWD/../../bitpowder
DEPENDPATH += $$PWD/../../../include/bitpowder $$PWD/../../bitpowder

INCLUDEPATH += $$PWD/../../../include/datamodel $$PWD/../../xmv/datamodel
DEPENDPATH += $$PWD/../../../include/datamodel $$PWD/../../xmv/datamodel

INCLUDEPATH += $$PWD/../../../include/plugins/interfaces  $$PWD/../../plugins/interfaces
DEPENDPATH += $$PWD/../../../include/plugins/interfaces  $$PWD/../../plugins/interfaces

INCLUDEPATH += $$PWD/../../../include/vt $$PWD/../../xmv/vt
DEPENDPATH += $$PWD/../../../include/vt $$PWD/../../xmv/vt

INCLUDEPATH += $$PWD/../../../include
DEPENDPATH += $$PWD/../../../include

