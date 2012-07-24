#ifndef CAMERAWIDGET_H
#define CAMERAWIDGET_H

#include <QtGui/QWidget>

#include <osg/Matrixd>

#include "ui_camerawidget.h"

class CameraWidget : public QWidget, private Ui::CameraWidget
{
    Q_OBJECT

    static const double RAD2DEG = 57.2957795130823208768;

    public:
        CameraWidget(QWidget *parent = 0);

    public slots:
        void onCameraChanged(osg::Matrixd);
        void onValueChanged();

    signals:
        void dataChanged(double cx, double cy, double cz);
};

#endif

