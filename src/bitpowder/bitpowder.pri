QT += qml quick

INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

LIBS += -lbitpowder

isEmpty(BASE){
    BASE = ../..
}

win32-msvc {
        CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/$$BASE/bitpowder/release/
        CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/$$BASE/bitpowder/debug/

        CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/$$BASE/bitpowder/release/bitpowder.lib
        CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/$$BASE/bitpowder/debug/bitpowder.lib
}

win32-g++ {
        CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/$$BASE/bitpowder/release/
        CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/$$BASE/bitpowder/debug/

        CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/$$BASE/bitpowder/release/libbitpowder.a
        CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/$$BASE/bitpowder/debug/libbitpowder.a
}

unix {
        LIBS += -L$$OUT_PWD/$$BASE/bitpowder/
        macx: PRE_TARGETDEPS += $$OUT_PWD/$$BASE/bitpowder/libbitpowder.a

        QMAKE_LFLAGS += -fprofile-arcs
}
