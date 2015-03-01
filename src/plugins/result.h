#ifndef RESULT_H
#define RESULT_H

#include <QObject>

struct ErrorObject {
    bool error;
    QString errorMessage;
    QString errorObjectName;
};

class Result : public QObject
{
    Q_OBJECT
    friend class SyntaxCheckWorker;

public:
    Result(QObject *parent = 0);
    ~Result();

    const QList<ErrorObject> &errorList() const {
        return m_errorList;
    }

    const QString description() const {
        return m_description;
    }

private:
    void addErrorList(ErrorObject errorObject) {
        m_errorList.append(errorObject);
    }

    void add2ResultString(QString partialResult) {
        m_description += partialResult;
    }

    QList<ErrorObject> m_errorList;
    QString m_description;
};

#endif // RESULT_H
