#include <QtGui>
#include <osgDB/ReadFile>

#include <osgEarth/MapNode>

#include "mainwindow.h"

MainWindow::MainWindow(osgViewer::ViewerBase::ThreadingModel threadingModel, QString filename, QWidget *parent)
    : QMainWindow(parent)
{
    setupUi(this);
    _viewerWidget = new ViewerWidget(threadingModel, this);
    setCentralWidget(_viewerWidget);

    connect(actionTogglePolygonMode, SIGNAL(toggled(bool)),
            _viewerWidget, SLOT(togglePolygonMode(bool)));

    createDockWindows();

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
        osgEarth::MapNode* mapNode = osgEarth::MapNode::findMapNode(node);
         _viewerWidget->setMap(mapNode->getMap());
    }
}

void MainWindow::createDockWindows()
{
    QDockWidget *dock = new QDockWidget("Viewer Settings", this);
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

    _cameraWidget = new CameraWidget(dock);
    connect(_viewerWidget, SIGNAL(cameraChanged(osg::Matrixd)),
            _cameraWidget, SLOT(onCameraChanged(osg::Matrixd)));
    connect(_cameraWidget, SIGNAL(dataChanged(double, double, double)),
            _viewerWidget, SLOT(setCameraPosition(double, double, double)));

    dock->setWidget(_cameraWidget);
    addDockWidget(Qt::LeftDockWidgetArea, dock);
}

