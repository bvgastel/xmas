TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

HEADERS +=


SOURCES += \
    main.cpp \
	parser.test.cpp \
	parser_json.test.cpp \
	simplestring.test.cpp \
	stringparse.test.cpp 


CONFIG += C++11

include(deployment.pri)
qtcAddDeployment()

# Google test library
unix|win32: LIBS += -lgtest_main -lgtest

# Remark: bitpowder is external, so use $$PWD, not $$OUT_PWD.

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../build-bitpowder-Desktop-Debug/release/ -lbitpowder
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../build-bitpowder-Desktop-Debug/debug/ -lbitpowder
else:unix: LIBS += -L$$PWD/../build-bitpowder-Desktop-Debug/ -lbitpowder

INCLUDEPATH += $$PWD/../bitpowder
DEPENDPATH += $$PWD/../bitpowder
