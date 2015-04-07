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
    jsonprinter.test.cpp \
    testsourcespec.cpp \
    testfunctionspec.cpp \
    testswitchfunctionspec.cpp \
    testjoinfunctionspec.cpp \
    flatten.test.cpp \
    testexport.cpp


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
# Dependencies
################################################
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../bitpowder/release/ -lbitpowder
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../bitpowder/debug/ -lbitpowder
else:unix: LIBS += -L$$OUT_PWD/../bitpowder/ -lbitpowder

INCLUDEPATH += $$PWD/../bitpowder
DEPENDPATH += $$PWD/../bitpowder

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../xmv/vt/release/ -lvt
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../xmv/vt/debug/ -lvt
else:unix: LIBS += -L$$OUT_PWD/../xmv/vt/ -lvt

INCLUDEPATH += $$PWD/../xmv/vt
DEPENDPATH += $$PWD/../xmv/vt

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../xmv/datamodel/release/ -ldatamodel
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../xmv/datamodel/debug/ -ldatamodel
else:unix: LIBS += -L$$OUT_PWD/../xmv/datamodel/ -ldatamodel

INCLUDEPATH += $$PWD/../xmv/datamodel
DEPENDPATH += $$PWD/../xmv/datamodel

unix|win32|macx: LIBS += -L$$PWD/../../lib -lgtest

INCLUDEPATH += $$PWD/../../include
DEPENDPATH += $$PWD/../../include

