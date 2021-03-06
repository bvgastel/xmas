#include <QList>
#include <QUrl>

#include "util.h"
#include "commoninterface.h"
#include "network.h"

#include "xmas.h"
#include "canvascomponentextension.h"
#include "composite-network-extension.h"
#include "canvas-network-extension.h"

/* Access to the global datacontrol pointer */
extern DataControl *dataControl;

model::Network::Network(QQuickItem *parent)
    : QQuickItem(parent),
      m_logger("Network.cpp"),
      m_alias(),
      m_size(QSize(2500,2000)),
      m_imageName(),
      m_boxedImage(true)
{
    QObject::connect(&m_logger, SIGNAL(writeLog(QString,QColor)), dataControl, SIGNAL(writeLog(QString,QColor)));
    m_logger.log(QString("Network starting ..."));
}

model::Network::~Network() {}

// set xmas composite network extension data
bool model::Network::setCompositeNetworkData() {
    auto project = dataControl->project();
    if (!project || !project->getRootNetwork()) {
        emit writeLog(QString("This network has no project or no network: big problems!"));
        return false;
    }

    project->getRootNetwork()->setCompositeNetworkData(m_alias.toStdString(),
                                                       m_size.width(),
                                                       m_size.height(),
                                                       m_imageName.toStdString(),
                                                       m_boxedImage);
    return true;
}

// read xmas network packet type
QString model::Network::packet() {
    auto project = dataControl->project();
    if (!project || !project->getRootNetwork()) {
        emit writeLog(QString("This network has no project or no network: big problems!"),Qt::red);
        return "";
    }
    return project->getRootNetwork()->packetType().c_str();
}

// set xmas network packet type
bool model::Network::setPacket(QString expression) {
    auto project = dataControl->project();
    if (!project || !project->getRootNetwork()) {
        emit writeLog(QString("This network has no project or no network: big problems!"),Qt::red);
        return false;
    }
    // TODO: Need to check expression with expression parser? Query for Bernard.
    project->getRootNetwork()->packetType(expression.toStdString());
    m_logger.log(QString("packet set to expression = ") + expression);
    return true;
}

// open xmas network project
bool model::Network::openFile(QUrl fileUrl) {
    std::string filename = Util::fileName(fileUrl);
    if (dataControl->loadNewProject(filename)) {
        emit compositeLibraryChanged();
        auto project = dataControl->project();
        auto result = emitNetwork(*project->getRootNetwork());
        return result;
    }
    return false;
}

// save xmas network project
bool model::Network::saveFile(QUrl fileUrl) {

    auto project = dataControl->project();

    std::string filename =
            fileUrl.isLocalFile() ? fileUrl.toLocalFile().toStdString()
                                  : fileUrl.fileName().toStdString();

    project->saveNetwork(filename);

    return true;
}

// close xmas network project
bool model::Network::closeFile() {
    auto project = dataControl->project();
    project->clear();
    emit compositeLibraryChanged();
    emit packetChanged();
    return true;
}

// new xmas network
bool model::Network::newFile() {
    return closeFile();
}

bool model::Network::emitNetwork(XMASNetwork &network) {

    auto &componentMap = network.getComponentMap();

    std::clock_t c_start = std::clock();
    QVariantList compList;
    for(auto &it : componentMap) {
        XMASComponent *comp = it.second;
        if (comp) {
            QVariantMap map;
            convertToQml(map, comp);
            compList.append(map);
        }
    }
    QVariantList channelList;
    for (auto &it : componentMap) {
        XMASComponent *comp = it.second;
        if (comp) {
            QVariantList list;
            connectInQml(list, comp);
            channelList.append(list);
        }
    }
    QVariantMap qmlNetwork;
    qmlNetwork["complist"] = compList;
    qmlNetwork["channellist"] = channelList;    emit packetChanged();

    emit packetChanged();
    // Var is not implemented in qml yet. No known semantics for var
    //qmlNetwork["var"] = QString(network.getVar().stl().c_str());

    auto cne = network.getNetworkExtension<CompositeNetworkExtension>(false);
    if(cne){
        this->m_alias = cne->alias.c_str();
        this->m_boxedImage = cne->boxedImage;
        this->m_imageName = cne->imageName.c_str();
    }

    auto cve = network.getNetworkExtension<CanvasNetworkExtension>(false);
    if(cve){
        this->m_size = QSize(cve->width,cve->height);
    }

    emit createNetwork(qmlNetwork);
    std::clock_t c_end = std::clock();
    qDebug() << "CPU time used: " << 1000.0 * (c_end-c_start) / CLOCKS_PER_SEC << " ms"
             << " for " << compList.length() << " components and " << channelList.length() << " channels";

    return true;
}

void model::Network::connectInQml(QVariantList &list, XMASComponent *comp) {
    for (Output *out : comp->outputPorts()) {
        if (out->isConnected()) {
            QVariantMap map;
            map.insert("outcomp", QString(out->getInitiator()->getStdName().c_str()));
            map.insert("outport", QString(out->getInitiatorPort()->getName()));
            map.insert("incomp", QString(out->getTarget()->getStdName().c_str()));
            map.insert("inport", QString(out->getTargetPort()->getName()));
            list.append(map);
        } else {
            m_logger.log("output port " + std::string(out->getName()) + " in comp "
                         + out->getComponent()->getStdName() + " is not connected");
        }
    }
}


void model::Network::convertToQml(QVariantMap &map, XMASComponent *comp) {
    std::string name = comp->getStdName();

    std::type_index typeIndex = std::type_index(typeid(*comp));
    model::Component::CompType type = m_type_index_map[typeIndex];
    QString qname = QString::fromStdString(name);

    CanvasComponentExtension *ext = comp->getExtension<CanvasComponentExtension *>();
    if (ext) {
        map.insert("x", ext->x());
        map.insert("y", ext->y());
        map.insert("rotation", ext->orientation());
        map.insert("scale", ext->scale());
    }

    auto project = dataControl->project();
    bitpowder::lib::MemoryPool& mp = project->mp();

    if (type == xqueue) {
        XMASQueue *queue = dynamic_cast<XMASQueue *>(comp);
        unsigned int capacity = 0;
        if (queue) {
            capacity = queue->c;
        }
        map.insert("capacity", capacity);
    } else if (type == xfunction) {
        XMASFunction *func = dynamic_cast<XMASFunction *>(comp);
        QString expression = QString();
        if (func) {
            expression = QString(func->getFunctionExpression(mp).c_str());
        }
        map.insert("expression", expression);
    } else if (type == xsource) {
        XMASSource *source = dynamic_cast<XMASSource *>(comp);
        QString expression = QString();
        bool required = true;
        if (source) {
            expression = QString(source->getSourceExpression(mp).stl().c_str());
            required = source->required_input;
        }
        map.insert("expression", expression);
        map.insert("required", required);
    } else if (type == xsink) {
        XMASSink *sink = dynamic_cast<XMASSink *>(comp);
        bool required = true;
        if (sink) {
            required = sink->required_output;
        }
        map.insert("required", required);
    } else if (type == xjoin) { // NOTE: for now only works for restricted join
        XMASJoin *join = dynamic_cast<XMASJoin *>(comp);
        QString expression = QString();
        if (join) {
            expression = QString(join->getJoinExpression(mp).stl().c_str());
        }
        map.insert("expression", expression);
    } else if (type == xswitch) {
        XMASSwitch *sw = dynamic_cast<XMASSwitch *>(comp);
        QString expression = QString();
        if (sw) {
            expression = QString(sw->getSwitchExpression(mp).stl().c_str());
        }
        map.insert("expression", expression);
    } else if (type == model::Component::Composite) {
        XMASComposite *composite = dynamic_cast<XMASComposite *>(comp);
        QString expression = QString();
        if (composite) {
            auto project = dataControl->project();
            //TODO uncomment once parametric parser is implemented
            expression = QString("");//QString(source->getCompositeExpression(mp).stl().c_str());
            if(project) {
                auto cn = project->getNetwork(composite->getNetwork().getStdName());
                if(cn) {
                    QString filename = QString::fromStdString(composite->getNetwork().getStdName());
                    map.insert("filename", filename);
                    auto cn_ext = cn->getNetworkExtension<CompositeNetworkExtension>(false);
                    if (cn_ext) {
                        map.insert("alias", QString::fromStdString(cn_ext->alias));
                        map.insert("image", QString::fromStdString(cn_ext->imageName));
                        map.insert("boxed", cn_ext->boxedImage);
                        map.insert("parametric", expression!="");
                    }
                }
            }
        }
    }
    map.insert("type", type);
    map.insert("name", qname);
}

/*
 * portError(port, errMsg) always return false to support error messaging.
 *
 */
bool model::Network::portError(XPort *port, QString errMsg) {
    if (port) {
        errMsg += (port->type() == model::XPort::OUTPORT ? " port = output_port! "
                                                         : " port = input_port! ");
        errMsg += port->getComponent()->name()+"."+port->name() + ". ";
        if (!port->isConnected()) {
            errMsg += "Port is not connected. ";
        }
    }
    emit writeLog(errMsg, Qt::red);
    return false;
}

bool model::Network::xmasError(Output *xmas_outport, Input *xmas_inport, QString errMsg) {
    QString out_owner = "null_output";
    QString out_port_name = "null";
    bool out_port_is_connected = false;
    QString in_owner = "null_input";
    QString in_port_name = "null";
    bool in_port_is_connected = false;

    if (xmas_outport) {
        if (xmas_outport->m_owner)
            out_owner = xmas_outport->m_owner->getStdName().c_str();
        out_port_name = xmas_outport->getName();
        out_port_is_connected = xmas_outport->isConnected();
    }
    if (xmas_inport) {
        if (xmas_inport->m_owner)
            in_owner = xmas_inport->m_owner->getStdName().c_str();
        in_port_name = xmas_inport->getName();
        in_port_is_connected = xmas_inport->isConnected();
    }

    errMsg += " " + out_owner+"."+out_port_name;
    errMsg += "->" + in_owner + "." + in_port_name + " ";
    if (in_port_is_connected && out_port_is_connected) {
        errMsg += "(both connected).";
    } else if (in_port_is_connected){
        errMsg += "(inport connected, outport not connected.";
    } else if (out_port_is_connected){
        errMsg += "(inport not connected).";
    } else {
        errMsg += "(both not connected).";
    }

    emit writeLog(errMsg);
    return false;
}

bool model::Network::xmasConnectOk(Output *xmas_outport, Input *xmas_inport) {
    if (!xmas_inport || !xmas_outport) {
        return xmasError(xmas_outport, xmas_inport, "[Network::connect()] Connect failed: inport or outport null. ");
    }
    //    no check for connected: already part of Qml programming.

    return true;
}

bool model::Network::connect(Output *xmas_outport, Input *xmas_inport) {
    if (xmasConnectOk(xmas_outport, xmas_inport)) {
        ::connect(*xmas_outport, *xmas_inport);
        return true;
    }
    return false;
}

bool model::Network::xmasDisconnectOk(Output *xmas_outport, Input *xmas_inport) {
    if (!xmas_outport->connectedTo(xmas_inport->m_owner)) {
        // Ignore requests for disconnecting already disconnected channels.
        // Qml issues 1 disconnect request for each port
        if (xmas_inport && !xmas_inport->isConnected()
                && xmas_outport && !xmas_outport->isConnected()) {
            return true;
        }
        xmasError(xmas_outport, xmas_inport,
                  "[Network::disconnect(xmas)] no out->in connection.");
        return false;
    }
    return true;
}

/*
 * @brief model::Network::disconnect
 *
 * Disconnecting already disconnected ports does not lead to error,
 * because xmas.cpp ::disconnect checks first and deletes after ok.
 * If not output.valid() it does nothing.
 *
 * @param xmas_outport
 * @param xmas_inport
 * @return
 */
bool model::Network::disconnect(Output *xmas_outport, Input *xmas_inport) {
    bool success = xmasDisconnectOk(xmas_outport, xmas_inport);
    if (success) {
        ::disconnect(*xmas_outport);
        return true;
    }
    return false;
}


// add connection (canvas request)
bool model::Network::connect(XPort *outport, XPort *inport) {

    // check outport
    if (!outport) {
        return portError(outport, "[Network::connect()] outport is null: ");
    }
    if (!inport) {
        return portError(inport, "[Network::connect()] inport is null.");
    }
    // obtain and check xmas inport and outport
    Output *xmas_outport = dynamic_cast<Output *>(outport->getPort());
    Input *xmas_inport = dynamic_cast<Input *>(inport->getPort());
    bool success = connect(xmas_outport, xmas_inport);
    emit outport->connectedChanged();
    emit inport->connectedChanged();
    return success;
}

// remove connection (canvas request)
bool model::Network::disconnect(XPort *outport, XPort *inport) {

    // check outport
    if (!outport) {
        return portError(outport, "[Network::disconnect()] outport is null: ");
    }
    if (!inport) {
        return portError(inport, "[Network::disconnect()] inport is null.");
    }
    //  check outport for being xmas outport to inport
    Output *xmas_outport = dynamic_cast<Output *>(outport->getPort());
    Input *xmas_inport = dynamic_cast<Input *>(inport->getPort());
    if (xmas_outport != nullptr && xmas_inport != nullptr) {
        if (disconnect(xmas_outport, xmas_inport)) {
            emit outport->connectedChanged();
            emit inport->connectedChanged();
            return true;
        }
    }
    emit outport->connectedChanged();
    emit inport->connectedChanged();
    return false;
}

// add component (canvas request )
bool model::Network::addComponent(model::Component *component) {

    bool result = component->addXmasComponent();
    if(!result) {
        emit writeLog(QString("XMAS Component not inserted!"), Qt::red);
    }
    return result;
}

// remove component (canvas request)
bool model::Network::removeComponent(model::Component *component) {
    auto project = dataControl->project();
    if (component) {
        auto name = component->name().toStdString();
        bool result = project->removeComponent(name);
        if (!result) {
            auto c = project->getRootNetwork()->getComponent(name);
            if (!c) {
                return true;
            } else {
                return false;
            }
        }
    }
    return true;
}


// load a composite network into xmas
bool model::Network::loadComposite(QUrl url){
    try {
        //1 - send url to xmas and parse as composite
        std::string name = url.toLocalFile().toStdString();
        auto xmas_network = dataControl->project()->loadNetwork(name);
        if(!xmas_network) return false;
        if(addComposite(xmas_network)) {
            emit compositeLibraryChanged();
            return true;
        }
        return false;
    } catch (bitpowder::lib::Exception) {
        return false;
    }
    return true;
}

// unload a composite network
bool model::Network::unloadComposite(QString name){
    if (dataControl->project()->unloadNetwork(name.toStdString())) {
        emit compositeLibraryChanged();
        return true;
    } else {
        return false;
    }
}

// read composite library
QVariantList model::Network::compositeLibrary() {
    m_compositeLibrary.clear();
    auto project = dataControl->project();
    for (auto network : project->getNetworks()) {
        if(network.second.get() != project->getRootNetwork()){
            addComposite(network.second.get());
        }
    }
    return m_compositeLibrary;
}

// add composite to model library
bool model::Network::addComposite(XMASNetwork* xmas_network){
    try {
        auto cn_ext = xmas_network->getNetworkExtension<CompositeNetworkExtension>(false);
        if (!cn_ext) return false;
        QVariantMap map;
        map.insert("filename", QString(xmas_network->getStdName().c_str()));
        map.insert("alias", QString::fromStdString(cn_ext->alias));
        map.insert("symbol", QString::fromStdString(cn_ext->imageName));
        map.insert("boxed", cn_ext->boxedImage);
        m_compositeLibrary.append(map);
    } catch (bitpowder::lib::Exception e) {
        emit writeLog("adding composite failed :" + QString(e.description()),Qt::red);
        return false;
    }
    return true;
}

// clear composite library
bool model::Network::clearCompositeLibrary(){
    m_compositeLibrary.clear();
    emit compositeLibraryChanged();
    return true;
}
