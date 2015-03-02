TEMPLATE = lib

WARNINGS += -Wall

QT       += widgets
QT       -= gui

CONFIG += C++11
CONFIG += create_prl

CONFIG += static

TARGET = interfaces

SOURCES += \

HEADERS +=\
    result.h \

unix|win32 {
    target.path = $$PWD/../../lib/plugins
    INSTALLS += target

    headerfiles.path=$$PWD/../../include/plugins/interfaces
    headerfiles.files = $$PWD/*.h
    INSTALLS += headerfiles
}
