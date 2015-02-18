#include <QString>

#include "xmdexception.h"

XmdException::XmdException(std::string msg) : std::domain_error(msg)
{

}

XmdException::XmdException(QString msg) : std::domain_error(msg.toStdString())
{

}

XmdException::~XmdException()
{

}

