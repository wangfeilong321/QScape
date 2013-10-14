#include <QtGui/QGridLayout>
#include <QtGui/QWidget>
#include <QColor>
#include <QDebug>

#include <osgGA/TrackballManipulator>
#include <osgViewer/ViewerEventHandlers>
#include <osgQt/GraphicsWindowQt>
#include <osgUtil/Optimizer>

#include <osgEarth/MapNode>

#include "viewerwidget.h"

ViewerWidget::ViewerWidget(osgViewer::ViewerBase::ThreadingModel threadingModel, QWidget *parent)
    : QWidget(parent)
{
    setThreadingModel(threadingModel);

    // Create osgQt widget with new camera
    QWidget* widget = createWidget(createCamera(0,0,100,100), NULL);
    QGridLayout* grid = new QGridLayout;
    grid->addWidget(widget, 0, 0);
    setLayout(grid);

    // Set up render loop
    connect(&_timer, SIGNAL(timeout()), this, SLOT(update()));
    _timerInterval = 10;
    start();
}

/**
 * Create osgQt widget
 */
QWidget* ViewerWidget::createWidget(osg::Camera* camera, osg::Node* scene)
{
    setCamera(camera);
    _cameraManipulator = new osgGA::TrackballManipulator();
    setCameraManipulator(_cameraManipulator);

    setSceneData(scene);

    addEventHandler(new osgViewer::StatsHandler);

    osgQt::GraphicsWindowQt* gw = dynamic_cast<osgQt::GraphicsWindowQt*>(camera->getGraphicsContext());

    return gw->getGLWidget();
}

/**
 * Create new camera
 */
osg::Camera* ViewerWidget::createCamera(int x, int y, int w, int h, QString name)
{
    osg::DisplaySettings* ds = osg::DisplaySettings::instance().get();
    ds->setNumMultiSamples(4);
    osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits;
    traits->windowName = name.toAscii().data();
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
    QColor color = palette().color(backgroundRole());
    camera->setClearColor( osg::Vec4(color.red() / 255.0, color.green() / 255.0, color.blue() / 255.0, 1.0) );
    camera->setViewport( new osg::Viewport(0, 0, traits->width, traits->height) );
    camera->setProjectionMatrixAsPerspective(
        30.0f, static_cast<double>(traits->width)/static_cast<double>(traits->height), 1.0f, 10000.0f );

    return camera.release();
}

/**
 * Stop render loop
 */
void ViewerWidget::stop()
{
    _timer.stop();
}

/**
 * Start render loop
 */
void ViewerWidget::start()
{
    _timer.start(_timerInterval);
}

/**
 * Set new map to widget
 */
int ViewerWidget::setMap(osgEarth::Map *map)
{
    osg::ref_ptr<osg::Group> root = new osg::Group;
    osg::ref_ptr<osgEarth::MapNode> mapNode = new osgEarth::MapNode(map);
    root->addChild(mapNode);

    osg::BoundingSphere bounds = mapNode->getBound();

    double cx = bounds.center().x();
    double cy = bounds.center().y() - bounds.radius();
    double cz = bounds.center().z();
    const osgEarth::SpatialReference *srs = map->getProfile()->getSRS();
    const osgEarth::SpatialReference *epsg4326 = osgEarth::SpatialReference::create("EPSG:4326");
    double gx, gy;
    srs->transform(bounds.center().x(), bounds.center().y(), epsg4326, gx, gy);
    qDebug() << gx << " " << gy;

    // Sun etc.

    SGPath texturePath("../src/texture/");
    texturePath.append("Textures");
    texturePath.append("Sky")

    osg::LightSource* sunLight = new osg::LightSource;
    sunLight->getLight()->setDataVariance(osg::Object::DYNAMIC);
    sunLight->getLight()->setLightNum(1);
    //
    sunLight->setReferenceFrame(osg::LightSource::RELATIVE_RF);
    sunLight->setLocalStateSetModes(osg::StateAttribute::ON);

    Ephemeris* ephemeris = new Ephemeris;
    ephemeris->init();
    ephemeris->bind();

    _sky = new SGSky;
    _sky->texture_path(texturePath.str());

    _sky->build(80000.0, 80000.0,
                463.3, 361.8,
                ephemeris,
                NULL);

    osg::ref_ptr<osg::Node> skyGroup = new osg::Group;
    skyGroup->getOrCreateStateSet()->setMode(GL_LIGHT0, osg::StateAttribute::OFF);
    skyGroup->addChild(_sky->getPreRoot());
    sunLight->addChild(skyGroup);

    root->addChild(sunLight);
    osgUtil::Optimizer optimizer;
    optimizer.optimize(root.get());

    setSceneData(root.get());

    getCamera()->setUpdateCallback(new CameraEventCallback(this));
    emit cameraChanged(getCamera()->getViewMatrix());
    setCameraPosition(cx, cy, cz);

    return 0;
}

/**
 * Toggle polygon mode for display
 */
void ViewerWidget::togglePolygonMode(bool enable)
{
    if(_polygonMode == NULL) {
        _polygonMode = new osg::PolygonMode;
        _polygonMode->setMode(osg::PolygonMode::FRONT_AND_BACK, osg::PolygonMode::LINE);
    }

    if(enable) {
        getSceneData()->getOrCreateStateSet()->setAttribute(_polygonMode.get());
    } else {
        getSceneData()->getOrCreateStateSet()->removeAttribute(_polygonMode.get());
    }
}

/**
 * Set new camera position
 */
void ViewerWidget::setCameraPosition(double x, double y, double z)
{
    osg::Vec3d eye;
    osg::Quat rotation;

    _cameraManipulator->getTransformation(eye, rotation);
    eye = osg::Vec3(x, y, z);
    _cameraManipulator->setTransformation(eye, rotation);
}

