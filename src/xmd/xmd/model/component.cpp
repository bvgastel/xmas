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
#include "port.h"
#include "component.h"

//#include "xmas.h"
//#include "canvascomponentextension.h"
//#include "composite-network-extension.h"
//#include "canvas-network-extension.h"

/* Access to the global datacontrol pointer */
extern DataControl *dataControl;

model::Component::Component(QQuickItem *parent)
    : QQuickItem(parent), m_name()
{
}

model::Component::~Component()
{
}

void model::Component::classBegin()
{
    // no action
}

void model::Component::componentComplete()
{
    // no action
}

void model::Component::updateCanvasData() {
    auto project = dataControl->project();
    auto network = project->getRootNetwork();
    XMASComponent *c = network->getComponent(name().toStdString());
    if (c) {
        c->canvasData(this->x(), this->y(), this->rotation(), this->scale());
    }
}

// This method was made for composite objects.
// So Qml can retrieve portnames and types
QVariantMap model::Component::getPorts()
{
    auto project = dataControl->project();
    if (!project) {
        emit writeLog("[model::Component::getPorts()] no project: big problems.");
        return QVariantMap();
    }

    auto c = xmas_component();
    QVariantMap map;
    if(c){
        for(Port *p : c->ports()) {
            QString name = p->getName();
            map[name] = typeid(*p) == typeid(Input) ? XPort::PortType::INPORT : XPort::PortType::OUTPORT;
        }
    }
    return map;
}

QString model::Component::name() {
    return m_name;
}

void model::Component::setName(QString name) {
    // initial setting
    if (m_name == QString()) {
        m_name = name;
        emit nameChanged(true);
        return;
    }
    // change current name, only if it's really a change
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

model::Component::CompType model::Component::type() const {
    return m_type;
}

void model::Component::setType(CompType type) {
    m_type = type;
    emit typeChanged();
}

bool model::Component::validExpr() {
    return m_validExpr;
}

void model::Component::setValidExpr(bool isValid) {
    m_validExpr = isValid;
    emit validExprChanged(-1, QString(""));
}

void model::Component::setValidExpr(bool isValid, int pos, QString errMsg) {
    m_validExpr = isValid;
    emit validExprChanged(pos, errMsg);
}

void model::Component::updateProperties(){

}


/**
 * Adds an xmas component into the root network
 *  (initial xmas properties are read and set)
 *
 * @brief model::Component::addXmasComponent
 * @return true if an xmas component is created successful
 */
bool model::Component::addXmasComponent() {
    auto project = dataControl->project();
    bitpowder::lib::MemoryPool& mp = project->mp();
    std::string name = m_name.toStdString();
    try {
        switch(m_type) {
        case model::Component::CompType::Source : {
            auto *source = dynamic_cast<XMASSource *>(project->insertSource(name));
            if(!source) return false;
            bool required = source->required_input;
            m_expression = QString(source->getSourceExpression(mp).stl().c_str());
            this->setProperty("required",required);
            emit componentAdded();
            return true;
        }
        case model::Component::CompType::Sink :{
            auto *sink = dynamic_cast<XMASSink *>(project->insertSink(name));
            if(!sink) return false;
            bool required = sink->required_output;
            this->setProperty("required",required);
            emit componentAdded();
            return true;
        }
        case model::Component::CompType::Function :{
            auto *func = dynamic_cast<XMASFunction *>(project->insertFunction(name));
            if(!func) return false;
            m_expression = QString(func->getFunctionExpression(mp).c_str());
            emit componentAdded();
            return true;
        }
        case model::Component::CompType::Queue :{
            auto *queue = dynamic_cast<XMASQueue *>(project->insertQueue(name));
            if(!queue) return false;
            unsigned int capacity = queue->c;
            this->setProperty("capacity",capacity);
            emit componentAdded();
            return true;
        }
        case model::Component::CompType::Join :{
            auto *join = dynamic_cast<XMASJoin *>(project->insertJoin(name));
            if(!join) return false;
            m_expression = QString(join->getJoinExpression(mp).stl().c_str());
            emit componentAdded();
            return true;
        }
        case model::Component::CompType::Merge :{
            if(!project->insertMerge(name)) return false;
            emit componentAdded();
            return true;
        }
        case model::Component::CompType::Switch :{
            if(!project->insertSwitch(name)) return false;
            emit componentAdded();
            return true;
        }
        case model::Component::CompType::Fork :{
            if(!project->insertFork(name)) return false;
            emit componentAdded();
            return true;
        }
        case model::Component::CompType::Composite : {
            QUrl fileUrl = QUrl(property("url").toString());
            std::string filename = fileUrl.fileName().toStdString();
            auto *composite = dynamic_cast<XMASComposite *>(project->insertComposite(name, filename));
            if(!composite) return false;
            XMASNetwork *network = project->getNetwork(composite->getNetwork().getStdName());
            auto cn_ext = network->getNetworkExtension<CompositeNetworkExtension>(false);
            if (cn_ext) {
                this->setProperty("alias",QString::fromStdString(cn_ext->alias));
                this->setProperty("symbol",QString::fromStdString(cn_ext->imageName));
                this->setProperty("boxed", cn_ext->boxedImage);
            }
            emit componentAdded();
            return true;
        }
        default :
            emit writeLog(QString("Unknown component type!"), Qt::red);
            return false;
        }
    } catch (bitpowder::lib::Exception e) {}      // component already exists, ignore

    return false;
}


bool model::Component::valid() {
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
    m_expression = expression;
    return updateExpression();
}

int model::Component::updateExpression() {
    QString typeName = QString(m_expression.typeName());
    emit writeLog(QString("[debug] received expression of type '")+typeName+"' and contents "+m_expression.toString());

    auto xmas_comp = xmas_component();
    if (!xmas_comp) {
        // some calls are before component is complete: without name, no xmas component.
        return false;
    }

    if (type() == Queue) {
        if (typeName != "QString") {
            return false;
        }
        unsigned int qexpr = m_expression.toUInt();
        auto queue = dynamic_cast<XMASQueue *>(xmas_comp);
        if (queue) {
            if (qexpr != queue->c) {
                queue->c = qexpr;
                return 0;
            }
        }
        return false;
    }

    if (type() == Source) {
        if (typeName != "QString") {
            return 0;
        }
        QString qexpr = m_expression.toString();
        XMASSource *source = dynamic_cast<XMASSource *>(xmas_comp);
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
                         " as source : " << xmas_comp->getStdName() << std::endl;
            return false;
        }
    } else if (type() == Function) {
        if (typeName != "QString") {
            return 0;
        }
        QString qexpr = m_expression.toString();
        XMASFunction *func = dynamic_cast<XMASFunction *>(xmas_comp);
        if (func) {
            std::string expr = qexpr.toStdString();
            auto result =  func->setFunctionExpression(expr, mp());
            QString errMsg = QString(result.m_errMsg.stl().c_str());
            setValidExpr(result.m_success, result.m_pos, errMsg);
            emit writeLog(QString("saving expression in XMASComponent ")
                          + (result.m_success? "succeeded." : "failed. Error message is:" + errMsg));
            if (result.m_success) {
                std::string expr = func->getFunctionExpression(mp());
                QString xmas_expression = QString(expr.c_str());
                emit writeLog(QString("result = ") + xmas_expression );
            }
            return result.m_pos;
        } else {
            std::cerr   << "Fatal error in Component: did not recognize m_component"
                           " as function : "
                        << xmas_comp->getStdName() << std::endl;
            return false;
        }
        return true;
    } else if (type() == Join) {
        if (typeName != "QString") {
            return 0;
        }
        QString qexpr = m_expression.toString();
        XMASJoin *join = dynamic_cast<XMASJoin *>(xmas_comp);
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
                emit writeLog(QString("Function for unrestricted Join is not implemented yet."));
            }
            QString errMsg = QString(result.m_errMsg.stl().c_str());
            setValidExpr(result.m_success, result.m_pos, errMsg);
            QString xmas_expression = result.m_success ? QString(join->getJoinExpression(mp()).stl().c_str())
                                                       : "<Should never show>";
            xmas_expression = xmas_expression == "" ? "<no value returned>" : xmas_expression;
            emit writeLog(QString("saving ")+ kindOfJoin + QString(" join expression in XMASComponent ")
                          + (result.m_success? "succeeded. Stored \"" + xmas_expression + "\""
                                             : "failed. Error message is:" + errMsg));
            return result.m_pos;
        } else {
            std::cerr << "Fatal error in Component: did not recognize m_component"
                         " as join : " << xmas_comp->getStdName() << std::endl;
            return false;
        }
        return true;
    } else if (type() == Switch) {
        if (typeName != "QString") {
            return 0;
        }
        QString qexpr = m_expression.toString();
        XMASSwitch *sw = dynamic_cast<XMASSwitch *>(xmas_comp);
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
                         " as switch : " << xmas_comp->getStdName() << std::endl;
            return false;
        }
        return true;
    }

    emit writeLog(QString("Component type not recognized for expression: ")+type());
    return false;
}

XMASComponent *model::Component::xmas_component() {
    std::string stdName = name().toStdString();

    auto project = dataControl ? dataControl->project() : nullptr;
    auto network = project ? project->getRootNetwork() : nullptr;
    return (network ? network->getComponent(stdName) : nullptr);
}

QVariant model::Component::expression() {
    auto c = xmas_component();
    // In case of queue return queue size
    auto queue = dynamic_cast<XMASQueue *>(c);
    if (queue) {
        return (int) queue->c;
    }
    // In case of function return function specification.
    auto func = dynamic_cast<XMASFunction *>(c);
    if (func) {
        auto expr = func->getFunctionExpression(mp());
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
    m_expression = expression;
    errorPosition = updateExpression(expression);
    emit expressionChanged(errorPosition);
}
