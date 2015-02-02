TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

HEADERS +=


SOURCES += \
    testxcomponentflat.cpp


CONFIG += C++11

include(deployment.pri)
qtcAddDeployment()

# Google test library
unix|win32: LIBS += -lgtest_main -lgtest

# xmv/datamodel debug library

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../build-xmv-Desktop-Debug/datamodel/release/ -ldatamodel
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../build-xmv-Desktop-Debug/datamodel/debug/ -ldatamodel
else:unix: LIBS += -L$$PWD/../build-xmv-Desktop-Debug/datamodel/ -ldatamodel

INCLUDEPATH += $$PWD/../xmv/datamodel
DEPENDPATH += $$PWD/../xmv/datamodel


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../build-xmv-Desktop-Debug/bitpowder/release/ -lbitpowder
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../build-xmv-Desktop-Debug/bitpowder/debug/ -lbitpowder
else:unix: LIBS += -L$$PWD/../build-xmv-Desktop-Debug/bitpowder/ -lbitpowder

INCLUDEPATH += $$PWD/../xmv/bitpowder
DEPENDPATH += $$PWD/../xmv/bitpowder

