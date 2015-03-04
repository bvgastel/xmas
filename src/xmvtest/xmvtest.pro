TEMPLATE = app

WARNINGS += -Wall

CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

HEADERS +=

SOURCES += \
    main.cpp \
    testcyclechecker.cpp \
    testdatamodel.cpp \
    jsonprinter.test.cpp


CONFIG += C++11
CONFIG += link_prl

CONFIG(debug, debug|release) {
    macx: TARGET = $$join(TARGET,,,_debug)
    win32: TARGET = $$join(TARGET,,,d)
}

DISTFILES += \
    readme.md

################################################
# INSTALL instructions
################################################
unix|win32 {
 target.path=$$PWD/../../bin
 INSTALLS += target
}

################################################
# Internal dependencies
################################################

################################################
# External dependencies
################################################
macx:CONFIG(debug, debug|release): LIBS += \
    -L$$PWD/../../lib/bitpowder/ -lbitpowder_debug \
    -L$$PWD/../../lib/vt/ -lvt_debug \
    -L$$PWD/../../lib/datamodel/ -ldatamodel_debug \
    -L$$PWD/../../lib/ -lgtest \

else:win32:CONFIG(debug, debug|release): LIBS += \
    -L$$PWD/../../lib/bitpowder/ -lbitpowderd \
    -L$$PWD/../../lib/vt/ -lvtd \
    -L$$PWD/../../lib/datamodel/ -ldatamodeld \
    -L$$PWD/../../lib/ -lgtest \

else:unix|CONFIG(release, debug|release): LIBS += \
    -L$$PWD/../../lib/bitpowder/ -lbitpowder \
    -L$$PWD/../../lib/vt/ -lvt \
    -L$$PWD/../../lib/datamodel/ -ldatamodel \
    -L$$PWD/../../lib/ -lgtest \

INCLUDEPATH += $$PWD/../../include/bitpowder
DEPENDPATH += $$PWD/../../include/bitpowder

INCLUDEPATH += $$PWD/../../include/vt
DEPENDPATH += $$PWD/../../include/vt

INCLUDEPATH += $$PWD/../../include/datamodel
DEPENDPATH += $$PWD/../../include/datamodel

INCLUDEPATH += $$PWD/../../include
DEPENDPATH += $$PWD/../../include
