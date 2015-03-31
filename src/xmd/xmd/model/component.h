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
#include <QQmlListProperty>

//#include "xmas.h"
#include "port.h"

/**
 * @brief The Component class
 *
 * A wrapper around XMASComponent from xmas.h to connect to
 * Qml using properties.
 */

namespace model
{
class Component : public QQuickItem //, public QQmlParserStatus
{
    Q_OBJECT

    friend class Network;
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
    enum CompType {Unknown=0, Source, Sink, Function, Queue, Join, Merge, Switch, Fork, Composite};

private:
    Q_INTERFACES(QQmlParserStatus)
    Q_ENUMS(Orientation)
    Q_ENUMS(CompType)
    Q_PROPERTY(QString name READ getName WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(CompType type READ getType WRITE setType NOTIFY typeChanged)
    Q_PROPERTY(QVariant expression READ getExpression WRITE setExpression NOTIFY expressionChanged)
    Q_PROPERTY(bool validExpr READ getValidExpr WRITE setValidExpr NOTIFY validExprChanged)
    Q_PROPERTY(bool valid READ getValid NOTIFY validChanged)
    Q_PROPERTY(QQmlListProperty<model::XPort> ports READ ports)

public:
    explicit Component(QQuickItem *parent = 0);
    ~Component();

signals:
    void nameChanged(int result);
    void typeChanged();
    void expressionChanged(int result);
    void validChanged();
    void validExprChanged(int errorPosition, QString errMsg);

    /* Mainly for composites. Primitives use fixed names and types. */
    void inportProperties(QVariantList portList);
    void outportProperties(QVariantList portList);

    void changeName(QString old_name, QString name);
    void writeLog(QString message, QColor color = Qt::blue);

public slots:

public:

    virtual void classBegin();
    virtual void componentComplete();

    CompType getType() const;
    void setType(CompType type);

    QString getName();
    void setName(QString name);

    /**
     * @brief getExpression the getter for expression.
     *
     * @return the xmas value of the expression or if that is empty the class member m_expression.
     */
    QVariant getExpression();
    void setExpression(QVariant expression);

    bool getValidExpr();
    void setValidExpr(bool validExpr);

    void setValidExpr(bool validExpr, int pos, QString errMsg);

    /**
     * @brief getValid
     * @return
     */
    bool getValid();

    XMASComponent *xmas_component();

    QQmlListProperty<XPort> ports();


private:
    XMASComponent *createXMASComponent(CompType type, QString name);
    int checkName(QString name);
    int updateExpression(QVariant expression);

    void emitInportProperties();
    void emitOutportProperties();

    static void append_port(QQmlListProperty<XPort> *list, XPort *port);

public:
private:
    bitpowder::lib::MemoryPool m_mp;

    QString m_name;
    /**
     * @brief m_expression
     *
     * m_expression contains the expression, even if it is syntactically incorrect.
     * However, as long as it is xmas will not register the expression.
     *
     * Only when correct will xmas register the expression.
     *
     * An incorrect expression will never result in JSON output.
     *
     * The getter for m_expression: getExpression(), preferrably returns
     * the value from xmas. Only if it has no value, does it return the
     * class member field m_expression.
     *
     */
    QVariant m_expression;

    CompType m_type;
    bool m_valid;
    bool m_validExpr;
    bool m_validExprWarningGiven; // an internal flag (non-qml)

    QList<XPort *> m_ports;

};

} // namespace model

#endif // COMPONENT_H
