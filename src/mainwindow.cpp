#include <QtGui>
#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupUi(this);
}

void MainWindow::on_actionQuit_triggered()
{
    qApp->quit();
}

