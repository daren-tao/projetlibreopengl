#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include "mainpanel.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void openFile();
    void saveImage();
    void setDockVisible();
    void toggleGaussianBlur();
    void toggleBilateralFilter();
    void toggleHistogramEqualization();
    void toggleEdgeDetection();

private:
    Ui::MainWindow *ui;
    MainPanel* centralWidget;
    QDockWidget* dockWidget;
    QAction* openAction;
    QAction* saveAction;
    QAction* showDockAction;
    QAction* exitAction;
    QGroupBox* gaussianBlurGroup;
    QGroupBox* bilateralFilterGroup;
    QGroupBox* histogramEqualizationGroup;
    QGroupBox* edgeDetectionGroup;
    QCheckBox* btnGaussianBlurEnable;
    QCheckBox* btnBilateralFilterEnable;
    QCheckBox* btnHistogramEqualizationEnable;
    QCheckBox* btnEdgeDetectionEnable;
    void createMenuBar();
    void createCentralWidget();
    void createDockWidgets();
    void fillGaussianBlurGroup();
    void fillBilateralFilterGroup();
    void fillHistogramEqualizationGroup();
    void fillEdgeDetectionGroup();
    void connectActions();
};

#endif // MAINWINDOW_H
