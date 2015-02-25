#include <QString>
#include <QColor>

#include "logger.h"
#include "simplestring.h"

Logger::~Logger()
{

}

void Logger::log(const std::string message, const QColor color) {
    log(QString::fromUtf8(message.c_str()), color);
}

void Logger::log(const QString message, const QColor color) {
    emit writeLog(m_prefix + message ,color);
}

void Logger::log(const bitpowder::lib::String message, const QColor color) {
    log(QString::fromUtf8(message.stl().c_str()), color);
}
