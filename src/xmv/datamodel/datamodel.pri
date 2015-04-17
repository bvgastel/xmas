include(../../bitpowder/bitpowder.pri)

QT += qml quick

INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

LIBS += -ldatamodel

win32 {
        CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/$$BASE/xmv/datamodel/release/
        CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/$$BASE/xmv/datamodel/debug/

        CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/$$BASE/xmv/datamodel/release/datamodel.lib
        CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/$$BASE/xmv/datamodel/debug/datamodel.lib
}

unix {
        LIBS += -L$$OUT_PWD/$$BASE/xmv/datamodel/
        macx: PRE_TARGETDEPS += $$OUT_PWD/$$BASE/xmv/datamodel/libdatamodel.a

	QMAKE_LFLAGS += -fprofile-arcs
}
