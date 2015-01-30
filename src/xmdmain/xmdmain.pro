# xmdmain.pro
# Contains the main program for xmd

CONFIG += C++11

QT += widgets svg declarative quick qml quickwidgets

TEMPLATE = app

HEADERS       = \

SOURCES += \
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

RESOURCES +=

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../datamodel/release/ -ldatamodel
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../datamodel/debug/ -ldatamodel
else:unix: LIBS += -L$$OUT_PWD/../datamodel/ -ldatamodel

INCLUDEPATH += $$PWD/../datamodel
DEPENDPATH += $$PWD/../datamodel

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../bitpowder/release/ -lbitpowder
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../bitpowder/debug/ -lbitpowder
else:unix: LIBS += -L$$OUT_PWD/../bitpowder/ -lbitpowder

INCLUDEPATH += $$PWD/../bitpowder
DEPENDPATH += $$PWD/../bitpowder
