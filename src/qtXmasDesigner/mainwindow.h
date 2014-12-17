#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class MdiChild;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_newAct_triggered();

    void on_saveAsAct_triggered();

private:
    Ui::MainWindow *ui;

    MdiChild* createMdiChild();
};

#endif // MAINWINDOW_H
