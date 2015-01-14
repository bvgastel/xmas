#include <QTextStream>

#include "consolereader.h"

#include <unistd.h> //Provides STDIN_FILENO

ConsoleReader::ConsoleReader(QObject *parent) :
    QObject(parent),
    notifier(STDIN_FILENO, QSocketNotifier::Read)
{
    connect(&notifier, SIGNAL(activated(int)), this, SLOT(text(int)));
}

ConsoleReader::~ConsoleReader()
{

}

void ConsoleReader::text(int)
{
    QTextStream qin(stdin);
    QString line = qin.readLine();
    emit textReceived(line);
}

