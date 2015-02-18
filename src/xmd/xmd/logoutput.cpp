#include "logoutput.h"
#include "controller.h"

LogOutput::LogOutput(QObject *parent) : QObject(parent)
{
    Controller *controller = qobject_cast<Controller *>(parent);
    if (controller) {
        QObject::connect(this, SIGNAL(writeLog(QString msg, QColor color)),
                         controller, SIGNAL(writeLog(QString message,QColor color)));
    }
}

LogOutput::~LogOutput()
{

}

void LogOutput::write(const QString msg, const QColor color) {
    emit writeLog(msg, color);
}

void LogOutput::write(const QString prefix, const QString msg, const QColor color) {
    emit writeLog(prefix+msg, color);
}

