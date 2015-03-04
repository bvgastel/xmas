#ifndef LOGGERFACTORY_H
#define LOGGERFACTORY_H

#include <QString>

#include "loggerinterface.h"
#include "logger.h"

class LoggerFactory
{

public:
    LoggerFactory() {}
    virtual ~LoggerFactory() {}

    static LoggerInterface *MakeLogger(QString name) {
        return new Logger(name, nullptr);
    }

};

#endif // LOGGERFACTORY_H
