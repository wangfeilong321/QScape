#include <iostream>
#include <math.h>

#include <QtGui>
#include <QDebug>

#include <osg/Vec3>

#include "camerawidget.h"

CameraWidget::CameraWidget(QWidget *parent)
    : QWidget(parent)
{
    setupUi(this);

    connect(X, SIGNAL(valueChanged(double)), this, SLOT(onValueChanged()));
    connect(Y, SIGNAL(valueChanged(double)), this, SLOT(onValueChanged()));
    connect(Z, SIGNAL(valueChanged(double)), this, SLOT(onValueChanged()));
}


void CameraWidget::onCameraChanged(osg::Matrixd viewMatrix)
{
    osg::Vec3 center, eye, up, los;
    osg::Vec3 normal(0.0, 1.0, 0.0);

    viewMatrix.getLookAt(eye, center, up);
    los = center - eye;

    X->blockSignals(true); if(!X->hasFocus()) { X->setValue(center.x()); }; X->blockSignals(false);
    Y->blockSignals(true); if(!Y->hasFocus()) { Y->setValue(center.y()); }; Y->blockSignals(false);
    Z->blockSignals(true); if(!Z->hasFocus()) { Z->setValue(center.z()); }; Z->blockSignals(false);

    los.normalize();
    pitch->setValue(acos(los * osg::Vec3(1.0, 0.0, 0.0)) * RAD2DEG);
    yaw->setValue(  acos(los * osg::Vec3(0.0, 0.0, 1.0)) * RAD2DEG);
    roll->setValue( acos(los * osg::Vec3(0.0, 1.0, 0.0)) * RAD2DEG);
}

void CameraWidget::onValueChanged()
{
    emit dataChanged(X->value(), Y->value(), Z->value());
}

