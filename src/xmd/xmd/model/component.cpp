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

/* Access to the global datacontrol pointer */
extern DataControl *dataControl;

model::Component::Component(QQuickItem *parent)
    : QQuickItem(parent),
      m_name(),
      m_type(model::Component::CompType::Unknown)
{
}

model::Component::~Component()
{
}

// get component name (must be unique and same as in xmas)
QString model::Component::name() {
    return m_name;
}

// Name changes need to reset the componentMap in network
// besides changing the name in component.
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
    // Do not allow change name to empty name
    if (m_name != QString() && name.trimmed() == QString()) {
        emit nameChanged(false);
        return;
    }
    // get project
    auto project = dataControl->project();
    if (!project) {
        return;
    }
    // Process change in xmas and xmd
    if (project->changeComponentName(m_name.toStdString(), name.trimmed().toStdString())) {
        m_name = name;
        emit nameChanged(true);
    } else {
       emit writeLog("Component name """ + name + """ allready exists!", Qt::red);
    }
    emit nameChanged(false);
}

// get component type
model::Component::CompType model::Component::type() const {
    return m_type;
}

// set component type (only set once during creation)
void model::Component::setType(CompType type) {
    m_type = type;
    emit typeChanged();
}

// read xmas capacity (queue)
unsigned int model::Component::capacity(){
    try {
        auto queue = dynamic_cast<XMASQueue *>(xmas_component());
        if (queue) m_capacity = (unsigned int) queue->c;
    }
    catch(bitpowder::lib::Exception e){
        emit writeLog(e.description(),Qt::red);
    }
    return m_capacity;
}

// write xmas capacity (queue)
void model::Component::setCapacity(unsigned int capacity) {
    try {
        auto queue = dynamic_cast<XMASQueue *>(xmas_component());
        if (queue) {
            queue->c = capacity;
            m_capacity = queue->c;
        }
    }
    catch(bitpowder::lib::Exception e){
        emit writeLog(e.description(),Qt::red);
    }
}

// read xmas required (source,sink)
bool model::Component::required(){
    try {
        switch(m_type) {
        case Source :
        {
            auto sink = dynamic_cast<XMASSink *>(xmas_component());
            if (sink) m_required = sink->required_output;
            break;
        }
        case Sink :
        {
            auto source = dynamic_cast<XMASSource *>(xmas_component());
            if (source) m_required = source->required_input;
            break;
        }
        default :; //not used for other components
        }
    }
    catch(bitpowder::lib::Exception e){
        emit writeLog(e.description(),Qt::red);
    }
    return m_required;
}

// write xmas required (source,sink)
void model::Component::setRequired(bool required) {
    try {
        switch(m_type) {
        case Sink :
        {
            auto sink = dynamic_cast<XMASSink *>(xmas_component());
            if (sink){
                sink->required_output = required;
                m_required = sink->required_output;
            }
            break;
        }
        case Source :
        {
            auto source = dynamic_cast<XMASSource *>(xmas_component());
            if (source){
                source->required_input = required;
                m_required = source->required_input;
            }
            break;
        }
        default:; //not used for other components
        }
    }
    catch(bitpowder::lib::Exception e){
        emit writeLog(e.description(),Qt::red);
    }
}

// read xmas expression (function,join,source,switch)
QString model::Component::expression() {
    try {
        switch(m_type) {
        case Function :
        {
            auto xmas_comp = dynamic_cast<XMASFunction *>(xmas_component());
            if (xmas_comp) {
                m_expression =  QString(xmas_comp->getFunctionExpression(mp()).c_str());
                emit expressionChanged(m_expression!="");
            }
            break;
        }
        case Source :
        {
            auto xmas_comp = dynamic_cast<XMASSource *>(xmas_component());
            if (xmas_comp) {
                m_expression =  QString(xmas_comp->getSourceExpression(mp()).stl().c_str());
                emit expressionChanged(m_expression!="");
            }
            break;
        }
        case Join :
        {
            auto xmas_comp = dynamic_cast<XMASJoin *>(xmas_component());
            if (xmas_comp){
                m_expression =  QString(xmas_comp->getJoinExpression(mp()).stl().c_str());
                emit expressionChanged(m_expression!="");
            }
            break;
        }
        case Switch :
        {
            auto xmas_comp = dynamic_cast<XMASSwitch *>(xmas_component());
            if (xmas_comp) {
                m_expression = QString(xmas_comp->getSwitchExpression(mp()).stl().c_str());
                emit expressionChanged(m_expression!="");
            }
            break;
        }
        default:; //not used for other components
        }
    }
    catch(bitpowder::lib::Exception e){
        emit writeLog(e.description(),Qt::red);
    }
    return m_expression;
}

// write xmas expression (function,join,source,switch)
void model::Component::setExpression(QString expression) {
    //setExpressionValid(false);
    bool hasUpdated = false;
    try {
        ExpressionResult result;
        std::string expr = expression.toStdString();
        switch(m_type) {
        case Function :
        {
            auto xmas_comp = dynamic_cast<XMASFunction *>(xmas_component());
            if (xmas_comp){
                result =  xmas_comp->setFunctionExpression(expr, mp());
                hasUpdated = true;
            }
            break;
        }
        case Source :
        {
            auto xmas_comp = dynamic_cast<XMASSource *>(xmas_component());
            if (xmas_comp){
                result = xmas_comp->setSourceExpression(expr, mp());
                hasUpdated = true;
            }
            break;
        }
        case Join :
        {
            auto xmas_comp = dynamic_cast<XMASJoin *>(xmas_component());
            if (xmas_comp){
                if (expr == "0" || expr == "1") {
                    result = xmas_comp->setRestrictedJoinPort(expr, mp());
                } else {
                    result = xmas_comp->setUnrestrictedJoinExpression(expr, mp());
                }
                hasUpdated = true;
            }
            break;
        }
        case Switch :
        {
            auto xmas_comp = dynamic_cast<XMASSwitch *>(xmas_component());
            if (xmas_comp){
                result = xmas_comp->setSwitchExpression(expr, mp());
                hasUpdated = true;
            }
            break;
        }
        default:; //not used for other components
        }
        if(result.m_success){
            m_expression = expression;
         } else {
            QMetaObject metaObject = model::Component::staticMetaObject;
            QMetaEnum metaEnum = metaObject.enumerator(metaObject.indexOfEnumerator("CompType"));
            emit writeLog(QString("Invalid expression entered for ") +
                          QString(metaEnum.valueToKey(m_type)) +  " \""  + m_name + "\" !" ,Qt::red);
        }

        if(hasUpdated) {
            setExpressionValid(result.m_success);
            setExpressionErrorPosition(result.m_pos);
        }
    }
    catch(bitpowder::lib::Exception e){
        emit writeLog(e.description(),Qt::red);
    }
}

// read expression error position derived from xmas
int  model::Component::expressionErrorPosition(){
    return m_expressionErrorPosition;
}

// store expression error position and let qml know by emit
void  model::Component::setExpressionErrorPosition(int error){
    m_expressionErrorPosition = error;
    emit expressionChanged(error==0);
}

// read expression valid derived from xmas
bool  model::Component::expressionValid(){
    return m_expressionValid;
}

// store expression valid and let qml know by emit
void model::Component::setExpressionValid(bool valid){
    m_expressionValid = valid;
    emit expressionChanged(valid);
}

// add a new xmas component
bool model::Component::addXmasComponent() {
    auto project = dataControl->project();
    std::string name = m_name.toStdString();
    XMASComponent *xmas_comp = nullptr;
    try {
        switch(m_type) {
        case model::Component::CompType::Source : {
            xmas_comp = project->insertSource(name);
            emit requiredChanged();
            emit expressionChanged(false);
            break;
        }
        case model::Component::CompType::Sink : {
            xmas_comp = project->insertSink(name);
            emit requiredChanged();
            break;
        }
        case model::Component::CompType::Function : {
            xmas_comp = project->insertFunction(name);
            emit expressionChanged(false);
            break;
        }
        case model::Component::CompType::Queue : {
            xmas_comp = project->insertQueue(name);
            emit capacityChanged();
            break;
        }
        case model::Component::CompType::Join : {
            xmas_comp = project->insertJoin(name);
            emit expressionChanged(false);
            break;
        }
        case model::Component::CompType::Merge : {
            xmas_comp = project->insertMerge(name);
            break;
        }
        case model::Component::CompType::Switch : {
            xmas_comp = project->insertSwitch(name);
            emit expressionChanged(false);
            break;
        }
        case model::Component::CompType::Fork : {
            xmas_comp = project->insertFork(name);
            break;
        }
        case model::Component::CompType::Composite : {
            std::string filename = QString(property("filename").toString()).toStdString();
            auto composite = dynamic_cast<XMASComposite *>(project->insertComposite(name, filename));
            if(composite){
                XMASNetwork *network = project->getNetwork(composite->getNetwork().getStdName());
                auto cn_ext = network->getNetworkExtension<CompositeNetworkExtension>(false);
                if (cn_ext) {
                    this->setProperty("alias",QString::fromStdString(cn_ext->alias));
                    this->setProperty("symbol",QString::fromStdString(cn_ext->imageName));
                    this->setProperty("boxed", cn_ext->boxedImage);
                }
                xmas_comp = composite;
            }
            break;
        }
        default :
            emit writeLog(QString("Unknown component type!"), Qt::red);
            return false;
        }
    } catch (bitpowder::lib::Exception e) {
        emit writeLog(QString(e.description()), Qt::red);
    }

    if (xmas_comp){
        emit componentAdded();
    }
    return (xmas_comp);
}

// x, y, rotation and scale are part of QQuickItem
// They need no definition in Component.
void model::Component::updateCanvasData() {
    XMASComponent *c = xmas_component();
    if (c) {
        c->canvasData(x(), y(), rotation(), scale());
    }
}

// This method was made for composite objects.
// So Qml can retrieve xmas portnames and types
QVariantMap model::Component::getPorts()
{
    QVariantMap map;
    XMASComponent *c = xmas_component();
    if(c){
        for(Port *p : c->ports()) {
            QString name = p->getName();
            map[name] = typeid(*p) == typeid(Input) ? XPort::PortType::INPORT : XPort::PortType::OUTPORT;
        }
    }
    return map;
}

// get xmas project memory pool
bitpowder::lib::MemoryPool &model::Component::mp() {
    auto project = dataControl->project();
    return  project->mp();
}

// get xmas component by its name
XMASComponent *model::Component::xmas_component() {
    std::string name = m_name.toStdString();

    auto project = dataControl ? dataControl->project() : nullptr;
    if (!project) {
        emit writeLog("No project: big problems.",Qt::red);
        return nullptr;
    }
    auto network = project ? project->getRootNetwork() : nullptr;
    return (network ? network->getComponent(name) : nullptr);
}
