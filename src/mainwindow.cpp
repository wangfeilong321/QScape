#include <QtGui>
#include "mainwindow.h"

MainWindow::MainWindow(osgViewer::ViewerBase::ThreadingModel threadingModel,
    QWidget *parent)
        : QMainWindow(parent)
{
    setupUi(this);
    _viewerWidget = new ViewerWidget(threadingModel, this);
    setCentralWidget(_viewerWidget);
}

void MainWindow::on_actionQuit_triggered()
{
    qApp->quit();
}

