#include <QtGui>
#include <osgDB/ReadFile>
#include "mainwindow.h"

MainWindow::MainWindow(osgViewer::ViewerBase::ThreadingModel threadingModel, QString filename, QWidget *parent)
    : QMainWindow(parent)
{
    setupUi(this);
    _viewerWidget = new ViewerWidget(threadingModel, this);
    setCentralWidget(_viewerWidget);

    connect(actionTogglePolygonMode, SIGNAL(toggled(bool)),
            _viewerWidget, SLOT(togglePolygonMode(bool)));

    _loadFile(filename);
}

void MainWindow::on_actionQuit_triggered()
{
    qApp->quit();
}

void MainWindow::on_actionOpen_triggered()
{
    _viewerWidget->stop();
    QString filename = QFileDialog::getOpenFileName(this, "Select Scene File");
    if(filename != NULL) {
        _loadFile(filename);
    }
    _viewerWidget->start();
}

void MainWindow::_loadFile(QString filename)
{
    osg::ref_ptr<osg::Node> node = osgDB::readNodeFile(filename.toAscii().data());
    if(node != NULL) {
         _viewerWidget->setMap(node.get());
    }
}

