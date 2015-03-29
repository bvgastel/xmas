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

    virtual const QList<std::shared_ptr<ResultObject>> &errorList() const override;
    virtual void addError(bool error, QString stepName, QString errorMessage, QString errorObjectName) override;
    virtual void addStep(QString stepName, QString stepMessage) override;

private:
    QList<std::shared_ptr<ResultObject>> m_errorList;
    QString m_description;
    bool m_error;
};

#endif // RESULT_H
