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


#include "modelwindow.h"
#include "complib.h"
#include <QDebug>

QT_BEGIN_NAMESPACE
class QFile;
class QTextStream;
QT_END_NAMESPACE

class CompLib;

ModelWindow::ModelWindow(QWidget * parent)
    : QQuickWidget(parent)
{

    setResizeMode(QQuickWidget::SizeRootObjectToView);
    setSource(QUrl("../xmd/qml/scene.qml"));
    //setAttribute(Qt::WA_DeleteOnClose);
    isUntitled = true;

}

ModelWindow::~ModelWindow()
{

}

void ModelWindow::addComponent(int type)
{
    CompLib *lib = new CompLib();
    Component *c = lib->createComponent(type);
    if (c != nullptr)
    {
        c->setX(200);
        c->setY(200);
        c->setParentItem(this->rootObject());
    }
}

void ModelWindow::newFile()
{
    static int sequenceNumber = 1;
    isUntitled = true;
    curFile = tr("model%1." MODEL_FILE_EXTENSION).arg(sequenceNumber++);
    //setWindowTitle(curFile + "[*]");

}

bool ModelWindow::loadFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
//        QMessageBox::warning(this, tr("Test"),
//                             tr("Cannot read file %1:\n%2.")
//                             .arg(fileName)
//                             .arg(file.errorString()));
        return false;
    }


    QApplication::setOverrideCursor(Qt::WaitCursor);

    QApplication::restoreOverrideCursor();

    setCurrentFile(fileName);



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
    QString fileName; //= QFileDialog::getSaveFileName(this, tr("Save Model As"),
                     //                               curFile);
    if (fileName.isEmpty())
        return false;

    return saveFile(fileName);
}

bool ModelWindow::saveFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
//        QMessageBox::warning(this, tr("Save Model"),
//                             tr("Cannot write file %1:\n%2.")
//                             .arg(fileName)
//                             .arg(file.errorString()));
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

//void ModelWindow::closeEvent(QCloseEvent *event)
//{
//    if (maybeSave()) {
//        event->accept();
//    } else {
//        event->ignore();
//    }
//}

void ModelWindow::wheelEvent(QWheelEvent* event) {
    Q_UNUSED(event)

//    setTransformationAnchor(QQuickView::AnchorUnderMouse);
//    if (event->modifiers().testFlag(Qt::ControlModifier)){
//        // Scale the view / do the zoom
//        double scaleFactor = 1.15;
//        if(event->delta() > 0) {
//            // Zoom in
//            scale(scaleFactor, scaleFactor);

//        } else {
//            // Zooming out
//            scale(1.0 / scaleFactor, 1.0 / scaleFactor);
//        }
//    }
}

void ModelWindow::documentWasModified()
{
    //setWindowModified(document()->isModified());
}

bool ModelWindow::maybeSave()
{

    return true;
}

void ModelWindow::setCurrentFile(const QString &fileName)
{
    curFile = QFileInfo(fileName).canonicalFilePath();
    isUntitled = false;
    //document()->setModified(false);
    //setWindowModified(false);
    //setWindowTitle(userFriendlyCurrentFile() + "[*]");
}

QString ModelWindow::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}
