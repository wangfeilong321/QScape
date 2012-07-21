#include <QtGui>
#include <osgDB/ReadFile>
#include "mainwindow.h"

MainWindow::MainWindow(osgViewer::ViewerBase::ThreadingModel threadingModel,
    QWidget *parent)
        : QMainWindow(parent)
{
    setupUi(this);
    _viewerWidget = new ViewerWidget(threadingModel, this);
    setCentralWidget(_viewerWidget);

    connect(actionTogglePolygonMode, SIGNAL(toggled(bool)),
            _viewerWidget, SLOT(togglePolygonMode(bool)));
}

void MainWindow::on_actionQuit_triggered()
{
    qApp->quit();
}

void MainWindow::on_actionOpen_triggered()
{
    _viewerWidget->stop();
    QString fileName = QFileDialog::getOpenFileName(this, "Select Scene File");
    if(fileName != NULL) {
        osg::ref_ptr<osg::Node> node = osgDB::readNodeFile(fileName.toAscii().data());
        if(node != NULL) {
            _viewerWidget->setMap(node.get());
        }
    }
    _viewerWidget->start();
}

