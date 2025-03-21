#include "MainWindow.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QFileDialog>
#include <QGroupBox>
#include <QTabWidget>
#include <QHBoxLayout>
#include <QTextEdit>
#include <QMessageBox>
#include <QColorDialog>
#include <QMenuBar>
#include <QStatusBar>
#include <QAction>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), presenter(new ParachutePresenter(this)) {

    // Menu Bar
    fileMenu = menuBar()->addMenu(tr("&File")); // Use menuBar() directly
    saveAction = fileMenu->addAction(tr("&Save"), this, &MainWindow::onSaveFile, QKeySequence::Save);
    openAction = fileMenu->addAction(tr("&Open"), this, &MainWindow::onOpenFile, QKeySequence::Open);
    exitAction = fileMenu->addAction(tr("E&xit"), this, &MainWindow::onExit, QKeySequence::Quit);

    // Status Bar
    statusBar()->showMessage("Ready"); // Use statusBar() directly

    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    // Top Section: Title and Message Box
    QLabel *titleLabel = new QLabel("Parachute Encoder");
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 18px; font-weight: bold;");
    mainLayout->addWidget(titleLabel);

    QGroupBox *messageBox = new QGroupBox("Message");
    QVBoxLayout *messageLayout = new QVBoxLayout(messageBox);

    QLabel *labelMessage = new QLabel("Your message:");
    messageInput = new QLineEdit();
    connect(messageInput, &QLineEdit::textChanged, this, &MainWindow::onMessageChanged);

    messageLayout->addWidget(labelMessage);
    messageLayout->addWidget(messageInput);

    QHBoxLayout *topLayout = new QHBoxLayout();
    topLayout->addStretch(); // Push the message box to the right
    topLayout->addWidget(messageBox);
    mainLayout->addLayout(topLayout);

    // Middle Section: Tabs for Views
    viewTabs = new QTabWidget(); // Initialize viewTabs

    // Parachute View
    parachuteView = new ParachuteView(this);
    viewTabs->addTab(parachuteView, tr("Parachute View"));

    // Binary View
    binaryView = new QWidget(); // Initialize binaryView
    QVBoxLayout *binaryLayout = new QVBoxLayout(binaryView);
    QTextEdit *binaryText = new QTextEdit();
    binaryText->setReadOnly(true);
    binaryLayout->addWidget(binaryText);
    viewTabs->addTab(binaryView, tr("Binary View"));

    mainLayout->addWidget(viewTabs);

    // Bottom Section: Controls
    QGroupBox *controlsBox = new QGroupBox("Controls");
    QVBoxLayout *controlsLayout = new QVBoxLayout(controlsBox);

    QLabel *labelSectors = new QLabel("Sectors:");
    sliderSectors = new QSlider(Qt::Horizontal);
    sliderSectors->setRange(7, 28);
    spinSectors = new QSpinBox();
    spinSectors->setRange(7, 28);
    connect(sliderSectors, &QSlider::valueChanged, spinSectors, &QSpinBox::setValue);
    connect(spinSectors, QOverload<int>::of(&QSpinBox::valueChanged), sliderSectors, &QSlider::setValue);
    connect(spinSectors, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainWindow::onSectorsOrTracksChanged);

    QLabel *labelTracks = new QLabel("Tracks:");
    sliderTracks = new QSlider(Qt::Horizontal);
    sliderTracks->setRange(3, 10);
    spinTracks = new QSpinBox();
    spinTracks->setRange(3, 10);
    connect(sliderTracks, &QSlider::valueChanged, spinTracks, &QSpinBox::setValue);
    connect(spinTracks, QOverload<int>::of(&QSpinBox::valueChanged), sliderTracks, &QSlider::setValue);
    connect(spinTracks, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainWindow::onSectorsOrTracksChanged);

    controlsLayout->addWidget(labelSectors);
    controlsLayout->addWidget(sliderSectors);
    controlsLayout->addWidget(spinSectors);
    controlsLayout->addWidget(labelTracks);
    controlsLayout->addWidget(sliderTracks);
    controlsLayout->addWidget(spinTracks);

    QPushButton *colorButton = new QPushButton("Choose Background Color");
    connect(colorButton, &QPushButton::clicked, this, &MainWindow::onBackgroundColorChanged);
    controlsLayout->addWidget(colorButton);

    QPushButton *exportButton = new QPushButton("Export Parachute Image");
    connect(exportButton, &QPushButton::clicked, this, &MainWindow::onSaveParachute);
    controlsLayout->addWidget(exportButton);

    mainLayout->addWidget(controlsBox);

    setCentralWidget(centralWidget);

    // Initialize sliders and spin boxes with default values
    sliderSectors->setValue(7);
    spinSectors->setValue(7);

    sliderTracks->setValue(3);
    spinTracks->setValue(3);

    // Ensure the parachute view is updated with initial values
    onSectorsOrTracksChanged();
}

void MainWindow::onBackgroundColorChanged() {
    QColor color = QColorDialog::getColor(Qt::white, this, "Choose Background Color");
    if (color.isValid()) {
        parachuteView->setBackgroundColor(color);
    }
}

void MainWindow::onMessageChanged() {
    QString message = messageInput->text();
    std::vector<int> encodedMessage = MessageEncoder::encodeMessage(message);
    parachuteView->setParachuteData(spinSectors->value(), spinTracks->value(), encodedMessage);
}

void MainWindow::onSectorsOrTracksChanged() {
    std::vector<int> encodedMessage = MessageEncoder::encodeMessage(messageInput->text());
    parachuteView->setParachuteData(spinSectors->value(), spinTracks->value(), encodedMessage);
}

void MainWindow::onSaveParachute() {
    QString filename = QFileDialog::getSaveFileName(this, "Save Image", "", "PNG Image (*.png)");
    if (!filename.isEmpty()) {
        parachuteView->saveParachuteImage(filename);
    }
}

void MainWindow::onSaveFile() {
    QString filename = QFileDialog::getSaveFileName(this, "Save File", "", "Parachute Files (*.parachute)");
    if (!filename.isEmpty()) {
        // Save message and parameters to file
        statusBar()->showMessage("File saved successfully", 3000);
    }
}

void MainWindow::onOpenFile() {
    QString filename = QFileDialog::getOpenFileName(this, "Open File", "", "Parachute Files (*.parachute)");
    if (!filename.isEmpty()) {
        // Load message and parameters from file
        statusBar()->showMessage("File loaded successfully", 3000);
    }
}

void MainWindow::onExit() {
    close();
}

MainWindow::~MainWindow() {
    delete presenter;
}
