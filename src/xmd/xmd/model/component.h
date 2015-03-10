/*********************************************************************
  *
  * Copyright (C) <name>, <year>
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

#ifndef COMPONENT_H
#define COMPONENT_H

#include <QQuickItem>
#include <QQmlParserStatus>

#include "xmas.h"

/**
 * @brief The Component class
 *
 * A wrapper around XMASComponent from xmas.h to connect to
 * Qml using properties.
 */

namespace model {

class Component : public QQuickItem //, public QQmlParserStatus
{
public:
    enum Orientation {
        North = 0,
        East = 90,
        South = 180,
        West = 270,
        NorthWest = 45,
        SouthWest = 225,
        NorthEast = 315,
        SouthEast = 135
    };
    enum CompType {Unknown=0, Source, Sink, Function, Queue, Join, Merge, Switch, Fork, In, Out, Composite};

private:
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)
    Q_ENUMS(Orientation)
    Q_ENUMS(CompType)
    Q_PROPERTY(QString name READ getName WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(CompType type READ getType WRITE setType NOTIFY typeChanged)
    Q_PROPERTY(QVariant expression READ getExpression WRITE setExpression NOTIFY expressionChanged)
    //@Guus valid true = expression ok , in de emit van validChanged kunnen we de positie van de fout meegeven
    Q_PROPERTY(bool valid READ getValid WRITE setValid NOTIFY validChanged)

public:
    explicit Component(QQuickItem *parent = 0);
    ~Component();

    //enum { Type = QVariant::UserType + 0 };

signals:
    void nameChanged();
    void typeChanged();
    void expressionChanged();
    void validChanged(int errorPosition);
    void changeName(QString old_name, QString name);
    void writeLog(QString message, QColor color = Qt::blue);

public slots:

public:

    virtual void classBegin();
    virtual void componentComplete();

    CompType getType() const {return m_type;}
    void setType(CompType type) {
        m_type = type;
        emit typeChanged();
    }

    QString getName() {
        return m_name;
    }

    void setName(QString name) {
        if (name != m_name) {
            m_name = name;
            if (m_component) {
                m_component->name(name.toStdString());
            }
        }
        emit nameChanged();
    }

    // TODO: find out how to store specifications
    QVariant getExpression() {
        return m_expression;
    }

    void setExpression(QVariant expression) {
        m_expression = expression;
        //TODO check expression en emit valid changed with -1 if ok , or > -1 if not where int is position error
        setValid(true);
        emit expressionChanged();
    }

    // TODO: Syntax check of expressions
    void param(QVariant param) {
        QString typeName = QString(param.typeName());
        writeLog(QString("param heeft type '")+typeName+"'");

        if (getType() == Queue) {
            if (param.typeName() == "int") {

            }
            // queue
        } else if (getType() == Source) {
            // source
            // TODO: syntax check
        } else if (getType() == Function) {
            // function
            // TODO: syntax
        } else if (getType() == Join) {
            // join
            // TODO: expressies? waarschijnlijk / unrestricted
        } else if (getType() == Switch) {
            // switch
            // TODO: expressie?
        }

        m_param = param;
	}

    bool getValid() {
        return m_valid;
    }

    void setValid(bool valid) {
        m_valid = valid;
        //TODO : replace -1 with error position from parser
        emit validChanged(-1);
	}

private:
    XMASComponent *createComponent(CompType type, QString name);

public:
private:
    QString m_name;
    QVariant m_expression;
    CompType m_type;
    bool m_valid;
    XMASComponent *m_component;
};

} // namespace model

#endif // COMPONENT_H
