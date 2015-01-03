/************************************************************************
 *
 * Copyright Stefan Versluys, 2014
 *
 * This file is part of the xmas-design tool.
 *
 *  The xmas-design tool is free software: you can redistribute it
 *  and/or modify it under the terms of the GNU General Public License
 *  as published by the Free Software Foundation, either version 3 of
 *  the License, or (at your option) any later version.
 *
 *  The xmas-design tool is distributed in the hope that it will be
 *  useful,  but WITHOUT ANY WARRANTY; without even the implied warranty
 *  of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with the xmas-design tool.  If not, see
 *  <http://www.gnu.org/licenses/>.
 *
 *
 * Parts of this code were copied from Qt examples with following copyright
 * and license notices:
 *
 * Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies)
 *
 * under BSD license.
 *
 *
 **************************************************************************/
#include <QGraphicsSvgItem>
#include <QSvgRenderer>
#include <QtWidgets>

#include "modelwindow.h"
#include "complib.h"
#include "checker/xmas.h"
#include "positioncomponentextension.h"

QT_BEGIN_NAMESPACE
class QFile;
class QTextStream;
QT_END_NAMESPACE

class CompLib;

ModelWindow::ModelWindow(QWidget *parent)
    : QGraphicsView(parent)
{
    m_scene = new QGraphicsScene(this);
    m_scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    m_scene->setSceneRect(-1000, -1000, 1000, 1000);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    setScene(m_scene);
    setCacheMode(CacheBackground);
    setViewportUpdateMode(BoundingRectViewportUpdate);
    setRenderHint(QPainter::Antialiasing);
    setTransformationAnchor(AnchorUnderMouse);
    scale(qreal(1.0), qreal(1.0));
    setMinimumSize(250, 250);

    setAttribute(Qt::WA_DeleteOnClose);
    isUntitled = true;

    connect(&m_network, &Network::componentAdded, this, &ModelWindow::componentAdded);
}

ModelWindow::~ModelWindow()
{

}

void ModelWindow::newFile()
{
    static int sequenceNumber = 1;
    isUntitled = true;
    curFile = tr("model%1." MODEL_FILE_EXTENSION).arg(sequenceNumber++);
    setWindowTitle(curFile + "[*]");

//    connect(document(), SIGNAL(contentsChanged()),
//            this, SLOT(documentWasModified()));
}

bool ModelWindow::loadFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Test"),
                             tr("Cannot read file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return false;
    }

    QTextStream in(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    //setPlainText(in.readAll());
    QApplication::restoreOverrideCursor();

    setCurrentFile(fileName);

//    connect(document(), SIGNAL(contentsChanged()),
//            this, SLOT(documentWasModified()));

    return true;
}

bool ModelWindow::save()
{
    if (isUntitled) {
        return saveAs();
    } else {
        return saveFile(curFile);
    }
}

bool ModelWindow::saveAs()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Model As"),
                                                    curFile);
    if (fileName.isEmpty())
        return false;

    return saveFile(fileName);
}

bool ModelWindow::saveFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Save Model"),
                             tr("Cannot write file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return false;
    }

    QTextStream out(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    //out << toPlainText();
    QApplication::restoreOverrideCursor();

    setCurrentFile(fileName);
    return true;
}

QString ModelWindow::userFriendlyCurrentFile()
{
    return strippedName(curFile);
}

void ModelWindow::closeEvent(QCloseEvent *event)
{
    if (maybeSave()) {
        event->accept();
    } else {
        event->ignore();
    }
}

void ModelWindow::documentWasModified()
{
    //setWindowModified(document()->isModified());
}


#include <iostream> // std::cout
void ModelWindow::componentAdded(XMASComponent *component)
{
    std::cout << "ModelWindow: A component has been added: " << component->getName() << std::endl;

    auto pce = component->getComponentExtension<PositionComponentExtension>();

    CompLib *lib = new CompLib();
    ComponentType type = component->type();
    Component *c = lib->getComponent(type);
    if (c != nullptr)
    {
        c->setPos(pce->x(), pce->y());
        m_scene->addItem(c);
    }
}





bool ModelWindow::maybeSave()
{
//    if (document()->isModified()) {
    if (false)
    {
        QMessageBox::StandardButton ret;
        ret = QMessageBox::warning(this, tr("Test"),
                     tr("'%1' has been modified.\n"
                        "Do you want to save your changes?")
                     .arg(userFriendlyCurrentFile()),
                     QMessageBox::Save | QMessageBox::Discard
                     | QMessageBox::Cancel);
        if (ret == QMessageBox::Save)
            return save();
        else if (ret == QMessageBox::Cancel)
            return false;
    }
    return true;
}

void ModelWindow::setCurrentFile(const QString &fileName)
{
    curFile = QFileInfo(fileName).canonicalFilePath();
    isUntitled = false;
    //document()->setModified(false);
    setWindowModified(false);
    setWindowTitle(userFriendlyCurrentFile() + "[*]");
}

QString ModelWindow::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}
