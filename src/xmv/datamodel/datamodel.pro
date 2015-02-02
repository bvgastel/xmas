#-------------------------------------------------
#
# Project created by QtCreator 2015-01-26T14:57:19
#
#-------------------------------------------------

QT      -= qt
QT      -= core gui

TARGET = datamodel
TEMPLATE = lib

CONFIG += C++11

DEFINES += DATAMODEL_LIBRARY

SOURCES += datamodel.cpp \
    xmas.cpp

HEADERS += datamodel.h\
        datamodel_global.h \
    xmas.h

# doesn't appear to work: should distribute header files with debug or release
#    header_files.files = $$HEADERS
#    header_files.path = $$PWD
#    QMAKE_EXTRA_TARGETS += header_files


win32 {
    # target.path = don't know
    INSTALLS += target
}

unix {
    target.path = /usr/lib
    INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../bitpowder/release/ -lbitpowder
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../bitpowder/debug/ -lbitpowder
else:unix: LIBS += -L$$OUT_PWD/../bitpowder/ -lbitpowder

INCLUDEPATH += $$PWD/../bitpowder
DEPENDPATH += $$PWD/../bitpowder
