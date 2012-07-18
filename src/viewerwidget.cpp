#include <QtGui/QGridLayout>
#include <QtGui/QWidget>

#include <osgGA/TrackballManipulator>
#include <osgDB/ReadFile>
#include <osgViewer/ViewerEventHandlers>
#include <osgQt/GraphicsWindowQt>

#include "viewerwidget.h"

ViewerWidget::ViewerWidget(osgViewer::ViewerBase::ThreadingModel threadingModel, QWidget *parent)
    : QWidget(parent)
{
    setThreadingModel(threadingModel);

    QWidget* widget = createWidget(createCamera(0,0,100,100), osgDB::readNodeFile("cow.osgt"));
    QGridLayout* grid = new QGridLayout;
    grid->addWidget(widget, 0, 0);
    setLayout(grid);

    connect(&_timer, SIGNAL(timeout()), this, SLOT(update()));
    _timer.start(10);
}

QWidget* ViewerWidget::createWidget(osg::Camera* camera, osg::Node* scene)
{
    setCamera(camera);

    setSceneData(scene);
    addEventHandler( new osgViewer::StatsHandler );
    setCameraManipulator( new osgGA::TrackballManipulator() );
    osgQt::GraphicsWindowQt* gw = dynamic_cast<osgQt::GraphicsWindowQt*>( camera->getGraphicsContext() );

    return gw->getGLWidget();
}

osg::Camera* ViewerWidget::createCamera(int x, int y, int w, int h, QString name)
{
    osg::DisplaySettings* ds = osg::DisplaySettings::instance().get();
    osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits;
    //traits->windowName = "";
    traits->windowDecoration = false;
    traits->x = 0;
    traits->y = 0;
    traits->width = width();
    traits->height = height();
    traits->doubleBuffer = true;
    traits->alpha = ds->getMinimumNumAlphaBits();
    traits->stencil = ds->getMinimumNumStencilBits();
    traits->sampleBuffers = ds->getMultiSamples();
    traits->samples = ds->getNumMultiSamples();

    osg::ref_ptr<osg::Camera> camera = new osg::Camera;

    camera->setGraphicsContext( new osgQt::GraphicsWindowQt(traits.get()) );
    camera->setClearColor( osg::Vec4(0.18, 0.18, 0.18, 1.0) );
    camera->setViewport( new osg::Viewport(0, 0, traits->width, traits->height) );
    camera->setProjectionMatrixAsPerspective(
        30.0f, static_cast<double>(traits->width)/static_cast<double>(traits->height), 1.0f, 10000.0f );

    return camera.release();
}

