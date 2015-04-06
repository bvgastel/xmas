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
#include "xmas.h"

namespace model
{
class Component : public QQuickItem
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
    Q_PROPERTY(QString name READ name WRITE name NOTIFY nameChanged)
    Q_PROPERTY(CompType type READ type WRITE type NOTIFY typeChanged)
    Q_PROPERTY(QVariant expression READ expression WRITE expression NOTIFY expressionChanged)
    Q_PROPERTY(bool validExpr READ validExpr WRITE validExpr NOTIFY validExprChanged)
    Q_PROPERTY(bool valid READ valid NOTIFY validChanged)

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

    void componentAdded();

public slots:
    QVariantMap getPorts();

    CompType type() const;
    void type(CompType type);

    QString name();
    void name(QString name);

    QVariant expression();
    void expression(QVariant expression);

    bool validExpr();
    bool valid();

    void updateCanvasData();

public:
    void validExpr(bool validExpr);
    void validExpr(bool validExpr, int pos, QString errMsg);

    virtual void classBegin();
    virtual void componentComplete();

    XMASComponent *xmas_component();

private:
    bool addXmasComponent();
    bool addComposite();
    int updateExpression(QVariant expression);
    int updateExpression();
    bitpowder::lib::MemoryPool &mp();       // Retrieves XMASProject->m_mp

public:
private:

    QString m_name;
    QVariant m_expression;  /* Expression, even if not accepted by xmas */

    CompType m_type;
    bool m_valid;       /* Is the object fully connected? */
    bool m_validExpr;   /* Is the expression correctly updated in xmas? */
};
} // namespace model

#endif // COMPONENT_H
