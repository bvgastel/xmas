include(../../xmv/datamodel/datamodel.pri)

QT += qml quick

INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

LIBS += -lvt

win32 {
        CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/$$BASE/xmv/vt/release/
        CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/$$BASE/xmv/vt/debug/

        CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/$$BASE/xmv/vt/release/vt.lib
        CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/$$BASE/xmv/vt/debug/vt.lib
}

unix {
        LIBS += -L$$OUT_PWD/$$BASE/xmv/vt/
        PRE_TARGETDEPS += $$OUT_PWD/$$BASE/xmv/vt/libvt.a

	QMAKE_LFLAGS += -fprofile-arcs
}
