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
    void toggleSharpening();
    void toggleEdgeDetection();

    void changeKernelValueGB(int);
    void changeDeviationValueGB(int);

    void changeKernelValueBF(int);
    void changeDeviationValueBF(int);
    void changeRangeValueBF(int);

    void changeValueSH(int);

    void changeValueED(int);

private:
    Ui::MainWindow *ui;
    MainPanel* centralWidget;
    QDockWidget* dockWidget;
    QAction* openAction;
    QAction* saveAction;
    QAction* showDockAction;
    QAction* exitAction;

    QGroupBox* gaussianBlurGroup;
    QCheckBox* btnGaussianBlurEnable;
    QSlider* gbKernelSizeSlider;
    QSlider* gbDeviationSlider;
    QLabel* gbKernelSizeLabel;
    QLabel* gbDeviationLabel;

    QGroupBox* bilateralFilterGroup;
    QCheckBox* btnBilateralFilterEnable;
    QSlider* bfKernelSizeSlider;
    QSlider* bfDeviationSlider;
    QSlider* bfRangeSlider;
    QLabel* bfKernelSizeLabel;
    QLabel* bfDeviationLabel;
    QLabel* bfRangeLabel;

    QGroupBox* sharpeningGroup;
    QCheckBox* btnSharpeningEnable;
    QSlider* shScaleFactorSlider;
    QLabel* shScaleFactorLabel;

    QGroupBox* edgeDetectionGroup;
    QCheckBox* btnEdgeDetectionEnable;
    QComboBox* edAlgorithmComboBox;
    QLabel* edAlgorithmLabel;

    void createMenuBar();
    void createCentralWidget();
    void createDockWidgets();
    void fillGaussianBlurGroup();
    void fillBilateralFilterGroup();
    void fillSharpeningGroup();
    void fillEdgeDetectionGroup();
    void connectActions();
};

#endif // MAINWINDOW_H
