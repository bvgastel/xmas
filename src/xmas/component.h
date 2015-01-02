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

class Component;

/**
 * @brief The Component class
 *
 * The component on an Noc. It contains ports (both
 * in ports and out ports.
 *
 * This class contains the nested definitions and declarations of Port
 * InPort and OutPort. This reflects the tight coupling : outside the
 * Component these objects would not exist.
 *
 *
 */
class XMASSHARED_EXPORT Component
{

    friend std::ostream &operator<< (std::ostream &os, const Component &comp);

public:
    /* Nested classes */
    class Port;
    class InPort;
    class OutPort;

    Component(QString name=0);
    virtual ~Component()
    {

    }

    const QString name() const;
    void name(const QString name);

    Component &add(std::shared_ptr<Component::InPort>);
    Component &add(std::shared_ptr<Component::OutPort>);

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


public:
    /**
     * @brief The Port class
     *
     * An abstract class as a base class for in ports and out ports. Most
     * of the functionality is in the base class.
     * This class is nested inside Component as it is inherently
     * connected to Component.
     *
     */
    class Port
    {
    public:
        Port(QString name, std::shared_ptr<Component> comp);
        ~Port();

        const QString name() const;

    private:

        QString m_name;
        std::shared_ptr<Component> m_comp;

    };
    /**
     * @brief The InPort class
     *
     * The specialised port for reading signals on an NoC wire.
     * It inherits the name and component from Port.
     * This class is nested inside Component as it is inherently
     * connected to Component.
     *
     */
    class InPort : public Port
    {
    public:
        InPort(QString name, std::shared_ptr<Component> comp, QString trdy);
        ~InPort();

        const QString trdy() const
        {
            return m_trdy;
        }

    private:
        const QString m_trdy;
    };

    /**
     * @brief The OutPort class
     *
     * The specialised class for writing signals on an NoC wire.
     * This class is nested inside Component as it is inherently
     * connected to Component.
     *
     */
    class OutPort : public Port
    {
    public:
        OutPort(QString name, std::shared_ptr<Component> comp, QString irdy);
        ~OutPort();

        const QString irdy() const
        {
            return m_irdy;
        }

    private:

        /**
         * @brief m_irdy contains the definition of o.irdy if o is the output port.
         *
         * The definition stems from the xmas primitives or a derivative.
         */
        const QString m_irdy;
    };


};

#endif // COMPONENT_H
