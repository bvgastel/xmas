/************************************************************************
 *
 * Copyright Stefan van der Sluys, 2014
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

#include "mainWindow.h"
#include "modelWindow.h"

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

    readSettings();
    createActions();
    createMenus();
    createToolBars();
    createStatusBar();
    updateMenus();


    setWindowTitle(tr(TOOL_NAME));
    setUnifiedTitleAndToolBarOnMac(true);

    createDockWindows();
    newFile();

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
    QString fileName = QFileDialog::getOpenFileName(this,tr("Open"),"",tr("xMAS Files (*.wck)"));

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

    QSettings settings("XmasDesigner", "xMAS Design Tool");
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
    QSettings settings("XmasDesigner", "xMAS Design Tool");

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
    if (activeModel())
        activeModel()->cut();
}

void MainWindow::copy()
{
    if (activeModel())
        activeModel()->copy();
}

void MainWindow::paste()
{
    if (activeModel())
        activeModel()->paste();
}
#endif

void MainWindow::about()
{
   QMessageBox::about(this, tr("About XmasDesigner"),
            tr("The XmasDesigner is a designing tool "
               "to create models in the XMAS language or "
               "eXecutable MicroArchitectural Specifications."));
}

void MainWindow::addPrimitive()
{
//dummy for now
}

void MainWindow::addComposite()
{
//dummy for now
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
    pasteAct->setEnabled(hasModel);
#endif

#ifndef QT_NO_CLIPBOARD
    bool hasSelection = (activeModel() &&
                         activeModel()->textCursor().hasSelection());
    cutAct->setEnabled(hasSelection);
    copyAct->setEnabled(hasSelection);
#endif
}

ModelWindow *MainWindow::createModel()
{
    ModelWindow *model = new ModelWindow;
    mdiArea->addSubWindow(model);

#ifndef QT_NO_CLIPBOARD
    connect(model, SIGNAL(copyAvailable(bool)),
            cutAct, SLOT(setEnabled(bool)));
    connect(model, SIGNAL(copyAvailable(bool)),
            copyAct, SLOT(setEnabled(bool)));
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


    queueAct = new QAction(QIcon(":/images/Queue.png"), tr("Queue"), this);
    queueAct->setStatusTip(tr("Draws the ''queue'' primitive"));
    connect(queueAct, SIGNAL(triggered()), this, SLOT(addPrimitive()));

    functionAct = new QAction(QIcon(":/images/Function.png"), tr("Function"), this);
    functionAct->setStatusTip(tr("Draws the ''function'' primitive"));
    connect(functionAct, SIGNAL(triggered()), this, SLOT(addPrimitive()));

    forkAct = new QAction(QIcon(":/images/Fork.png"), tr("Fork"), this);
    forkAct->setStatusTip(tr("Draws the ''fork'' primitive"));
    connect(forkAct, SIGNAL(triggered()), this, SLOT(addPrimitive()));

    joinAct = new QAction(QIcon(":/images/Join.png"), tr("Join"), this);
    joinAct->setStatusTip(tr("Draws the ''join'' primitive"));
    connect(joinAct, SIGNAL(triggered()), this, SLOT(addPrimitive()));

    switchAct = new QAction(QIcon(":/images/Switch.png"), tr("Switch"), this);
    switchAct->setStatusTip(tr("Draws the ''switch'' primitive"));
    connect(switchAct, SIGNAL(triggered()), this, SLOT(addPrimitive()));

    mergeAct = new QAction(QIcon(":/images/Merge.png"), tr("Merge"), this);
    mergeAct->setStatusTip(tr("Draws the ''merge'' primitive"));
    connect(mergeAct, SIGNAL(triggered()), this, SLOT(addPrimitive()));

    sinkAct = new QAction(QIcon(":/images/Sink.png"), tr("Sink"), this);
    sinkAct->setStatusTip(tr("Draws the ''sink'' primitive"));
    connect(sinkAct, SIGNAL(triggered()), this, SLOT(addPrimitive()));

    sourceAct = new QAction(QIcon(":/images/Source.png"), tr("Source"), this);
    sourceAct->setStatusTip(tr("Draws the ''source'' primitive"));
    connect(sourceAct, SIGNAL(triggered()), this, SLOT(addPrimitive()));

    inputAct = new QAction(QIcon(":/images/In.png"), tr("Input"), this);
    inputAct->setStatusTip(tr("Draws an ''Input connection point''"));
    connect(inputAct, SIGNAL(triggered()), this, SLOT(addPrimitive()));

    outputAct = new QAction(QIcon(":/images/Out.png"), tr("Output"), this);
    outputAct->setStatusTip(tr("Draws an ''Output connection point''"));
    connect(outputAct, SIGNAL(triggered()), this, SLOT(addPrimitive()));

    compositeAct = new QAction(QIcon(":/images/Composite.png"), tr("Composite"), this);
    compositeAct->setStatusTip(tr("Draws a ''Composite''"));
    connect(compositeAct, SIGNAL(triggered()), this, SLOT(addComposite()));

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
    QSettings settings("XmasDesigner", "xMAS Design Tool");
    settings.beginGroup("MainWindow");
    move(settings.value("pos", QPoint(200, 200)).toPoint());
    resize(settings.value("size", QSize(400, 400)).toSize());
    settings.endGroup();
}

void MainWindow::writeSettings()
{
    QSettings settings("XmasDesigner", "xMAS Design Tool");
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
