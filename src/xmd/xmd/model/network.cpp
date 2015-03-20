#include "network.h"

model::Network::Network(QQuickItem *parent)
    : QQuickItem(parent)
{
}

model::Network::~Network()
{

}
/** Connect method for qml */
bool model::Network::connect(XPort *outport, XPort *inport) {
    if (!outport) {
        emit writeLog(QString("[Network::connect()] outport is null."), Qt::red);
        return false;
    }
    if (!inport) {
        emit writeLog(QString("[Network::connect()] inport is null."), Qt::red);
        return false;
    }
    Output *xmas_outport = dynamic_cast<Output *>(outport->getPort());
    Input *xmas_inport = dynamic_cast<Input *>(inport->getPort());
    if (xmas_inport && xmas_outport) {
        ::connect(*xmas_outport, *xmas_inport);
        return true;
    }
    QString errMsg = "[Network::connect()] ";
    errMsg += (!xmas_inport ? (xmas_outport ? "xmas_inport and _outport are null."
                                           : "xmas_inport is null.")
                            : "xmas_outport is null.");
    emit writeLog(errMsg, Qt::red);
    return false;
}
/** Disconnect method from output port for qml
 *
 *  Remark that we can disconnect from either input or output port.
 *  It does not matter for the result which we use.
 *  Port must be connected for disconnect to work.
 *
 *
 *
 */
bool model::Network::disconnect(XPort *port) {
    if (!port) {
        emit writeLog(QString("[Network::disconnect()] port is null."));
        return false;
    }
    Output *outport = dynamic_cast<Output *>(port->getPort());
    Input *inport = dynamic_cast<Input *>(port->getPort());
    if (outport && outport->isConnected()) {
        ::disconnect(*outport);
        return true;
    } else if (inport && inport->isConnected()) {
        ::disconnect(*inport);
        return true;
    }
    return false;
}



