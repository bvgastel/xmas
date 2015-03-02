#ifndef RESULT_H
#define RESULT_H

#include <QObject>

struct ErrorObject {
    bool error;
    QString errorMessage;
    QString errorObjectName;
};


/**
 * @brief The Result class
 *
 * This class is sufficient for simple error messages and indication of
 * an error object. For more complex needs, one can subclass this class
 * and add features as necessary.
 *
 */
class Result : public QObject
{
    Q_OBJECT

public:
    Result(QObject *parent = 0) : QObject(parent) {

    }
    virtual ~Result() {

    }

    virtual const QList<ErrorObject> &errorList() const {
        return m_errorList;
    }

    virtual const QString description() const {
        return m_description;
    }

    virtual void addErrorList(ErrorObject errorObject) {
        m_errorList.append(errorObject);
    }

    virtual void add2ResultString(QString partialResult) {
        m_description += partialResult;
    }

private:
    QList<ErrorObject> m_errorList;
    QString m_description;
};

#endif // RESULT_H
