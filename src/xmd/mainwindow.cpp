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
#include <QtWidgets>
#include <QDebug>

#include "mainwindow.h"
#include "modelwindow.h"
#include "designercontroller.h"


MainWindow::MainWindow()
{

    //setup docking area's
    setCorner(Qt::TopLeftCorner, Qt::LeftDockWidgetArea);
    setCorner(Qt::BottomLeftCorner, Qt::LeftDockWidgetArea);
    setCorner(Qt::TopRightCorner, Qt::RightDockWidgetArea);
    setCorner(Qt::BottomRightCorner, Qt::RightDockWidgetArea);

    mdiArea = new QMdiArea;
    mdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    mdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    mdiArea->setViewMode(QMdiArea::TabbedView);
    mdiArea->setTabPosition(QTabWidget::South);
    mdiArea->setTabsClosable(true);

    setCentralWidget(mdiArea);
    setWindowTitle(tr(TOOL_NAME));
    setUnifiedTitleAndToolBarOnMac(true);

    signalMapper = new QSignalMapper(this);

    readSettings();
    createActions();
    createMenus();
    createToolBars();
    createStatusBar();
    updateMenus();  
    createDockWindows();

    //emit newFile();

}

void MainWindow::closeEvent(QCloseEvent *event)
{
    mdiArea->closeAllSubWindows();
    if (mdiArea->currentSubWindow()) {
        event->ignore();
    } else {
        writeSettings();
        event->accept();
    }
}

void MainWindow::newFile()
{
    ModelWindow *child = createModel();
    child->newFile();
    child->showMaximized();
    updateMenus();
}

void MainWindow::open()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("Open"),"",tr("xMAS Model Files (*.xmdm)"));

    if (!fileName.isEmpty()) {
        QMdiSubWindow *existing = findModel(fileName);
        if (existing) {
            mdiArea->setActiveSubWindow(existing);
            return;
        }

        if (openFile(fileName))
            statusBar()->showMessage(tr("File opened"), 2000);
    }
    updateMenus();
}

bool MainWindow::openFile(const QString &fileName)
{
    ModelWindow *child = createModel();
    const bool succeeded = child->loadFile(fileName);
    if (succeeded){
        setCurrentFile(fileName);
        child->show();
    }
    else
        child->close();
    updateMenus();
    return succeeded;
}

void MainWindow::openRecentFile()
{
    QAction *action = qobject_cast<QAction *>(sender());
    if (action)
        openFile(action->data().toString());
}

void MainWindow::setCurrentFile(const QString &fileName)
{
    curFile = fileName;
    setWindowFilePath(curFile);

    QSettings settings("XMD", "xMAS Model Design Tool");
    QStringList files = settings.value("recentFileList").toStringList();
    files.removeAll(fileName);
    files.prepend(fileName);
    while (files.size() > MAXIMUM_RECENT_FILES)
        files.removeLast();

    settings.setValue("recentFileList", files);

    foreach (QWidget *widget, QApplication::topLevelWidgets()) {
        MainWindow *mainWin = qobject_cast<MainWindow *>(widget);
        if (mainWin)
            mainWin->updateRecentFileActions();
    }
}

void MainWindow::updateRecentFileActions()
{
    QSettings settings("XMD", "xMAS Model Design Tool");

    QStringList files = settings.value("recentFileList").toStringList();

    int numRecentFiles = qMin(files.size(), (int)MAXIMUM_RECENT_FILES);

    for (int i = 0; i < numRecentFiles; ++i) {
        QString text = tr("&%1 %2").arg(i + 1).arg(strippedName(files[i]));
        recentFileActs[i]->setText(text);
        recentFileActs[i]->setData(files[i]);
        recentFileActs[i]->setVisible(true);
    }
    for (int j = numRecentFiles; j < MAXIMUM_RECENT_FILES; ++j)
        recentFileActs[j]->setVisible(false);

    recentSeparatorAct->setVisible(numRecentFiles > 0);
}

QString MainWindow::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}

void MainWindow::save()
{
    if (activeModel() && activeModel()->save())
        statusBar()->showMessage(tr("Model saved"), 2000);
}

void MainWindow::saveAs()
{
    if (activeModel() && activeModel()->saveAs())
        statusBar()->showMessage(tr("Model saved"), 2000);
}

#ifndef QT_NO_CLIPBOARD
void MainWindow::cut()
{
//    if (activeModel())
//        activeModel()->cut();
}

void MainWindow::copy()
{
//    if (activeModel())
//        activeModel()->copy();
}

void MainWindow::paste()
{
//    if (activeModel())
//        activeModel()->paste();
}
#endif

void MainWindow::about()
{
   QMessageBox::about(this, tr("About XmasDesigner"),
            tr("The XmasDesigner is a designing tool "
               "to create models in the XMAS language or "
               "eXecutable MicroArchitectural Specifications."));
}

/**
 * @brief MainWindow::addComponent
 * @param type
 */
void MainWindow::addComponent(int type)
{
    ModelWindow *child = activeModel();
    if (child) {
        qDebug() << "type = " << type;

        Network& network = child->network();
        DesignerController dc {network};

        static int counter = 1;
        std::string counter_str = std::to_string(counter++);

        switch (type) {
            case ComponentType::Queue:
                dc.addComponent<XMASQueue>("Queue" + counter_str, -300, -300, Orientation::North);
                break;
            case ComponentType::Function:
                dc.addComponent<XMASFunction>("Function" + counter_str, -300, -300, Orientation::North);
                break;
            case ComponentType::Fork:
                dc.addComponent<XMASFork>("Fork" + counter_str, -300, -300, Orientation::North);
                break;
            case ComponentType::Join:
                dc.addComponent<XMASJoin>("Join" + counter_str, -300, -300, Orientation::North);
                break;
            case ComponentType::Switch:
                dc.addComponent<XMASSwitch>("Switch" + counter_str, -300, -300, Orientation::North);
                break;
            case ComponentType::Merge:
                dc.addComponent<XMASMerge>("Merge" + counter_str, -300, -300, Orientation::North);
                break;
            case ComponentType::Sink:
                dc.addComponent<XMASSink>("Sink" + counter_str, -300, -300, Orientation::North);
                break;
            case ComponentType::Source:
                dc.addComponent<XMASSource>("Source" + counter_str, -300, -300, Orientation::North);
                break;
        }
    }

}

void MainWindow::setPackets()
{
//dummy for now
}

void MainWindow::updateMenus()
{
    bool hasModel = (activeModel() != 0);
    saveAct->setEnabled(hasModel);
    saveAsAct->setEnabled(hasModel);
    closeAct->setEnabled(hasModel);
    closeAllAct->setEnabled(hasModel);

#ifndef QT_NO_CLIPBOARD
    //pasteAct->setEnabled(hasModel);
#endif

#ifndef QT_NO_CLIPBOARD
    //bool hasSelection = (activeModel() &&
    //                     activeModel()->textCursor().hasSelection());
    //cutAct->setEnabled(hasSelection);
    //copyAct->setEnabled(hasSelection);
#endif
}

ModelWindow *MainWindow::createModel()
{
    ModelWindow *model = new ModelWindow;
    mdiArea->addSubWindow(model);
    setModelWindow(model->window());

#ifndef QT_NO_CLIPBOARD
//    connect(model, SIGNAL(copyAvailable(bool)),
//            cutAct, SLOT(setEnabled(bool)));
//    connect(model, SIGNAL(copyAvailable(bool)),
//            copyAct, SLOT(setEnabled(bool)));
#endif

    return model;
}

void MainWindow::createActions()
{
    newAct = new QAction(QIcon(":/images/new.png"), tr("&New"), this);
    newAct->setShortcuts(QKeySequence::New);
    newAct->setStatusTip(tr("Create a new model."));
    connect(newAct, SIGNAL(triggered()), this, SLOT(newFile()));

    openAct = new QAction(QIcon(":/images/open.png"), tr("&Open..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open an existing model."));
    connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

    for (int i = 0; i < MAXIMUM_RECENT_FILES; ++i) {
        recentFileActs[i] = new QAction(this);
        recentFileActs[i]->setVisible(false);
        connect(recentFileActs[i], SIGNAL(triggered()),
                this, SLOT(openRecentFile()));
    }

    saveAct = new QAction(QIcon(":/images/save.png"), tr("&Save"), this);
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip(tr("Save the model."));
    connect(saveAct, SIGNAL(triggered()), this, SLOT(save()));

    saveAsAct = new QAction(tr("Save &As..."), this);
    saveAsAct->setShortcuts(QKeySequence::SaveAs);
    saveAsAct->setStatusTip(tr("Save the model as..."));
    connect(saveAsAct, SIGNAL(triggered()), this, SLOT(saveAs()));

    closeAct = new QAction(tr("Cl&ose"), this);
    closeAct->setStatusTip(tr("Close the active model."));
    connect(closeAct, SIGNAL(triggered()),
            mdiArea, SLOT(closeActiveSubWindow()));

    closeAllAct = new QAction(tr("Close &All"), this);
    closeAllAct->setStatusTip(tr("Close all models."));
    connect(closeAllAct, SIGNAL(triggered()),
            mdiArea, SLOT(closeAllSubWindows()));

//! [0]
    exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcuts(QKeySequence::Quit);
    exitAct->setStatusTip(tr("Exit the Xmas Designer application."));
    connect(exitAct, SIGNAL(triggered()), qApp, SLOT(closeAllWindows()));
//! [0]

#ifndef QT_NO_CLIPBOARD
    cutAct = new QAction(QIcon(":/images/cut.png"), tr("Cu&t"), this);
    cutAct->setShortcuts(QKeySequence::Cut);
    cutAct->setStatusTip(tr("Cut the current selection's contents to the "
                            "clipboard"));
    connect(cutAct, SIGNAL(triggered()), this, SLOT(cut()));

    copyAct = new QAction(QIcon(":/images/copy.png"), tr("&Copy"), this);
    copyAct->setShortcuts(QKeySequence::Copy);
    copyAct->setStatusTip(tr("Copy the current selection's contents to the "
                             "clipboard"));
    connect(copyAct, SIGNAL(triggered()), this, SLOT(copy()));

    pasteAct = new QAction(QIcon(":/images/paste.png"), tr("&Paste"), this);
    pasteAct->setShortcuts(QKeySequence::Paste);
    pasteAct->setStatusTip(tr("Paste the clipboard's contents into the current "
                              "selection"));
    connect(pasteAct, SIGNAL(triggered()), this, SLOT(paste()));
#endif

    aboutAct = new QAction(tr("&About"), this);
    aboutAct->setStatusTip(tr("Show the application's About box"));
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));


    queueAct = new QAction(QIcon(":/images/queue.png"), tr("Queue"), this);
    queueAct->setStatusTip(tr("Draws a ''queue'' primitive"));
    signalMapper->setMapping(queueAct, CompLib::Queue);
    connect(queueAct, SIGNAL(triggered()), signalMapper, SLOT(map()));

    functionAct = new QAction(QIcon(":/images/function.png"), tr("Function"), this);
    functionAct->setStatusTip(tr("Draws a ''function'' primitive"));
    signalMapper->setMapping(functionAct, CompLib::Function);
    connect(functionAct, SIGNAL(triggered()), signalMapper, SLOT(map()));

    forkAct = new QAction(QIcon(":/images/fork.png"), tr("Fork"), this);
    forkAct->setStatusTip(tr("Draws a ''fork'' primitive"));
    signalMapper->setMapping(forkAct, CompLib::Fork);
    connect(forkAct, SIGNAL(triggered()), signalMapper, SLOT(map()));

    joinAct = new QAction(QIcon(":/images/join.png"), tr("Join"), this);
    joinAct->setStatusTip(tr("Draws a ''join'' primitive"));
    signalMapper->setMapping(joinAct, CompLib::Join);
    connect(joinAct, SIGNAL(triggered()), signalMapper, SLOT(map()));

    switchAct = new QAction(QIcon(":/images/switch.png"), tr("Switch"), this);
    switchAct->setStatusTip(tr("Draws a ''switch'' primitive"));
    signalMapper->setMapping(switchAct, CompLib::Switch);
    connect(switchAct, SIGNAL(triggered()), signalMapper, SLOT(map()));

    mergeAct = new QAction(QIcon(":/images/merge.png"), tr("Merge"), this);
    mergeAct->setStatusTip(tr("Draws a ''merge'' primitive"));
    signalMapper->setMapping(mergeAct, CompLib::Merge);
    connect(mergeAct, SIGNAL(triggered()), signalMapper, SLOT(map()));

    sinkAct = new QAction(QIcon(":/images/sink.png"), tr("Sink"), this);
    sinkAct->setStatusTip(tr("Draws a ''sink'' primitive"));
    signalMapper->setMapping(sinkAct, CompLib::Sink);
    connect(sinkAct, SIGNAL(triggered()), signalMapper, SLOT(map()));

    sourceAct = new QAction(QIcon(":/images/source.png"), tr("Source"), this);
    sourceAct->setStatusTip(tr("Draws the ''source'' primitive"));
    signalMapper->setMapping(sourceAct, CompLib::Source);
    connect(sourceAct, SIGNAL(triggered()), signalMapper, SLOT(map()));

    inputAct = new QAction(QIcon(":/images/in.png"), tr("Input"), this);
    inputAct->setStatusTip(tr("Draws an ''Input connection point''"));
    signalMapper->setMapping(inputAct, CompLib::In);
    connect(inputAct, SIGNAL(triggered()), signalMapper, SLOT(map()));

    outputAct = new QAction(QIcon(":/images/out.png"), tr("Output"), this);
    outputAct->setStatusTip(tr("Draws an ''Output connection point''"));
    signalMapper->setMapping(outputAct, CompLib::Out);
    connect(outputAct, SIGNAL(triggered()), signalMapper, SLOT(map()));

    compositeAct = new QAction(QIcon(":/images/composite.png"), tr("Composite"), this);
    compositeAct->setStatusTip(tr("Draws a ''Composite''"));
    signalMapper->setMapping(compositeAct, CompLib::Composite);
    connect(compositeAct, SIGNAL(triggered()), signalMapper, SLOT(map()));

    connect(signalMapper, SIGNAL(mapped(int)),
            this, SLOT(addComponent(int)));


    packetAct = new QAction(QIcon(":/images/packet.png"), tr("Packet"), this);
    packetAct->setStatusTip(tr("Setup the packets."));
    connect(packetAct, SIGNAL(triggered()), this, SLOT(setPackets()));

}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newAct);
    fileMenu->addAction(openAct);

    fileMenu->addAction(saveAct);
    fileMenu->addAction(saveAsAct);
    fileMenu->addAction(closeAct);
    fileMenu->addAction(closeAllAct);
    recentSeparatorAct = fileMenu->addSeparator();
    for (int i = 0; i < MAXIMUM_RECENT_FILES; ++i)
        fileMenu->addAction(recentFileActs[i]);

    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);
    updateRecentFileActions();

    editMenu = menuBar()->addMenu(tr("&Edit"));
#ifndef QT_NO_CLIPBOARD
    editMenu->addAction(cutAct);
    editMenu->addAction(copyAct);
    editMenu->addAction(pasteAct);
#endif

    viewMenu = menuBar()->addMenu(tr("&View"));

    menuBar()->addSeparator();

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAct);
}

void MainWindow::createToolBars()
{
    fileToolBar = addToolBar(tr("File"));
    fileToolBar->addAction(newAct);
    fileToolBar->addAction(openAct);
    fileToolBar->addAction(saveAct);
    fileToolBar->setMovable(false);

#ifndef QT_NO_CLIPBOARD
    editToolBar = addToolBar(tr("Edit"));
    editToolBar->addAction(cutAct);
    editToolBar->addAction(copyAct);
    editToolBar->addAction(pasteAct);
    editToolBar->setMovable(false);
#endif

    shapeToolBar = addToolBar(tr("Shape"));
    shapeToolBar->addAction(queueAct);
    shapeToolBar->addAction(functionAct);
    shapeToolBar->addSeparator();
    shapeToolBar->addAction(forkAct);
    shapeToolBar->addAction(joinAct);
    shapeToolBar->addSeparator();
    shapeToolBar->addAction(switchAct);
    shapeToolBar->addAction(mergeAct);
    shapeToolBar->addSeparator();
    shapeToolBar->addAction(sinkAct);
    shapeToolBar->addAction(sourceAct);
    shapeToolBar->addSeparator();
    shapeToolBar->addAction(inputAct);
    shapeToolBar->addAction(outputAct);
    shapeToolBar->addSeparator();
    shapeToolBar->addAction(compositeAct);
    shapeToolBar->addSeparator();
    shapeToolBar->addAction(packetAct);
    shapeToolBar->setMovable(false);

}

void MainWindow::createStatusBar()
{
    statusBar()->showMessage(tr("Ready"));
}

void MainWindow::readSettings()
{
    QSettings settings("XMD", "xMAS Model Design Tool");
    settings.beginGroup("MainWindow");
    move(settings.value("pos", QPoint(200, 200)).toPoint());
    resize(settings.value("size", QSize(400, 400)).toSize());
    settings.endGroup();
}

void MainWindow::writeSettings()
{
    QSettings settings("XMD", "xMAS Model Design Tool");
    settings.beginGroup("MainWindow");
    settings.setValue("pos", pos());
    settings.setValue("size", size());
    settings.endGroup();
}

ModelWindow *MainWindow::activeModel()
{
    if (QMdiSubWindow *model = mdiArea->activeSubWindow())
        return qobject_cast<ModelWindow *>(model->widget());
    return 0;
}

QMdiSubWindow *MainWindow::findModel(const QString &fileName)
{
    QString canonicalFilePath = QFileInfo(fileName).canonicalFilePath();

    foreach (QMdiSubWindow *model, mdiArea->subWindowList()) {
        ModelWindow *thismodel = qobject_cast<ModelWindow *>(model->widget());
        if (thismodel->currentFile() == canonicalFilePath)
            return model;
    }
    return 0;
}

void MainWindow::setModelWindow(QWidget *window)
{
    if (!window)
        return;
    mdiArea->setActiveSubWindow(qobject_cast<QMdiSubWindow *>(window));
}

void MainWindow::createDockWindows()
{
    //ToDo --> project and view need to be seperate
    // QWindow classes. At this moment the code
    // below is ment as a demo.

    //project window
    QDockWidget *dock = new QDockWidget(tr("Project Manager"), this);
    //dock->setAllowedAreas(Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea);
    dock->setFeatures(QDockWidget::DockWidgetClosable);

    projectTree = new QTreeWidget();
    projectTree->setColumnCount(1);
    projectTree->setHeaderHidden(true);
    QTreeWidgetItem *root = new QTreeWidgetItem();
    root->setText(0,"projectX");
    projectTree->insertTopLevelItem(0, root);

    QList<QTreeWidgetItem *> items;
     for (int i = 0; i < 10; ++i)
         items.append(new QTreeWidgetItem((QTreeWidget*)0, QStringList(QString("model%1").arg(i))));
    root->insertChildren(0,items);

    dock->setWidget(projectTree);
    addDockWidget(Qt::LeftDockWidgetArea, dock);
    viewMenu->addAction(dock->toggleViewAction());


    //output window
    dock = new QDockWidget(tr("Output"), this);
    dock->setFeatures(QDockWidget::DockWidgetClosable);
    outputList = new QListWidget();
    outputList->addItems(QStringList()
            << "...ready!");
    dock->setWidget(outputList);
    addDockWidget(Qt::BottomDockWidgetArea, dock);
    viewMenu->addAction(dock->toggleViewAction());

}
