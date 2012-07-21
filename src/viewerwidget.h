#ifndef VIEWERWIDGET_H
#define VIEWERWIDGET_H

#include <QtCore/QTimer>
#include <QtGui/QWidget>
#include <osgViewer/Viewer>

class ViewerWidget : public QWidget, public osgViewer::Viewer
{
    Q_OBJECT

    public:
        ViewerWidget(osgViewer::ViewerBase::ThreadingModel threadingModel = osgViewer::Viewer::SingleThreaded,
            QWidget *parent = 0);

        virtual void paintEvent(QPaintEvent* event) { frame(); }

        int setMap(osg::Node* map);

    public slots:
        void stop();
        void start();

    protected:
        osg::Camera* createCamera(int x, int y, int w, int h, QString = "");
        QWidget* createWidget(osg::Camera* camera, osg::Node* scene);

        QTimer _timer;
        int _timerInterval;
};

#endif

