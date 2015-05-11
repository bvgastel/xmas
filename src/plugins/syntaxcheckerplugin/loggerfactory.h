#ifndef LOGGERFACTORY_H
#define LOGGERFACTORY_H

#include <QString>

#include "loggerinterface.h"

class LoggerFactory
{

public:
    LoggerFactory() {}
    virtual ~LoggerFactory() {}

    static LoggerInterface *MakeLogger(QString name);

};

#endif // LOGGERFACTORY_H
