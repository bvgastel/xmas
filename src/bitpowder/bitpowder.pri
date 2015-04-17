QT += qml quick

INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

LIBS += -lbitpowder

isEmpty(BASE){
    BASE = ../..
}

win32 {
        CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/$$BASE/bitpowder/release/
        CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/$$BASE/bitpowder/debug/

        CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/$$BASE/bitpowder/release/bitpowder.lib
        CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/$$BASE/bitpowder/debug/bitpowder.lib
}

unix {
        LIBS += -L$$OUT_PWD/$$BASE/bitpowder/
        macx: PRE_TARGETDEPS += $$OUT_PWD/$$BASE/bitpowder/libbitpowder.a

        QMAKE_LFLAGS += -fprofile-arcs
}
