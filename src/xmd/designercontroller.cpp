#include "designercontroller.h"

DesignerController::DesignerController(Network& network)
  : m_network(network)
{
}

DesignerController::~DesignerController()
{
}

void DesignerController::addChannel(Output &output, Input &input)
{
    m_network.addChannel(output, input);
}

void DesignerController::moveComponent(XMASComponent* component, int x, int y, Orientation orientation)
{
    auto pce = component->getComponentExtension<PositionComponentExtension>();
    *pce = PositionComponentExtension {x, y, orientation};

    emit m_network.componentMoved(component);     // emit a Network signal from the DesignerController??
}
