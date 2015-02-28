/*********************************************************************
 *
 * Copyright (C) Guus Bonnema, 2015
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

#ifndef VTPLUGININTERFACE_H
#define VTPLUGININTERFACE_H

#include <map>

#include <QtPlugin>
#include <QColor>
#include <QMap>

#include "xmas.h"
#include "../logger.h"

class VtPluginInterface
{

public:
    virtual ~VtPluginInterface() {}

    virtual QString name() = 0;
    virtual void name(QString name) = 0;

    virtual QMap<QString, QString> parameters() = 0;
    virtual void parameters(QMap<QString, QString> paramMap) = 0;

    virtual bool run(const QString &jsonString) = 0;
    virtual bool run(const QUrl &fileUrl) = 0;

    virtual Logger *logger() = 0;

};

#define VtPluginInterface_iid "nl.ou.xmd.VtPluginInterface/1.0"

Q_DECLARE_INTERFACE(VtPluginInterface, VtPluginInterface_iid)


#endif // VTPLUGININTERFACE

