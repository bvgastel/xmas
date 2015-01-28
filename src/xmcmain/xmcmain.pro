# xmcmain.pro
# Contains the main program for xmc

CONFIG += C++11

TEMPLATE = app

QT       += core widgets

QT       -= gui

TARGET = xmc
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += \
	main.cpp

HEADERS += \


win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../xmd/release/ -lxmd
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../xmd/debug/ -lxmd
else:unix: LIBS += -L$$OUT_PWD/../xmd/ -lxmd

INCLUDEPATH += $$PWD/../xmd
DEPENDPATH += $$PWD/../xmd

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../xmc/release/ -lxmc
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../xmc/debug/ -lxmc
else:unix: LIBS += -L$$OUT_PWD/../xmc/ -lxmc

INCLUDEPATH += $$PWD/../xmc
DEPENDPATH += $$PWD/../xmc
