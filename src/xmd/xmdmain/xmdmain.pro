# xmdmain.pro
# Contains the main program for xmd

CONFIG += C++11

QT += widgets svg declarative quick qml quickwidgets

TEMPLATE = app

HEADERS       = \

SOURCES += \
    main.cpp

RESOURCES +=

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



win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../build-bitpowder-Desktop-Debug/release/ -lbitpowder
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../build-bitpowder-Desktop-Debug/debug/ -lbitpowder
else:unix: LIBS += -L$$PWD/../../build-bitpowder-Desktop-Debug/ -lbitpowder

INCLUDEPATH += $$PWD/../../bitpowder
DEPENDPATH += $$PWD/../../bitpowder

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../build-xmv-Desktop-Debug/datamodel/release/ -ldatamodel
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../build-xmv-Desktop-Debug/datamodel/debug/ -ldatamodel
else:unix: LIBS += -L$$PWD/../../build-xmv-Desktop-Debug/datamodel/ -ldatamodel

INCLUDEPATH += $$PWD/../../xmv/datamodel
DEPENDPATH += $$PWD/../../xmv/datamodel
