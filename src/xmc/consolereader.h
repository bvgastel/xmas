#ifndef CONSOLEREADER_H
#define CONSOLEREADER_H

#pragma once

#include <QObject>
#include <QSocketNotifier>

class ConsoleReader : public QObject
{
    Q_OBJECT

public:
    explicit ConsoleReader(QObject *parent = 0);
    ~ConsoleReader();
signals:
    void textReceived(QString message);
public slots:
    void text(int);
private:
    QSocketNotifier notifier;
};

#endif // CONSOLEREADER_H
