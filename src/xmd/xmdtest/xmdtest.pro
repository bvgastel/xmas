#-------------------------------------------------
#
# Project created by QtCreator 2015-02-18T10:17:35
#
#-------------------------------------------------
TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle

TARGET = xmdtest

HEADERS +=

SOURCES += \
    main.cpp \
    testnetwork.cpp


CONFIG += C++11
CONFIG += link_prl

DISTFILES += \
    readme.md


unix|win32 {
 target.path=$$PWD/../../bin
 INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../xmd/release/ -lxmd
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../xmd/debug/ -lxmd
else:unix: LIBS += -L$$OUT_PWD/../xmd/ -lxmd

INCLUDEPATH += $$PWD/../xmd
DEPENDPATH += $$PWD/../xmd

#win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../plugins/release/ -lplugins
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../plugins/debug/ -lplugins
#else:unix: LIBS += -L$$OUT_PWD/../plugins/ -lplugins

#INCLUDEPATH += $$PWD/../plugins
#DEPENDPATH += $$PWD/../plugins

# All external libraries from $$PWD/../lib[/<subdir>], no distinction win32/unix necessary
#
# Remark: 1. always using gtest and gtest_main contrary to gtestd and gtest_maind for debug.
#         2. always use gtest or gtest_main from a version compilated for your machine
#

unix|win32: LIBS += -L$$PWD/../../../lib/datamodel -ldatamodel
unix|win32: LIBS += -L$$PWD/../../../lib/vt -lvt
unix|win32: LIBS += -L$$PWD/../../../lib/bitpowder -lbitpowder
unix|win32: LIBS += -L$$PWD/../../../lib/plugins/interfaces -linterfaces
unix|win32: LIBS += -L$$PWD/../../../lib -lgtest

INCLUDEPATH += $$PWD/../../../include/plugins/interfaces
DEPENDPATH += $$PWD/../../../include/plugins/interfaces

INCLUDEPATH += $$PWD/../../../include/datamodel
DEPENDPATH += $$PWD/../../../include/datamodel

INCLUDEPATH += $$PWD/../../../include/vt
DEPENDPATH += $$PWD/../../../include/vt

INCLUDEPATH += $$PWD/../../../include/bitpowder
DEPENDPATH += $$PWD/../../../include/bitpowder

INCLUDEPATH += $$PWD/../../../include
DEPENDPATH += $$PWD/../../../include

