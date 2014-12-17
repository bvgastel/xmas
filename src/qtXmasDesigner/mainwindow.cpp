#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "mdichild.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_newAct_triggered()
{
    MdiChild *child = createMdiChild();
    child->newFile();
    child->show();
}

void MainWindow::on_saveAsAct_triggered()
{
    // recent files test:
    // try programmatically adding menu items to a ui defined in the designer

    QAction *separator = ui->menu_File->insertSeparator(ui->exitAct);
    QAction *newAction = new QAction("Recent file", this);

    ui->menu_File->insertAction(separator, newAction);
}

MdiChild* MainWindow::createMdiChild()
{
    MdiChild *child = new MdiChild;
    ui->mdiArea->addSubWindow(child);

    return child;
}
