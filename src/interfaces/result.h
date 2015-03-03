#ifndef RESULT_H
#define RESULT_H

#include <QList>
#include <QObject>

#include "resultinterface.h"

/**
 * @brief The Result class
 *
 * This class is sufficient for simple error messages and indication of
 * an error object. For more complex needs, one can subclass this class
 * and add features as necessary.
 *
 */
class Result : public ResultInterface
{

public:
    Result(QObject *parent = 0);
    virtual ~Result();

    virtual const QList<ErrorObject> &errorList() const;
    virtual const QString description() const;
    virtual void addErrorList(ErrorObject errorObject);
    virtual void add2ResultString(QString partialResult);

private:
    QList<ErrorObject> m_errorList;
    QString m_description;
};

#endif // RESULT_H
