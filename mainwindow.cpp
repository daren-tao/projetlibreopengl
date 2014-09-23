#include "mainwindow.h"
#include "ui_mainwindow.h"

/**
 * The main window GUI creation.
 * Creates all the components in the main window.
 * Connects actions using Qt slots and signals.
 * @brief MainWindow::MainWindow
 * @param parent
 */
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

/**
 * Creates the menu bar to handle actions.
 * @brief MainWindow::createMenuBar
 */
void MainWindow::createMenuBar() {

    // creating the file and display menus
    QMenu* fileMenu = menuBar()->addMenu("File");
    QMenu* displayMenu = menuBar()->addMenu("Display");

    // creating the open action
    openAction = new QAction("Open", this);
    openAction->setShortcut(QKeySequence("Ctrl+O"));

    // creating the save action
    saveAction = new QAction("Save", this);
    saveAction->setShortcut(QKeySequence("Ctrl+S"));

    // creating the algorithm dock show action
    showDockAction = new QAction("Show algorithms window", this);
    showDockAction->setShortcut(QKeySequence("Ctrl+D"));

    // creating the exit action
    exitAction = new QAction("Exit", this);
    exitAction->setShortcut(QKeySequence("Alt+F4"));

    // adding the actions to their menu
    fileMenu->addAction(openAction);
    fileMenu->addAction(saveAction);
    fileMenu->addAction(exitAction);
    displayMenu->addAction(showDockAction);
}

/**
 * Creates the opengl widget as the main window's central widget.
 * @brief MainWindow::createCentralWidget
 */
void MainWindow::createCentralWidget() {
    centralWidget = new MainPanel();
    centralWidget->setParent(this);
    setCentralWidget(centralWidget);
}

/**
 * Slot used to open an image file.
 * @brief MainWindow::openFile
 */
void MainWindow::openFile() {
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), QString(), tr("*.bmp *.jpg *.png *.tga)"));
    if(fileName != NULL) {
        centralWidget->loadImage(fileName);
    }
}

/**
 * Slot used to save an image file.
 * @brief MainWindow::saveImage
 */
void MainWindow::saveImage() {
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Image"), QString(), tr("Image Files(*.bmp)"));
    centralWidget->saveImage(fileName);
}

/**
 * Creates the algorithms panel as the main window's dock widget.
 * @brief MainWindow::createDockWidgets
 */
void MainWindow::createDockWidgets() {

    // creating the dock widget
    dockWidget = new QDockWidget(tr("Algorithms"), this);
    dockWidget->setAllowedAreas(Qt::RightDockWidgetArea);

    // creating the group for the gaussian blur's parameters
    gaussianBlurGroup = new QGroupBox(tr("Gaussian Blur"));
    fillGaussianBlurGroup();

    // creating the group for the bilateral filter's parameters
    bilateralFilterGroup = new QGroupBox(tr("Bilateral Filter"));
    fillBilateralFilterGroup();

    // creating the group for the sharpening's parameters
    sharpeningGroup = new QGroupBox(tr("Sharpening"));
    fillSharpeningGroup();

    // creating the group for the edge detection's parameters
    edgeDetectionGroup = new QGroupBox(tr("Edge Detection"));
    fillEdgeDetectionGroup();

    // adding all the algorithm groups to the dock widget's layout
    QVBoxLayout* layout = new QVBoxLayout();
    layout->addWidget(gaussianBlurGroup);
    layout->addWidget(bilateralFilterGroup);
    layout->addWidget(sharpeningGroup);
    layout->addWidget(edgeDetectionGroup);

    // in order to have a layout the dock widget has to have a parent which will have the layout
    QWidget* widget = new QWidget();
    widget->setLayout(layout);
    dockWidget->setWidget(widget);
    addDockWidget(Qt::RightDockWidgetArea, dockWidget);
}

/**
 * Creates the GUI for the gaussian blur's parameters.
 * @brief MainWindow::fillGaussianBlurGroup
 */
void MainWindow::fillGaussianBlurGroup() {

    // creating the layout
    QGridLayout* layout = new QGridLayout();

    // creating the enable checkbox
    btnGaussianBlurEnable = new QCheckBox();
    btnGaussianBlurEnable->setText("Disabled");

    // creating the kernel size parameter's GUI
    gbKernelSizeSlider = new QSlider(Qt::Horizontal, this);
    gbKernelSizeSlider->setRange(0, 3);
    gbKernelSizeSlider->setEnabled(false);
    gbKernelSizeLabel = new QLabel("Kernel size: 3x3", this);

    // creating the deviation parameter's GUI
    gbDeviationSlider = new QSlider(Qt::Horizontal, this);
    gbDeviationSlider->setRange(5, 50);
    gbDeviationSlider->setEnabled(false);
    gbDeviationLabel = new QLabel("Deviation: 0.5", this);

    // adding the controls to the layout
    layout->addWidget(btnGaussianBlurEnable, 0, 0);
    layout->addWidget(gbKernelSizeLabel, 1, 0);
    layout->addWidget(gbKernelSizeSlider, 2, 0);
    layout->addWidget(gbDeviationLabel, 3, 0);
    layout->addWidget(gbDeviationSlider, 4, 0);
    gaussianBlurGroup->setLayout(layout);
}

/**
 * Updates the value of the kernel size for the gaussian blur algorithm.
 * @brief MainWindow::changeKernelValueGB
 * @param value
 */
void MainWindow::changeKernelValueGB(int value) {
    int kernelSize = 3;
    switch(value) {
    case 0: gbKernelSizeLabel->setText(QString("Kernel size: 3x3"));
        kernelSize = 3;
        break;
    case 1: gbKernelSizeLabel->setText(QString("Kernel size: 5x5"));
        kernelSize = 5;
        break;
    case 2: gbKernelSizeLabel->setText(QString("Kernel size: 7x7"));
        kernelSize = 7;
        break;
    case 3: gbKernelSizeLabel->setText(QString("Kernel size: 9x9"));
        kernelSize = 9;
        break;
    }

    // updating in the opengl widget
    centralWidget->updateGB(kernelSize);
}

/**
 * Updates the value of the standard deviation for the gaussian blur algorithm.
 * @brief MainWindow::changeDeviationValueGB
 * @param value
 */
void MainWindow::changeDeviationValueGB(int value) {
    float deviation = value / 10.0;
    gbDeviationLabel->setText(QString("Deviation: %1").arg(deviation));

    // updating in the opengl widget
    centralWidget->updateGB(deviation);
}

/**
 * Creates the GUI for the bilateral filter's parameters.
 * @brief MainWindow::fillBilateralFilterGroup
 */
void MainWindow::fillBilateralFilterGroup() {

    // creating the layout
    QGridLayout* layout = new QGridLayout();

    // creating the enable checkbox
    btnBilateralFilterEnable = new QCheckBox();
    btnBilateralFilterEnable->setText("Disabled");

    // creating the kernel size parameter's GUI
    bfKernelSizeSlider = new QSlider(Qt::Horizontal, this);
    bfKernelSizeSlider->setRange(0, 3);
    bfKernelSizeSlider->setEnabled(false);
    bfKernelSizeLabel = new QLabel("Kernel size: 3x3", this);

    // creating the deviation parameter's GUI
    bfDeviationSlider = new QSlider(Qt::Horizontal, this);
    bfDeviationSlider->setRange(5, 50);
    bfDeviationSlider->setEnabled(false);
    bfDeviationLabel = new QLabel("Deviation: 0.5", this);

    // creating the range parameter's GUI
    bfRangeSlider = new QSlider(Qt::Horizontal, this);
    bfRangeSlider->setRange(1, 10);
    bfRangeSlider->setEnabled(false);
    bfRangeLabel = new QLabel("Range: 0.1", this);

    // adding the controls to the layout
    layout->addWidget(btnBilateralFilterEnable, 0, 0);
    layout->addWidget(bfKernelSizeLabel, 1, 0);
    layout->addWidget(bfKernelSizeSlider, 2, 0);
    layout->addWidget(bfDeviationLabel, 3, 0);
    layout->addWidget(bfDeviationSlider, 4, 0);
    layout->addWidget(bfRangeLabel, 5, 0);
    layout->addWidget(bfRangeSlider, 6, 0);
    bilateralFilterGroup->setLayout(layout);
}

/**
 * Updates the value of the kernel size for the bilateral filter algorithm.
 * @brief MainWindow::changeKernelValueBF
 * @param value
 */
void MainWindow::changeKernelValueBF(int value) {
    int kernelSize = 3;
    switch(value) {
    case 0: bfKernelSizeLabel->setText(QString("Kernel size: 3x3"));
        kernelSize = 3;
        break;
    case 1: bfKernelSizeLabel->setText(QString("Kernel size: 5x5"));
        kernelSize = 5;
        break;
    case 2: bfKernelSizeLabel->setText(QString("Kernel size: 7x7"));
        kernelSize = 7;
        break;
    case 3: bfKernelSizeLabel->setText(QString("Kernel size: 9x9"));
        kernelSize = 9;
        break;
    }

    // updating in the opengl widget
    centralWidget->updateBF(kernelSize);
}

/**
 * Updates the value of the standard deviation for the bilateral filter algorithm.
 * @brief MainWindow::changeDeviationValueBF
 * @param value
 */
void MainWindow::changeDeviationValueBF(int value) {
    float deviation = value / 10.0;
    bfDeviationLabel->setText(QString("Deviation: %1").arg(deviation));

    // updating in the opengl widget
    centralWidget->updateDeviationBF(deviation);
}

/**
 * Updates the value of the range for the bilateral filter algorithm.
 * @brief MainWindow::changeRangeValueBF
 * @param value
 */
void MainWindow::changeRangeValueBF(int value) {
    float range = value / 10.0;
    bfRangeLabel->setText(QString("Range: %1").arg(range));

    // updating in the opengl widget
    centralWidget->updateRangeBF(range);
}

/**
 * Creates the GUI for the sharpening's parameters.
 * @brief MainWindow::fillSharpeningGroup
 */
void MainWindow::fillSharpeningGroup() {

    // creating the layout
    QGridLayout* layout = new QGridLayout();

    // creating the enable checkbox
    btnSharpeningEnable = new QCheckBox();
    btnSharpeningEnable->setText("Disabled");

    // creating the scale factor parameter's GUI
    shScaleFactorSlider = new QSlider(Qt::Horizontal, this);
    shScaleFactorSlider->setRange(0, 500);
    shScaleFactorSlider->setEnabled(false);
    shScaleFactorLabel = new QLabel("Scale factor: 0", this);

    // adding the controls to the layout
    layout->addWidget(btnSharpeningEnable, 0, 0);
    layout->addWidget(shScaleFactorLabel, 1, 0);
    layout->addWidget(shScaleFactorSlider, 2, 0);
    sharpeningGroup->setLayout(layout);
}

/**
 * Updates the value of the scale factor for the sharpening algorithm.
 * @brief MainWindow::changeValueSH
 * @param value
 */
void MainWindow::changeValueSH(int value) {
    float scaleFactor = value / 10.0;
    shScaleFactorLabel->setText(QString("Scale factor: %1").arg(scaleFactor));

    // updating in the opengl widget
    centralWidget->updateSH(scaleFactor);
}

/**
 * Creates the GUI for the edge detection's parameters.
 * @brief MainWindow::fillEdgeDetectionGroup
 */
void MainWindow::fillEdgeDetectionGroup() {

    // creating the layout
    QGridLayout* layout = new QGridLayout();

    // creating the enable checkbox
    btnEdgeDetectionEnable = new QCheckBox();
    btnEdgeDetectionEnable->setText("Disabled");

    // creating the algorithm choice parameter's GUI
    edAlgorithmComboBox = new QComboBox(this);
    edAlgorithmComboBox->addItem("LoG");
    edAlgorithmComboBox->addItem("Sobel");
    edAlgorithmComboBox->addItem("Prewitt");
    edAlgorithmComboBox->setEnabled(false);
    edAlgorithmLabel = new QLabel("Algorithm", this);

    // adding the controls to the layout
    layout->addWidget(btnEdgeDetectionEnable, 0, 0);
    layout->addWidget(edAlgorithmLabel, 1, 0);
    layout->addWidget(edAlgorithmComboBox, 1, 1);
    edgeDetectionGroup->setLayout(layout);
}

/**
 * Updates the value of the algo choice for the edge detection algorithm.
 * @brief MainWindow::changeValueED
 * @param value
 */
void MainWindow::changeValueED(int value){

    // updating in the opengl widget
    centralWidget->updateED(value);
}

/**
 * Updates the GUI for the gaussian blur group in the dock widget.
 * @brief MainWindow::toggleGaussianBlur
 */
void MainWindow::toggleGaussianBlur() {

    // each time the checkbox is triggered, updating the enablement of the controls
    if(btnGaussianBlurEnable->isChecked()) {
        btnGaussianBlurEnable->setText("Enabled");
        btnBilateralFilterEnable->setChecked(false);
        btnSharpeningEnable->setChecked(false);
        btnEdgeDetectionEnable->setChecked(false);

        toggleBilateralFilter();
        toggleSharpening();
        toggleEdgeDetection();
    } else {
        btnGaussianBlurEnable->setText("Disabled");
    }
    gbKernelSizeSlider->setEnabled(btnGaussianBlurEnable->isChecked());
    gbDeviationSlider->setEnabled(btnGaussianBlurEnable->isChecked());

    // updating in the opengl widget
    centralWidget->updateGB(btnGaussianBlurEnable->isChecked());
}

/**
 * Updates the GUI for the bilateral filter group in the dock widget.
 * @brief MainWindow::toggleBilateralFilter
 */
void MainWindow::toggleBilateralFilter() {

    // each time the checkbox is triggered, updating the enablement of the controls
    if(btnBilateralFilterEnable->isChecked()) {
        btnBilateralFilterEnable->setText("Enabled");
        btnGaussianBlurEnable->setChecked(false);
        btnSharpeningEnable->setChecked(false);
        btnEdgeDetectionEnable->setChecked(false);

        toggleGaussianBlur();
        toggleSharpening();
        toggleEdgeDetection();
    } else {
        btnBilateralFilterEnable->setText("Disabled");
    }
    bfKernelSizeSlider->setEnabled(btnBilateralFilterEnable->isChecked());
    bfDeviationSlider->setEnabled(btnBilateralFilterEnable->isChecked());
    bfRangeSlider->setEnabled(btnBilateralFilterEnable->isChecked());

    // updating in the opengl widget
    centralWidget->updateBF(btnBilateralFilterEnable->isChecked());
}

/**
 * Updates the GUI for the sharpening group in the dock widget.
 * @brief MainWindow::toggleSharpening
 */
void MainWindow::toggleSharpening() {

    // each time the checkbox is triggered, updating the enablement of the controls
    if(btnSharpeningEnable->isChecked()) {
        btnSharpeningEnable->setText("Enabled");
        btnBilateralFilterEnable->setChecked(false);
        btnGaussianBlurEnable->setChecked(false);
        btnEdgeDetectionEnable->setChecked(false);

        toggleBilateralFilter();
        toggleGaussianBlur();
        toggleEdgeDetection();
    } else {
        btnSharpeningEnable->setText("Disabled");
    }
    shScaleFactorSlider->setEnabled(btnSharpeningEnable->isChecked());

    // updating in the opengl widget
    centralWidget->updateSH(btnSharpeningEnable->isChecked());
}

/**
 * Updates the GUI for the edge detection group in the dock widget.
 * @brief MainWindow::toggleEdgeDetection
 */
void MainWindow::toggleEdgeDetection() {

    // each time the checkbox is triggered, updating the enablement of the controls
    if(btnEdgeDetectionEnable->isChecked()) {
        btnEdgeDetectionEnable->setText("Enabled");
        btnBilateralFilterEnable->setChecked(false);
        btnGaussianBlurEnable->setChecked(false);
        btnSharpeningEnable->setChecked(false);

        toggleBilateralFilter();
        toggleSharpening();
        toggleGaussianBlur();
    } else {
        btnEdgeDetectionEnable->setText("Disabled");
    }
    edAlgorithmComboBox->setEnabled(btnEdgeDetectionEnable->isChecked());

    // updating in the opengl widget
    centralWidget->updateED(btnEdgeDetectionEnable->isChecked());
}

/**
 * Slot used to make the algorithms dock widget visible if it is not.
 * @brief MainWindow::setDockVisible
 */
void MainWindow::setDockVisible() {
    if(!dockWidget->isVisible()) {
        dockWidget->setVisible(true);
    }
}

/**
 * Connects all the signals with their corresponding slots.
 * @brief MainWindow::connectActions
 */
void MainWindow::connectActions() {
    connect(btnGaussianBlurEnable, SIGNAL(released()), this, SLOT(toggleGaussianBlur()));
    connect(btnBilateralFilterEnable, SIGNAL(released()), this, SLOT(toggleBilateralFilter()));
    connect(btnSharpeningEnable, SIGNAL(released()), this, SLOT(toggleSharpening()));
    connect(btnEdgeDetectionEnable, SIGNAL(released()), this, SLOT(toggleEdgeDetection()));

    connect(showDockAction, SIGNAL(triggered()), this, SLOT(setDockVisible()));
    connect(saveAction, SIGNAL(triggered()), this, SLOT(saveImage()));
    connect(openAction, SIGNAL(triggered()), this, SLOT(openFile()));
    connect(exitAction, SIGNAL(triggered()), qApp, SLOT(quit()));

    connect(gbKernelSizeSlider, SIGNAL(valueChanged(int)), this, SLOT(changeKernelValueGB(int)));
    connect(gbDeviationSlider, SIGNAL(valueChanged(int)), this, SLOT(changeDeviationValueGB(int)));

    connect(bfKernelSizeSlider, SIGNAL(valueChanged(int)), this, SLOT(changeKernelValueBF(int)));
    connect(bfDeviationSlider, SIGNAL(valueChanged(int)), this, SLOT(changeDeviationValueBF(int)));
    connect(bfRangeSlider, SIGNAL(valueChanged(int)), this, SLOT(changeRangeValueBF(int)));

    connect(shScaleFactorSlider, SIGNAL(valueChanged(int)), this, SLOT(changeValueSH(int)));

    connect(edAlgorithmComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(changeValueED(int)));
}

MainWindow::~MainWindow() {
    delete ui;
}
