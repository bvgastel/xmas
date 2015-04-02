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
#include <QQmlListProperty>
#include "xmas.h"

namespace model
{
class Component : public QQuickItem
{
    Q_OBJECT

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
    Q_ENUMS(Orientation)
    Q_ENUMS(CompType)
    Q_PROPERTY(QString name READ getName WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(CompType type READ getType WRITE setType NOTIFY typeChanged)
    Q_PROPERTY(QVariant expression READ getExpression WRITE setExpression NOTIFY expressionChanged)
    Q_PROPERTY(bool validExpr READ getValidExpr WRITE setValidExpr NOTIFY validExprChanged)
    Q_PROPERTY(bool valid READ getValid NOTIFY validChanged)

public:
    explicit Component(QQuickItem *parent = 0);
    ~Component();

signals:
    void nameChanged(int result);
    void typeChanged();
    void expressionChanged(int result);
    void validChanged();
    void validExprChanged(int errorPosition, QString errMsg);

    void changeName(QString old_name, QString name);
    void writeLog(QString message, QColor color = Qt::blue);

public slots:

    QVariantMap getPorts();

public:

    CompType getType() const;
    void setType(CompType type);

    QString getName();
    void setName(QString name);

    QVariant getExpression();
    void setExpression(QVariant expression);

    bool getValidExpr();
    void setValidExpr(bool validExpr);
    void setValidExpr(bool validExpr, int pos, QString errMsg);

    bool getValid();

    XMASComponent *xmas_component();

private:
    int updateExpression(QVariant expression);
    bitpowder::lib::MemoryPool &mp();       // Retrieves XMASProject->m_mp

public:
private:

    QString m_name;
    /**
     * @brief m_expression the expression entered (if syntactically incorrect)     *
     */
    QVariant m_expression;

    CompType m_type;
    bool m_valid;       /* Is the object fully connected? */
    bool m_validExpr;   /* Is the expression correctly updated in xmas? */
};
} // namespace model

#endif // COMPONENT_H
