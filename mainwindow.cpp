#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {
    ui->setupUi(this);
    setWindowTitle("Image Filtering Tools");
    statusBar()->hide();

    /* menu bar */
    createMenuBar();

    /* central widget */
    createCentralWidget();

    /* dock widgets */
    createDockWidgets();

    /* connect actions */
    connectActions();
}

void MainWindow::createMenuBar() {
    QMenu* fileMenu = menuBar()->addMenu("File");
    QMenu* displayMenu = menuBar()->addMenu("Display");

    openAction = new QAction("Open", this);
    openAction->setShortcut(QKeySequence("Ctrl+O"));

    saveAction = new QAction("Save", this);
    saveAction->setShortcut(QKeySequence("Ctrl+S"));

    showDockAction = new QAction("Show algorithms window", this);
    showDockAction->setShortcut(QKeySequence("Ctrl+D"));

    exitAction = new QAction("Exit", this);
    exitAction->setShortcut(QKeySequence("Alt+F4"));

    fileMenu->addAction(openAction);
    fileMenu->addAction(saveAction);
    fileMenu->addAction(exitAction);

    displayMenu->addAction(showDockAction);
}

void MainWindow::createCentralWidget() {
    centralWidget = new MainPanel();
    centralWidget->setParent(this);
    setCentralWidget(centralWidget);
}

void MainWindow::openFile() {
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), QString(), tr("*.bmp *.jpg *.png *.tga)"));
    centralWidget->loadImage(fileName);
}

void MainWindow::saveImage() {
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Image"), QString(), tr("Image Files(*.bmp)"));
    centralWidget->saveImage(fileName);
}

void MainWindow::createDockWidgets() {
    dockWidget = new QDockWidget(tr("Algorithms"), this);
    dockWidget->setAllowedAreas(Qt::RightDockWidgetArea);

    gaussianBlurGroup = new QGroupBox(tr("Gaussian Blur"));
    fillGaussianBlurGroup();

    bilateralFilterGroup = new QGroupBox(tr("Bilateral Filter"));
    fillBilateralFilterGroup();

    histogramEqualizationGroup = new QGroupBox(tr("Histogram Equalization"));
    fillHistogramEqualizationGroup();

    edgeDetectionGroup = new QGroupBox(tr("Edge Detection"));
    fillEdgeDetectionGroup();

    QVBoxLayout* layout = new QVBoxLayout();
    layout->addWidget(gaussianBlurGroup);
    layout->addWidget(bilateralFilterGroup);
    layout->addWidget(histogramEqualizationGroup);
    layout->addWidget(edgeDetectionGroup);

    QWidget* widget = new QWidget();
    widget->setLayout(layout);
    dockWidget->setWidget(widget);
    addDockWidget(Qt::RightDockWidgetArea, dockWidget);
}

void MainWindow::fillGaussianBlurGroup() {
    QGridLayout* layout = new QGridLayout();
    btnGaussianBlurEnable = new QCheckBox();
    btnGaussianBlurEnable->setText("Disabled");
    layout->addWidget(btnGaussianBlurEnable, 0, 0);
    gaussianBlurGroup->setLayout(layout);
}

void MainWindow::fillBilateralFilterGroup() {
    QGridLayout* layout = new QGridLayout();
    btnBilateralFilterEnable = new QCheckBox();
    btnBilateralFilterEnable->setText("Disabled");
    layout->addWidget(btnBilateralFilterEnable, 0, 0);
    bilateralFilterGroup->setLayout(layout);
}

void MainWindow::fillHistogramEqualizationGroup() {
    QGridLayout* layout = new QGridLayout();
    btnHistogramEqualizationEnable = new QCheckBox();
    btnHistogramEqualizationEnable->setText("Disabled");
    layout->addWidget(btnHistogramEqualizationEnable, 0, 0);
    histogramEqualizationGroup->setLayout(layout);
}

void MainWindow::fillEdgeDetectionGroup() {
    QGridLayout* layout = new QGridLayout();
    btnEdgeDetectionEnable = new QCheckBox();
    btnEdgeDetectionEnable->setText("Disabled");
    layout->addWidget(btnEdgeDetectionEnable, 0, 0);
    edgeDetectionGroup->setLayout(layout);
}

void MainWindow::toggleGaussianBlur() {
    if(btnGaussianBlurEnable->isChecked()) {
        btnGaussianBlurEnable->setText("Enabled");
        btnBilateralFilterEnable->setChecked(false);
        btnHistogramEqualizationEnable->setChecked(false);
        btnEdgeDetectionEnable->setChecked(false);

        toggleBilateralFilter();
        toggleHistogramEqualization();
        toggleEdgeDetection();
    } else {
        btnGaussianBlurEnable->setText("Disabled");
    }
    centralWidget->updateGB(btnGaussianBlurEnable->isChecked());
}

void MainWindow::toggleBilateralFilter() {
    if(btnBilateralFilterEnable->isChecked()) {
        btnBilateralFilterEnable->setText("Enabled");
        btnGaussianBlurEnable->setChecked(false);
        btnHistogramEqualizationEnable->setChecked(false);
        btnEdgeDetectionEnable->setChecked(false);

        toggleGaussianBlur();
        toggleHistogramEqualization();
        toggleEdgeDetection();
    } else {
        btnBilateralFilterEnable->setText("Disabled");
    }
    centralWidget->updateBF(btnBilateralFilterEnable->isChecked());
}

void MainWindow::toggleHistogramEqualization() {
    if(btnHistogramEqualizationEnable->isChecked()) {
        btnHistogramEqualizationEnable->setText("Enabled");
        btnBilateralFilterEnable->setChecked(false);
        btnGaussianBlurEnable->setChecked(false);
        btnEdgeDetectionEnable->setChecked(false);

        toggleBilateralFilter();
        toggleGaussianBlur();
        toggleEdgeDetection();
    } else {
        btnHistogramEqualizationEnable->setText("Disabled");
    }
}

void MainWindow::toggleEdgeDetection() {
    if(btnEdgeDetectionEnable->isChecked()) {
        btnEdgeDetectionEnable->setText("Enabled");
        btnBilateralFilterEnable->setChecked(false);
        btnGaussianBlurEnable->setChecked(false);
        btnHistogramEqualizationEnable->setChecked(false);

        toggleBilateralFilter();
        toggleHistogramEqualization();
        toggleGaussianBlur();
    } else {
        btnEdgeDetectionEnable->setText("Disabled");
    }
    centralWidget->updateED(btnEdgeDetectionEnable->isChecked());
}

void MainWindow::setDockVisible() {
    if(!dockWidget->isVisible()) {
        dockWidget->setVisible(true);
    }
}

void MainWindow::connectActions() {
    connect(btnGaussianBlurEnable, SIGNAL(released()), this, SLOT(toggleGaussianBlur()));
    connect(btnBilateralFilterEnable, SIGNAL(released()), this, SLOT(toggleBilateralFilter()));
    connect(btnHistogramEqualizationEnable, SIGNAL(released()), this, SLOT(toggleHistogramEqualization()));
    connect(btnEdgeDetectionEnable, SIGNAL(released()), this, SLOT(toggleEdgeDetection()));
    connect(showDockAction, SIGNAL(triggered()), this, SLOT(setDockVisible()));
    connect(saveAction, SIGNAL(triggered()), this, SLOT(saveImage()));
    connect(openAction, SIGNAL(triggered()), this, SLOT(openFile()));
    connect(exitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
}

MainWindow::~MainWindow() {
    delete ui;
}
