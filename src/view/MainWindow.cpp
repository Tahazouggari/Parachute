#include "MainWindow.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QFileDialog>
#include <QMainWindow>
#include <QLineEdit>
#include <QSlider>
#include <QSpinBox>
#include <QPushButton>
#include <QColorDialog>
#include <QWidget>

// Chemins relatifs depuis le répertoire du projet
#include "../presenter/ParachutePresenter.h"
#include "ParachuteView.h"
#include "../model/MessageEncoder.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), presenter(new ParachutePresenter(this)) {

    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);

    QLabel *labelMessage = new QLabel("Your message:");
    messageInput = new QLineEdit();
    connect(messageInput, &QLineEdit::textChanged, this, &MainWindow::onMessageChanged);

    QLabel *labelSectors = new QLabel("Sectors:");
    sliderSectors = new QSlider(Qt::Horizontal);
    sliderSectors->setRange(7, 28);
    spinSectors = new QSpinBox();
    spinSectors->setRange(7, 28);

    QLabel *labelTracks = new QLabel("Tracks:");
    sliderTracks = new QSlider(Qt::Horizontal);
    sliderTracks->setRange(3, 10);
    spinTracks = new QSpinBox();
    spinTracks->setRange(3, 10);

    colorButton = new QPushButton("Choose Background Color");
    connect(colorButton, &QPushButton::clicked, this, &MainWindow::onBackgroundColorChanged);

    exportButton = new QPushButton("Export Parachute Image");
    connect(exportButton, &QPushButton::clicked, this, &MainWindow::onSaveParachute);

    parachuteView = new ParachuteView(this);

    layout->addWidget(labelMessage);
    layout->addWidget(messageInput);
    layout->addWidget(labelSectors);
    layout->addWidget(sliderSectors);
    layout->addWidget(spinSectors);
    layout->addWidget(labelTracks);
    layout->addWidget(sliderTracks);
    layout->addWidget(spinTracks);
    layout->addWidget(colorButton);
    layout->addWidget(exportButton);
    layout->addWidget(parachuteView);

    setCentralWidget(centralWidget);
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


void MainWindow::onSaveParachute() {
    QString filename = QFileDialog::getSaveFileName(this, "Save Image", "", "PNG Image (*.png)");
    if (!filename.isEmpty()) {
        parachuteView->saveParachuteImage(filename);
    }
}

MainWindow::~MainWindow() {
    delete presenter;
}
