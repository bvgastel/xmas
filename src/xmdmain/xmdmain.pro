# xmdmain.pro
# Contains the main program for xmd

CONFIG += C++11

QT += widgets svg declarative quick qml quickwidgets

TEMPLATE = app

HEADERS       = \
    controller.h

SOURCES += \
    controller.cpp \
    main.cpp

##################################################################
#LIBS += -L../xmas -L../xmd -lxmas -lxmd
#win32:CONFIG(release, debug|release): LIBS += -L../xmas/release -L../xmd/release -lxmas -lxmd
#else:win32:CONFIG(debug, debug|release): LIBS += -L../xmas/debug -L../xmd/debug -lxmas -lxmd
#else:unix: LIBS += -L../xmas -L../xmd -lxmas -lxmd
## We will build the final executable in the build directory.
#TARGET = xmd
##################################################################


win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../xmd/release/ -lxmd
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../xmd/debug/ -lxmd
else:unix: LIBS += -L$$OUT_PWD/../xmd/ -lxmd

INCLUDEPATH += $$PWD/../xmd
DEPENDPATH += $$PWD/../xmd
