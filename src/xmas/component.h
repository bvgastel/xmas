/*********************************************************************
 *
 * Copyright (C) Guus Bonnema, 2014
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

#include <memory>

#include <QString>
#include <QMap>

#include "xmas_global.h"
#include "inport.h"
#include "outport.h"

/**
 * @brief The Component class
 *
 * The component on an Noc. It contains ports (both
 * in ports and out ports.
 *
 *
 */
class XMASSHARED_EXPORT Component
{

public:
    Component(QString name);
    virtual ~Component() {

    }

    const QString name() const;

private:

    /**
     * @brief m_name the name of the component
     */
    QString m_name;
    /**
     * @brief m_inport_map A map of input ports by name
     */
    QMap<QString, std::shared_ptr<InPort>> m_inport_map;
    /**
     * @brief m_outport_map A map of output ports by name
     */
    QMap<QString, std::shared_ptr<OutPort>> m_outport_map;

    /**
     * @brief m_function a function body
     *
     * The function body is empty if not applicable.
     * It is unstructured information for the verification
     * tools and the reader of the component.
     *
     */
    QString m_function;


};

#endif // COMPONENT_H
