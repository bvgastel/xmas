TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

HEADERS +=

SOURCES += \
    main.cpp \
    testcyclechecker.cpp \
    testdatamodel.cpp


CONFIG += C++11
CONFIG += link_prl

include(deployment.pri)
qtcAddDeployment()


DISTFILES += \
    readme.md


unix|win32 {
 target.path=$$PWD/../../bin
 INSTALLS += target
}


# All external libraries from $$PWD/../lib[/<subdir>], no distinction win32/unix necessary
#
# Remark: 1. always using gtest and gtest_main contrary to gtestd and gtest_maind for debug.
#         2. always use gtest or gtest_main from a version compilated for your machine
#

unix|win32: LIBS += -L$$PWD/../../lib/datamodel -ldatamodel
unix|win32: LIBS += -L$$PWD/../../lib/vt -lvt
unix|win32: LIBS += -L$$PWD/../../lib/bitpowder -lbitpowder
unix|win32: LIBS += -L$$PWD/../../lib -lgtest


INCLUDEPATH += $$PWD/../../include/datamodel
DEPENDPATH += $$PWD/../../include/datamodel

INCLUDEPATH += $$PWD/../../include/vt
DEPENDPATH += $$PWD/../../include/vt

INCLUDEPATH += $$PWD/../../include/bitpowder
DEPENDPATH += $$PWD/../../include/bitpowder

INCLUDEPATH += $$PWD/../../include
DEPENDPATH += $$PWD/../../include
