#ifndef LOGOUTPUT_H
#define LOGOUTPUT_H

#include <QObject>
#include <QColor>

class LogOutput : public QObject
{
    Q_OBJECT

public:
    explicit LogOutput(QObject *parent = 0);
    ~LogOutput();

signals:

    void writeLog(QString msg, QColor color);

public slots:
    void write(QString msg, QColor color = QColor("black"));
    void write(QString prefix, QString msg, QColor color = QColor("black"));

};

#endif // LOGOUTPUT_H
