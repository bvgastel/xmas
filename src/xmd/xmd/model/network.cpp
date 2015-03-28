#include <QList>
#include <QUrl>

#include "util.h"
#include "network.h"

model::Network::Network(QQuickItem *parent)
    : QQuickItem(parent)
{
}

model::Network::~Network()
{

}

QString model::Network::packet() {
 return m_packet ;
}

void model::Network::setPacket(QString expression) {
    if (expression != m_packet) {
        m_packet = expression;
    }
    emit packetChanged();
    qDebug() << "packet expression = " << m_packet;
}

QList<model::Component *> model::Network::getAllComponents() {
    QList<Component *> componentList;
    componentList.clear();

    for(QQuickItem *item : this->childItems()) {
        Component *c = qobject_cast<Component *>(item);
        if(c){
            componentList.append(c);
        }
    }
    qDebug() << "Total comps in list = " << componentList.count();
    return componentList;
}

/*
 * portError(port, errMsg) always return false to support error messaging.
 *
 */
bool model::Network::portError(XPort *port, QString errMsg) {
    if (port) {
        errMsg += (port->getType() == model::XPort::OUTPORT ? " port = output_port! "
                                                               : " port = input_port! ");
        errMsg += port->getComponent()->getName()+"."+port->getName() + ". ";
        if (!port->getConnected()) {
            errMsg += "Port is not connected. ";
        }
    }
    emit writeLog(errMsg, Qt::red);
    qDebug() << errMsg;
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
    qDebug() << errMsg;
    return false;
}

bool model::Network::xmasConnectOk(Output *xmas_outport, Input *xmas_inport) {
    if (!xmas_inport || !xmas_outport) {
        return xmasError(xmas_outport, xmas_inport, "[Network::connect()] Connect failed: inport or outport null. ");
    }
    if (xmas_inport->isConnected() || xmas_outport->isConnected()) {
        return xmasError(xmas_outport, xmas_inport, "[Network::connect()] Connect failed: inport or outport already connected. ");
    }
    return true;
}

bool model::Network::connect(Output *xmas_outport, Input *xmas_inport) {
    if (xmasConnectOk(xmas_outport, xmas_inport)) {
        ::connect(*xmas_outport, *xmas_inport);
        return true;
    }
    return false;
}

/*
 * @brief model::Network::xmasDisconnectOk
 *
 *  The Qml system calls the disconnect on request of the qml
 * javascript both explicitly and implicitly. Due to the way Qml
 * works, at closing time, Qml might ask for disconnect of ports
 * that are not connected. This is not an error. The cause is a
 * difference in structure on the canvas and in xmas-components.
 *
 * On the canvas the channel is an object that has 2 ports that are
 * also objects. When deleting the canvas, Qml deletes each port in
 * sequence. This causes two disconnect requests in sequence. The first
 * will always be ok, as the components are connected. The second
 * will meet with components that are already disconnect. In xmas
 * the disconnects leads to automatic disconnection of both ends of
 * the channel, while in Qml the system tries to disconnect both ports
 * in sequence. So, we ignore this fact en execute the disconnect anyways,
 * because xmas.cpp will not do anything for already disconnected ports.
 *
 * For that reason the disconnect responds to a request to disconnect
 * already disconnected ports as if the disconnect was successful.
 *
 * @param xmas_outport
 * @param xmas_inport
 * @return
 */
bool model::Network::xmasDisconnectOk(Output *xmas_outport, Input *xmas_inport) {
    if (!xmas_outport->connectedTo(xmas_inport->m_owner)) {
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

/**
 * Connect method for qml
 *
 * @brief model::Network::connect
 * @param outport
 * @param inport
 * @return xmas connection succeeded
 */
bool model::Network::connect(XPort *outport, XPort *inport) {

    // Check ports
    if (!outport) return portError(outport, "[Network::connect()] outport is null: ");
    if (!inport) return portError(inport, "[Network::connect()] inport is null.");

    // obtain and check xmas inport and outport
    Output *xmas_outport = dynamic_cast<Output *>(outport->getPort());
    Input *xmas_inport = dynamic_cast<Input *>(inport->getPort());
    bool success = connect(xmas_outport, xmas_inport);

    if (success) {
        emit outport->connectedChanged();
        emit inport->connectedChanged();
    }
    return success;
}

bool model::Network::disconnect(XPort *outport, XPort *inport) {

    // check outport
    if (!outport) return portError(outport, "[Network::disconnect()] outport is null: ");
    if (!inport) return portError(inport, "[Network::disconnect()] inport is null.");

    //  check outport for being xmas outport to inport
    Output *xmas_outport = dynamic_cast<Output *>(outport->getPort());
    Input *xmas_inport = dynamic_cast<Input *>(inport->getPort());
    if (xmas_outport == nullptr || xmas_inport == nullptr) {
        xmasError(xmas_outport, xmas_inport, "[model::Network::disconnect()] xmas ports null: ");
        return false;
    }
    if (disconnect(xmas_outport, xmas_inport)) {
        emit outport->connectedChanged();
        emit inport->connectedChanged();
        return true;
    }

    return false;
}


bool model::Network::openFile(QUrl url) {

    QString json = Util::openFile(url);
    // TODO: convert json to XMASComponent
    return true;
}

bool model::Network::saveFile(QUrl url) {
    return Util::saveFile(url, toJson());
}

bool model::Network::closeFile() {
    //TODO destroy the model and reset everything
    return true;
}

bool model::Network::newFile() {
    return closeFile();
}

QString model::Network::toJson() {
    bitpowder::lib::String result;
    bitpowder::lib::MemoryPool mp;
    bitpowder::lib::JSONData globals = bitpowder::lib::JSONData::AllocateMap(mp);
    bitpowder::lib::JSONData packet = bitpowder::lib::JSONData::AllocateVector(mp);
    //packet.insert(m_packet);
    globals["PACKET_TYPE"] = packet;
    globals["VARS"] = bitpowder::lib::JSONData::AllocateMap(mp);

    QList<model::Component *> allComponents = getAllComponents();

    std::set<XMASComponent *> allComp;
    for (Component *comp : allComponents) {
        auto c = comp->xmas_component();
        allComp.insert(c);
    }
    result = ::Export(allComp,globals,mp);
     QString jsonString = QString(result.stl().c_str());
    return jsonString;
}

bool model::Network::addComponent(model::Component *component) {
    auto xmas_comp = component->createXMASComponent(component->getType(), component->getName());
    component->xmas_component(xmas_comp);
    bool result;
    std::tie(std::ignore, result) = this->m_xmas_comp_list.insert(xmas_comp);
    return result;
}
