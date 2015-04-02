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

#include "datacontrol.h"
#include "component.h"

extern DataControl *dataControl;

model::Component::Component(QQuickItem *parent)
    : QQuickItem(parent), m_name()
{
}

model::Component::~Component()
{
}
// This method was made for composite objects.
QVariantMap model::Component::getPorts()
{
    auto project = dataControl->project();
    if (!project) {
        emit writeLog("[model::Component::getPorts()] no project: big problems.");
        return QVariantMap();
    }

    auto comp = xmas_component();
    QVariantMap map;
//    for(Port *p : comp->ports()) {
//        QString name = p->getName();
//        map[name] = typeid(*p) == typeid(Input) ? XPort::PortType::INPORT : XPort::PortType::OUTPORT;
//    }
    map["in0"] = XPort::PortType::INPORT;
//    map["in1"] = XPort::PortType::INPORT;
    map["out"] = XPort::PortType::OUTPORT;
    return map;
}

QString model::Component::getName() {
    return m_name;
}

void model::Component::setName(QString name) {
    // initial setting
    if (m_name == QString()) {
        m_name = name;
        emit nameChanged(true);
        return;
    }
    // change current name
    if (name == m_name) {
        return;
    }
    // get project
    auto project = dataControl->project();
    if (!project) {
        return;
    }
    if (project->changeComponentName(m_name.toStdString(), name.toStdString())) {
        m_name = name;
        emit nameChanged(true);
    }
    emit nameChanged(false);
}

model::Component::CompType model::Component::getType() const {
    return m_type;
}

void model::Component::setType(CompType type) {
    m_type = type;
    emit typeChanged();
}

bool model::Component::getValidExpr() {
    return m_validExpr;
}

void model::Component::setValidExpr(bool validExpr) {
    m_validExpr = validExpr;
    emit validExprChanged(-1, QString(""));
}

void model::Component::setValidExpr(bool validExpr, int pos, QString errMsg) {
    m_validExpr = validExpr;
    emit validExprChanged(pos, errMsg);
}

bool model::Component::getValid() {
    bool result = false;
    auto c = xmas_component();
    if (c) {
        result = c->valid();
    }
    return result;
}

bitpowder::lib::MemoryPool &model::Component::mp() {
    auto project = dataControl->project();
    return project->mp();
}

int model::Component::updateExpression(QVariant expression) {
    QString typeName = QString(expression.typeName());
    emit writeLog(QString("[debug] received expression of type '")+typeName+"' and contents "+expression.toString());

    auto c = xmas_component();
    if (getType() == Queue) {
        if (typeName != "int") {
            setValidExpr(false, 0, QString("Received non integer size."));
            return 0;
        }
        m_expression = expression;
        int size = expression.toInt();
        XMASQueue *queue = dynamic_cast<XMASQueue *>(c);
        if (!queue) {
            emit writeLog(QString("Fatal error in Component: "
                             "did not recognize m_component as queue."));
            throw bitpowder::lib::Exception("Fatal error in Component.");
        }
        queue->c = size;
        setValidExpr(true, -1, QString(""));
        emit writeLog(QString("queuesize set: ")+size);
        return -1;
    } else if (getType() == Source) {
        if (typeName != "QString") {
            return 0;
        }
        m_expression = expression;
        QString qexpr = expression.toString();
        XMASSource *source = dynamic_cast<XMASSource *>(c);
        if (source) {
            std::string expr = qexpr.toStdString();
            auto result = source->setSourceExpression(expr, mp());
            QString errMsg = QString(result.m_errMsg.stl().c_str());
            setValidExpr(result.m_success, result.m_pos, errMsg);
            QString xmas_expression =
                    result.m_success ? QString(source->getSourceExpression(mp())
                                               .stl().c_str())
                                     : "<should never show>";
            emit writeLog(QString("saving expression in XMASComponent ")
                     + (result.m_success? "succeeded." + xmas_expression
                                        : "failed. Error message is:" + errMsg));
            return result.m_pos;
        } else {
            std::cerr << "Fatal error in Component: did not recognize m_component"
                         " as source." << std::endl;
            throw bitpowder::lib::Exception("Fatal error in Component.");
        }
    } else if (getType() == Function) {
        if (typeName != "QString") {
            return 0;
        }
        m_expression = expression;
        QString qexpr = expression.toString();
        XMASFunction *func = dynamic_cast<XMASFunction *>(c);
        if (func) {
            std::string expr = qexpr.toStdString();
            auto result =  func->setFunctionExpression(expr, mp());
            QString errMsg = QString(result.m_errMsg.stl().c_str());
            setValidExpr(result.m_success, result.m_pos, errMsg);
            emit writeLog(QString("saving expression in XMASComponent ")
                     + (result.m_success? "succeeded." : "failed. Error message is:" + errMsg));
            if (result.m_success) {
                QString xmas_expression = QString(func->getFunctionExpression(mp()).stl().c_str());
                emit writeLog(QString("result = ") + xmas_expression );
            }
            return result.m_pos;
        } else {
            std::cerr << "Fatal error in Component: did not recognize m_component"
                         " as function." << std::endl;
            throw bitpowder::lib::Exception("Fatal error in Component.");
        }
        return true;
    } else if (getType() == Join) {
        if (typeName != "QString") {
            return 0;
        }
        m_expression = expression;
        QString qexpr = expression.toString();
        XMASJoin *join = dynamic_cast<XMASJoin *>(c);
        if (join) {
            std::string expr = qexpr.toStdString();
            ExpressionResult result;
            QString kindOfJoin;
            if (expr == "0" || expr == "1") {
                kindOfJoin = "restricted";
                result = join->setRestrictedJoinPort(expr, mp());
            } else {
                kindOfJoin = "unrestricted";
                result = join->setUnrestrictedJoinExpression(expr, mp());
            }
            QString errMsg = QString(result.m_errMsg.stl().c_str());
            setValidExpr(result.m_success, result.m_pos, errMsg);
            QString xmas_expression = result.m_success ? QString(join->getJoinExpression(mp()).stl().c_str())
                                                       : "<Should never show>";
            xmas_expression = xmas_expression == "" ? "<no value returned>" : xmas_expression;
            emit writeLog(QString("saving ")+ kindOfJoin + QString(" join expression in XMASComponent ")
                     + (result.m_success? "succeeded. Stored \"" + xmas_expression + "\""
                                        : "failed. Error message is:" + errMsg));
            emit writeLog(QString("Function for Join is not implemented yet."));
            return result.m_pos;
        } else {
            std::cerr << "Fatal error in Component: did not recognize m_component"
                         " as join." << std::endl;
            throw bitpowder::lib::Exception("Fatal error in Component.");
        }
        return true;
    } else if (getType() == Switch) {
        if (typeName != "QString") {
            return 0;
        }
        m_expression = expression;
        QString qexpr = expression.toString();
        XMASSwitch *sw = dynamic_cast<XMASSwitch *>(c);
        if (sw) {
            std::string expr = qexpr.toStdString();
            auto result =  sw->setSwitchExpression(expr, mp());
            QString errMsg = QString(result.m_errMsg.stl().c_str());
            setValidExpr(result.m_success, result.m_pos, errMsg);
            QString xmas_expression = result.m_success ? QString(sw->getSwitchExpression(mp()).stl().c_str())
                                                       : "<Should never show>";
            emit writeLog(QString("saving expression in XMASComponent ")
                     + (result.m_success? "succeeded." + xmas_expression
                                        : "failed. Error message is:" + errMsg));
            return result.m_pos;
        } else {
            std::cerr << "Fatal error in Component: did not recognize m_component"
                         " as switch." << std::endl;
            throw bitpowder::lib::Exception("Fatal error in Component.");
        }
        return true;
    }

    emit writeLog(QString("Component type not recognized for expression: ")+getType());
    return false;
}

XMASComponent *model::Component::xmas_component() {
    auto project = dataControl->project();
    if (!project) {
        return nullptr;
    }

    auto network = project->getRootNetwork();
    if (!network) {
        return nullptr;
    }

    bitpowder::lib::String name = bitpowder::lib::String(getName().toStdString().c_str());
    auto xmap = network->getComponentMap();
    auto c = xmap[name];
    if (!c) {
        return nullptr;
    }

    return c;
}

QVariant model::Component::getExpression() {
    auto c = xmas_component();
    // In case of queue return queue size
    auto queue = dynamic_cast<XMASQueue *>(c);
    if (queue) {
        qulonglong expr = queue->c;
        return expr;
    }
    // In case of function return function specification.
    auto func = dynamic_cast<XMASFunction *>(c);
    if (func) {
        auto expr = func->getFunctionExpression(mp()).stl();
        if (expr != "") {
            return QString(expr.c_str()); // Only return xmas string, if useful
        } else {
            return m_expression;
        }
    }
    // In case of source return source specification.
    auto src = dynamic_cast<XMASSource *>(c);
    if (src) {
        auto expr = src->getSourceExpression(mp());
        if (expr != "") {
            return QString(expr.stl().c_str());  // Only return xmas string, if useful
        } else {
            return m_expression;
        }
    }
    // In case of switch return switch expression
    auto sw = dynamic_cast<XMASSwitch *>(c);
    if (sw) {
        auto expr = sw->getSwitchExpression(mp());
        if (expr != "") {
            return QString(expr.stl().c_str());  // Only return xmas string, if useful
        } else {
            return m_expression;
        }
    }
    // In case of join return join expression
    auto join = dynamic_cast<XMASJoin *>(c);
    if (join) {
        auto expr = join->getJoinExpression(mp());
        if (expr != "") {
            return QString(expr.stl().c_str());  // Only return xmas string, if useful
        } else {
            return m_expression;
        }
    }
    return m_expression;
}

void model::Component::setExpression(QVariant expression) {
    int errorPosition = -1;
    errorPosition = updateExpression(expression);
    emit expressionChanged(errorPosition);
}

