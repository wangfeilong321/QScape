#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <osgViewer/ViewerBase>
#include <osg/Camera>

#include "ui_mainwindow.h"
#include "viewerwidget.h"
#include "camerawidget.h"

class MainWindow : public QMainWindow, private Ui::MainWindow
{
    Q_OBJECT

    public:
        MainWindow(osgViewer::ViewerBase::ThreadingModel threadingModel, QString filename = NULL, QWidget *parent = 0);


    private slots:
        void on_actionQuit_triggered();
        void on_actionOpen_triggered();

    private:
        ViewerWidget *_viewerWidget;
        CameraWidget *_cameraWidget;
        void _loadFile(QString filename);
        void createDockWindows();
};

#endif

