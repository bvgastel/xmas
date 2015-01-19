#-------------------------------------------------
#
# Project created by QtCreator 2014-12-30T16:16:10
#
#-------------------------------------------------

! include( ../common.pri ) {
	error( Could not find the common.pri file )
}


QT      += core
QT      += testlib
QT      += qml

# CONFIG   += testcase
CONFIG   += console
CONFIG   += C++11

QT       -= gui
CONFIG   -= app_bundle

SOURCES += \
    testrunner.cpp \
    testport.cpp \
    testchipcomponent.cpp

HEADERS += \
    testrunner.h \
    testport.h \
    testchipcomponent.h

RESOURCES += \
    testport.qrc

# By default TARGET is the same as the directory,
# so it will make test (in linux). Uncomment to override.
# TARGET = target

unix {
	target.path = /usr/lib
	INSTALLS += target
}

win32 {
    target.path = test
    INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../model/release/ -lmodel
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../model/debug/ -lmodel
else:unix: LIBS += -L$$OUT_PWD/../model/ -lmodel

INCLUDEPATH += $$PWD/../model
DEPENDPATH += $$PWD/../model

