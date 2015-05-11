#ifndef XMDEXCEPTION_H
#define XMDEXCEPTION_H

#include <stdexcept>

#include <QString>

class XmdException : std::domain_error
{
public:
    XmdException(const std::string msg);
    XmdException(const QString msg);
    ~XmdException();
};

#endif // XMDEXCEPTION_H
