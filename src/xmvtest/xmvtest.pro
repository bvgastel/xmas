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

include(deployment.pri)
qtcAddDeployment()


DISTFILES += \
    readme.md


# All external libraries from $$PWD/../lib, no distinction win32/unix necessary
#
# Remark: always using gtest and gtest_main contrary to gtestd and gtest_maind for debug.
#

unix|win32: LIBS += -L$$PWD/../../lib -ldatamodel -lvt -lbitpowder -lgtest


INCLUDEPATH += $$PWD/../../include/datamodel
DEPENDPATH += $$PWD/../../include/datamodel

INCLUDEPATH += $$PWD/../../include/vt
DEPENDPATH += $$PWD/../../include/vt

INCLUDEPATH += $$PWD/../../include/bitpowder
DEPENDPATH += $$PWD/../../include/bitpowder

INCLUDEPATH += $$PWD/../../include
DEPENDPATH += $$PWD/../../include

