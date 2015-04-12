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
#include "canvascomponentextension.h"
#include "composite-network-extension.h"

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
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(CompType type READ type WRITE setType NOTIFY typeChanged)
    //only for queue
    Q_PROPERTY(unsigned int capacity READ capacity WRITE setCapacity NOTIFY capacityChanged)
    // for source/join/switch/function
    Q_PROPERTY(QString expression READ expression WRITE setExpression NOTIFY expressionChanged)
    Q_PROPERTY(bool expressionValid READ expressionValid WRITE setExpressionValid NOTIFY expressionChanged)
    Q_PROPERTY(int expressionErrorPosition READ expressionErrorPosition NOTIFY expressionChanged)
    // sink & source
    Q_PROPERTY(bool required READ required WRITE setRequired NOTIFY requiredChanged)


signals:
    void nameChanged(int result);
    void typeChanged();
    void capacityChanged();
    void expressionChanged(bool success);
    void requiredChanged();
    void changeName(QString old_name, QString name);
    void writeLog(QString message, QColor color = Qt::blue);
    void componentAdded();

public slots:
    QVariantMap getPorts(); //xmas ports

    CompType type() const;
    void setType(CompType type);

    QString name();
    void setName(QString name);

    QString expression();
    void setExpression(QString expression);

    int expressionErrorPosition();
    void setExpressionErrorPosition(int error);

    bool expressionValid();
    void setExpressionValid(bool valid);

    unsigned int capacity();
    void setCapacity(unsigned int capacity);

    bool required();
    void setRequired(bool required);

    void updateCanvasData();

public:

    explicit Component(QQuickItem *parent = 0);
    ~Component();

    XMASComponent *xmas_component();

private:
    bool addXmasComponent();
    bitpowder::lib::MemoryPool &mp();


    QString m_name;
    CompType m_type;
    unsigned int m_capacity;
    bool m_required;
    QString m_expression;
    int m_expressionErrorPosition;
    bool m_expressionValid = false;
};
} // namespace model

#endif // COMPONENT_H
