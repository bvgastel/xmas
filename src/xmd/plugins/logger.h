#ifndef LOGGER_H
#define LOGGER_H

#include <QObject>
#include <QColor>

#include "simplestring.h"

class Logger : public QObject
{
    Q_OBJECT

signals:
    void writeLog(QString message,QColor color=Qt::black);

public:
    Logger(QString name): m_name(name) {

    }

    ~Logger();

    virtual void log(const std::string message, const QColor color=QColor("black"));
    virtual void log(const QString message, const QColor color=QColor("black"));
    virtual void log(const bitpowder::lib::String message, const QColor color=QColor("black"));

private:
    QString m_name;
};

#endif // LOGGER_H
