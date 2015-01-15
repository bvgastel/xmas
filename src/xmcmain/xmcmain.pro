# xmcmain.pro
# Contains the main program for xmc

CONFIG += C++11

TEMPLATE = app

include (../xmas/defines.pri)
include (../xmd/defines.pri)
include (../xmc/defines.pri)

QT       += core widgets

QT       -= gui

TARGET = xmc
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += \
	main.cpp

HEADERS += \


#unix {
#	target.path = /usr/lib
#	INSTALLS += target
#        LIBS += -L../xmas -L../xmd  -L../xmc
#}

#win32 {
#	target.path = xmc
#	INSTALLS += target
#        CONFIG(release, debug|release): LIBS += -L../xmas/release -L../xmd/release  -L../xmc/release
#        else: CONFIG(debug, debug|release): LIBS += -L../xmas/debug -L../xmas/debug -L../xmc/debug
#}

#LIBS += -lxmas -lxmd -lxmc


win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../xmd/release/ -lxmd
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../xmd/debug/ -lxmd
else:unix: LIBS += -L$$OUT_PWD/../xmd/ -lxmd

INCLUDEPATH += $$PWD/../xmd
DEPENDPATH += $$PWD/../xmd

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../xmas/release/ -lxmas
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../xmas/debug/ -lxmas
else:unix: LIBS += -L$$OUT_PWD/../xmas/ -lxmas

INCLUDEPATH += $$PWD/../xmas
DEPENDPATH += $$PWD/../xmas

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../xmc/release/ -lxmc
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../xmc/debug/ -lxmc
else:unix: LIBS += -L$$OUT_PWD/../xmc/ -lxmc

INCLUDEPATH += $$PWD/../xmc
DEPENDPATH += $$PWD/../xmc
