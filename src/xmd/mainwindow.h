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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "complib.h"
#include "setup.h"

#include <QMainWindow>
#include <QSignalMapper>

QT_BEGIN_NAMESPACE
class QAction;
class QToolBox;
class QComboBox;
class QToolButton;
class QAbstractButton;
class QMdiArea;
class QMdiSubWindow;
class QListWidget;
class QTreeWidget;
class QSignalMapper;
QT_END_NAMESPACE

class ModelWindow;


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();
    bool openFile(const QString &fileName);

protected:
    void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;

private slots:
    void newFile();
    void open();
    void openRecentFile();
    void save();
    void saveAs();
#ifndef QT_NO_CLIPBOARD
    void cut();
    void copy();
    void paste();
#endif
    void addComponent(int type);
    void setPackets();
    void about();
    void updateMenus();
    ModelWindow *createModel();
    void setModelWindow(QWidget *window);

private:
    void createActions();
    void createMenus();
    void createToolBars();
    void createStatusBar();
    void readSettings();
    void writeSettings();
    void createDockWindows();
    void setCurrentFile(const QString &fileName);
    void updateRecentFileActions();
    QString strippedName(const QString &fullFileName);
    QString curFile;
    ModelWindow *activeModel();
    QMdiSubWindow *findModel(const QString &fileName);
    QMdiArea *mdiArea;
    QSignalMapper *windowMapper;
    QSignalMapper *signalMapper;
    QListWidget *outputList;
    QTreeWidget *projectTree;
    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *viewMenu;
    QMenu *helpMenu;
    QToolBar *fileToolBar;
    QToolBar *editToolBar;
    QToolBar *shapeToolBar;
    //QToolBar *packetToolBar;
    QAction *newAct;
    QAction *openAct;
    QAction *saveAct;
    QAction *saveAsAct;
    QAction *closeAct;
    QAction *closeAllAct;
    QAction *exitAct;
#ifndef QT_NO_CLIPBOARD
    QAction *cutAct;
    QAction *copyAct;
    QAction *pasteAct;
#endif
    QAction *queueAct;
    QAction *functionAct;
    QAction *forkAct;
    QAction *joinAct;
    QAction *switchAct;
    QAction *mergeAct;
    QAction *sinkAct;
    QAction *sourceAct;
    QAction *compositeAct;
    QAction *inputAct;
    QAction *outputAct;
    QAction *packetAct;
    QAction *aboutAct;
    QAction *recentFileActs[MAXIMUM_RECENT_FILES];
    QAction *recentSeparatorAct;
};

#endif
