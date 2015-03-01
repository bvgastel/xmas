/*********************************************************************
 *
 * Copyright (C) Guus Bonnema, 2015
 *
 * This file is part of the xmas-design tool.
 *
 * The xmas-design tool is free software: you can redistribute it
 * and/or modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 *
 * The xmas-design tool is distributed in the hope that it will be
 * useful,  but WITHOUT ANY WARRANTY; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with the xmas-design tool.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 **********************************************************************/
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
