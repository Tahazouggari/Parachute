#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QFileDialog>
#include <QDebug> // Add this header for qDebug()
#include "../model/MessageEncoder.h"
#include "../view/ParachuteView.h"
#include "../view/BinaryWidget.h"
#include <QColorDialog>
#include "../utils/LanguageManager.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), languageManager(new LanguageManager(this)) {
    ui->setupUi(this); // Set up the UI

    // Initialize parachuteView
    parachuteView = new ParachuteView(this);

    // Add parachuteView to the parachuteViewTab
    QVBoxLayout *parachuteLayout = new QVBoxLayout(ui->parachuteViewTab);
    parachuteLayout->addWidget(parachuteView);

    // Initialize binaryWidget
    binaryWidget = new BinaryWidget(this);

    // Add binaryWidget to the binaryViewLayout
    QVBoxLayout *binaryLayout = qobject_cast<QVBoxLayout *>(ui->binaryViewTab->layout());
    if (binaryLayout) {
        binaryLayout->addWidget(binaryWidget);
    }

    // Connect signals and slots
    connect(ui->messageInput, &QLineEdit::textChanged, this, &MainWindow::onMessageChanged);
    connect(ui->colorButton, &QPushButton::clicked, this, &MainWindow::onBackgroundColorChanged);
    connect(ui->exportButton, &QPushButton::clicked, this, &MainWindow::onSaveParachute);
    connect(ui->saveAction, &QAction::triggered, this, &MainWindow::onSaveFile);
    connect(ui->openAction, &QAction::triggered, this, &MainWindow::onOpenFile);
    connect(ui->exitAction, &QAction::triggered, this, &MainWindow::onExit);

    // Connect sliders to update parachute view live
    connect(ui->sliderSectors, &QSlider::valueChanged, ui->spinSectors, &QSpinBox::setValue);
    connect(ui->spinSectors, QOverload<int>::of(&QSpinBox::valueChanged), ui->sliderSectors, &QSlider::setValue);
    connect(ui->sliderSectors, &QSlider::valueChanged, this, &MainWindow::onSectorsOrTracksChanged);

    connect(ui->sliderTracks, &QSlider::valueChanged, ui->spinTracks, &QSpinBox::setValue);
    connect(ui->spinTracks, QOverload<int>::of(&QSpinBox::valueChanged), ui->sliderTracks, &QSlider::setValue);
    connect(ui->sliderTracks, &QSlider::valueChanged, this, &MainWindow::onSectorsOrTracksChanged);

    // Initialize sliders and spin boxes
    ui->sliderSectors->setRange(7, 28);
    ui->spinSectors->setRange(7, 28);
    ui->sliderTracks->setRange(3, 10);
    ui->spinTracks->setRange(3, 10);

    ui->sliderSectors->setValue(7);
    ui->spinSectors->setValue(7);
    ui->sliderTracks->setValue(3);
    ui->spinTracks->setValue(3);

    // Connect language menu actions
    connect(ui->actionEnglish, &QAction::triggered, this, &MainWindow::onLanguageEnglish);
    connect(ui->actionfrensh, &QAction::triggered, this, &MainWindow::onLanguageFrench);
    connect(ui->actionArabic, &QAction::triggered, this, &MainWindow::onLanguageArabic);

    // Connect language manager signal
    connect(languageManager, &LanguageManager::languageChanged, this, &MainWindow::retranslateUi);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::onMessageChanged() {
    QString message = ui->messageInput->text();

    // Encode the message into binary
    std::vector<int> encodedMessage = MessageEncoder::encodeMessage(message);

    // Convert to bool vector for BinaryWidget
    std::vector<bool> binaryBits(encodedMessage.begin(), encodedMessage.end());

    // Update binaryWidget
    binaryWidget->updateBitSet(binaryBits);

    // Update the parachute view
    parachuteView->setParachuteData(ui->spinSectors->value(), ui->spinTracks->value(), encodedMessage);
}

void MainWindow::onBackgroundColorChanged() {
    QColor color = QColorDialog::getColor(Qt::white, this, "Choose Background Color");
    if (color.isValid()) {
        parachuteView->setBackgroundColor(color);
    }
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
        // Save message and parameters
    }
}

void MainWindow::onOpenFile() {
    QString filename = QFileDialog::getOpenFileName(this, "Open File", "", "Parachute Files (*.parachute)");
    if (!filename.isEmpty()) {
        // Load message and parameters
    }
}

void MainWindow::onExit() {
    close();
}

void MainWindow::onSectorsOrTracksChanged() {
    // Get the current message
    QString message = ui->messageInput->text();

    // Encode the message into binary
    std::vector<int> encodedMessage = MessageEncoder::encodeMessage(message);

    // Update the parachute view with the current sectors, tracks, and encoded message
    parachuteView->setParachuteData(ui->spinSectors->value(), ui->spinTracks->value(), encodedMessage);
}

void MainWindow::onLanguageEnglish() {
    languageManager->switchLanguage("en");
}

void MainWindow::onLanguageFrench() {
    languageManager->switchLanguage("fr");
}

void MainWindow::onLanguageArabic() {
    languageManager->switchLanguage("ar");
}

void MainWindow::retranslateUi() {
    ui->retranslateUi(this); // Retranslate the UI
    qDebug() << "UI retranslated to language:" << languageManager->getCurrentLanguage();
}

