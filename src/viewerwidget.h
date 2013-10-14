#ifndef VIEWERWIDGET_H
#define VIEWERWIDGET_H

#include <iostream>

#include <QtCore/QTimer>
#include <QtGui/QWidget>

#include <osgViewer/Viewer>
#include <osg/PolygonMode>
#include <osg/Camera>
#include <osgGA/StandardManipulator>

#include <osgEarth/Map>

#include <simgear/scene/sky/sky.hxx>

class ViewerWidget : public QWidget, public osgViewer::Viewer
{
    Q_OBJECT

    public:
        ViewerWidget(osgViewer::ViewerBase::ThreadingModel threadingModel = osgViewer::Viewer::SingleThreaded,
            QWidget *parent = 0);

        virtual void paintEvent(QPaintEvent* event) { frame(); }

        int setMap(osgEarth::Map* map);

        void cc(osg::Matrixd viewMatrix) { emit cameraChanged(viewMatrix); };

    public slots:
        void stop();
        void start();
        void togglePolygonMode(bool enable);
        void setCameraPosition(double x, double y, double z);

    signals:
        void cameraChanged(osg::Matrixd);

    protected:
        osg::Camera* createCamera(int x, int y, int w, int h, QString = "");
        QWidget* createWidget(osg::Camera* camera, osg::Node* scene);

        QTimer _timer;
        int _timerInterval;

        osg::ref_ptr<osg::PolygonMode> _polygonMode;
        osg::ref_ptr<osgGA::StandardManipulator> _cameraManipulator;
        SGSky* _sky;

};

class CameraEventCallback : public osg::NodeCallback
{
    ViewerWidget *viewer;
    osg::Vec3 eye, center, up;

    public:
        CameraEventCallback(ViewerWidget *viewer)
        {
            this->viewer = viewer;
        }

        virtual void operator()(osg::Node* node, osg::NodeVisitor* nv)
        {
            traverse(node, nv);
            osg::Camera* camera = dynamic_cast<osg::Camera *>(node);
            if(camera != NULL) {
                this->viewer->cc(camera->getViewMatrix());
            }
        }
};

#endif

