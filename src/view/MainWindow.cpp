#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QFileDialog>
#include <QDebug>
#include "../model/MessageEncoder.h"
#include "../view/ParachuteView.h"
#include "../view/BinaryWidget.h"
#include <QColorDialog>
#include "../utils/LanguageManager.h"
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QPainter>
#include "HexView.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), languageManager(new LanguageManager(this)) {
    ui->setupUi(this);

    parachuteView = new ParachuteView(this);
    QVBoxLayout *parachuteLayout = new QVBoxLayout(ui->parachuteViewTab);
    parachuteLayout->addWidget(parachuteView);

    binaryWidget = new BinaryWidget(this);
    QVBoxLayout *binaryLayout = qobject_cast<QVBoxLayout *>(ui->binaryViewTab->layout());
    if (binaryLayout) {
        binaryLayout->addWidget(binaryWidget);
    }

    hexView = new HexView(this);
    QVBoxLayout *hexLayout = new QVBoxLayout(ui->hexViewTab);
    hexLayout->addWidget(hexView);

    connect(ui->messageInput, &QLineEdit::textChanged, this, &MainWindow::onMessageChanged);
    connect(ui->colorButton, &QPushButton::clicked, this, &MainWindow::onBackgroundColorChanged);
    connect(ui->exportButton, &QPushButton::clicked, this, &MainWindow::onSaveParachute);
    connect(ui->saveAction, &QAction::triggered, this, &MainWindow::onSaveFile);
    connect(ui->openAction, &QAction::triggered, this, &MainWindow::onOpenFile);
    connect(ui->exitAction, &QAction::triggered, this, &MainWindow::onExit);

    connect(ui->sliderSectors, &QSlider::valueChanged, ui->spinSectors, &QSpinBox::setValue);
    connect(ui->spinSectors, QOverload<int>::of(&QSpinBox::valueChanged), ui->sliderSectors, &QSlider::setValue);
    connect(ui->sliderSectors, &QSlider::valueChanged, this, &MainWindow::onSectorsOrTracksChanged);

    connect(ui->sliderTracks, &QSlider::valueChanged, ui->spinTracks, &QSpinBox::setValue);
    connect(ui->spinTracks, QOverload<int>::of(&QSpinBox::valueChanged), ui->sliderTracks, &QSlider::setValue);
    connect(ui->sliderTracks, &QSlider::valueChanged, this, &MainWindow::onSectorsOrTracksChanged);

    ui->sliderSectors->setRange(7, 28);
    ui->spinSectors->setRange(7, 28);
    ui->sliderTracks->setRange(3, 10);
    ui->spinTracks->setRange(3, 10);

    ui->sliderSectors->setValue(7);
    ui->spinSectors->setValue(7);
    ui->sliderTracks->setValue(3);
    ui->spinTracks->setValue(3);

    connect(ui->actionEnglish, &QAction::triggered, this, &MainWindow::onLanguageEnglish);
    connect(ui->actionfrensh, &QAction::triggered, this, &MainWindow::onLanguageFrench);
    connect(ui->actionArabic, &QAction::triggered, this, &MainWindow::onLanguageArabic);

    connect(languageManager, &LanguageManager::languageChanged, this, &MainWindow::retranslateUi);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::onMessageChanged() {
    QString message = ui->messageInput->text();
    std::vector<int> encodedMessage = MessageEncoder::encodeMessage(message);
    std::vector<bool> binaryBits(encodedMessage.begin(), encodedMessage.end());

  

    binaryWidget->updateBitSet(binaryBits);
    parachuteView->setParachuteData(ui->spinSectors->value(), ui->spinTracks->value(), encodedMessage);

    QString hexData;
    for (int i = 0; i < encodedMessage.size(); i += 8) {
        int byte = 0;
        for (int j = 0; j < 8 && i + j < encodedMessage.size(); ++j) {
            byte = (byte << 1) | encodedMessage[i + j];
        }
        hexData += QString("%1").arg(byte, 2, 16, QChar('0')).toUpper();
    }
    hexView->setHexData(hexData);
}

void MainWindow::onBackgroundColorChanged() {
    QColor color = QColorDialog::getColor(Qt::white, this, "Choose Background Color");
    if (color.isValid()) {
        parachuteView->setBackgroundColor(color);
        binaryWidget->setBackgroundColor(color);
        hexView->setBackgroundColor(color);
    }
}

void MainWindow::onSaveParachute() {
    QString filename = QFileDialog::getSaveFileName(this, "Save Image", "", "PNG Image (*.png)");
    if (!filename.isEmpty()) {
        if (!filename.endsWith(".png")) {
            filename += ".png";
        }
        QSize parachuteSize = parachuteView->size();
        QPixmap parachutePixmap(parachuteSize);
        parachutePixmap.fill(Qt::white);
        QPainter painter(&parachutePixmap);
        parachuteView->render(&painter);
        painter.end();
        if (parachutePixmap.save(filename)) {
            QMessageBox::information(this, "Export Image", "Parachute image saved successfully.");
        } else {
            QMessageBox::warning(this, "Export Image", "Failed to save the parachute image.");
        }
    }
}

void MainWindow::onSaveFile() {
    QString filename = QFileDialog::getSaveFileName(this, "Save File", "", "Parachute Files (*.ep)");
    if (!filename.isEmpty()) {
        if (!filename.endsWith(".ep")) {
            filename += ".ep";
        }
        QFile file(filename);
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&file);
            QString message = ui->messageInput->text();
            out << "[Message]\n" << message << "\n";
            int sectors = ui->spinSectors->value();
            int tracks = ui->spinTracks->value();
            out << "[Parameters]\n";
            out << "Sectors=" << sectors << "\n";
            out << "Tracks=" << tracks << "\n";
            file.close();
            QMessageBox::information(this, "Save File", "File saved successfully.");
        } else {
            QMessageBox::warning(this, "Save File", "Unable to open file for writing.");
        }
    }
}

void MainWindow::onOpenFile() {
    QString filename = QFileDialog::getOpenFileName(this, "Open File", "", "Parachute Files (*.ep)");
    if (!filename.isEmpty()) {
        QFile file(filename);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&file);
            QString line;
            QString message;
            int sectors = 0;
            int tracks = 0;
            while (!in.atEnd()) {
                line = in.readLine().trimmed();
                if (line == "[Message]") {
                    message = in.readLine().trimmed();
                } else if (line == "[Parameters]") {
                    while (!in.atEnd()) {
                        line = in.readLine().trimmed();
                        if (line.startsWith("Sectors=")) {
                            sectors = line.mid(QString("Sectors=").length()).toInt();
                        } else if (line.startsWith("Tracks=")) {
                            tracks = line.mid(QString("Tracks=").length()).toInt();
                        }
                    }
                }
            }
            ui->messageInput->setText(message);
            ui->spinSectors->setValue(sectors);
            ui->spinTracks->setValue(tracks);
            onSectorsOrTracksChanged();
            file.close();
            QMessageBox::information(this, "Open File", "File loaded successfully.");
        } else {
            QMessageBox::warning(this, "Open File", "Unable to open file for reading.");
        }
    }
}

void MainWindow::onExit() {
    close();
}

void MainWindow::onSectorsOrTracksChanged() {
    QString message = ui->messageInput->text();
    std::vector<int> encodedMessage = MessageEncoder::encodeMessage(message);
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
    ui->retranslateUi(this);
    qDebug() << "UI retranslated to language:" << languageManager->getCurrentLanguage();
}