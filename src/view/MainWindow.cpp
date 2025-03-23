#include "MainWindow.h"
#include "ui_MainWindow.h"
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
#include <QPainter>
#include <QMessageBox>
#include <QTextStream>
#include <QDebug>
#include <QAction>
#include <QCheckBox>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QComboBox>

// Chemins relatifs depuis le répertoire du projet
#include "../presenter/ParachutePresenter.h"
#include "ParachuteView.h"
#include "BinaryWidget.h"
#include "HexView.h"
#include "../utils/LanguageManager.h"
#include "../model/MessageEncoder.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), languageManager(new LanguageManager(this)), mode10Enabled(false) {
    ui->setupUi(this);
    
    // Set English as the default language
    languageManager->switchLanguage("en");

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

    // Ajouter des boutons et contrôles pour les nouvelles fonctionnalités de couleur
    QGroupBox *colorGroupBox = new QGroupBox(tr("Color Customization"), ui->centralwidget);
    QVBoxLayout *colorLayout = new QVBoxLayout(colorGroupBox);
    
    // Couleur d'arrière-plan
    QHBoxLayout *bgColorLayout = new QHBoxLayout();
    QLabel *bgColorLabel = new QLabel(tr("Background Color:"), colorGroupBox);
    QPushButton *bgColorButton = ui->colorButton; // Utiliser le bouton existant
    bgColorLayout->addWidget(bgColorLabel);
    bgColorLayout->addWidget(bgColorButton);
    colorLayout->addLayout(bgColorLayout);
    
    // Couleur des bits à 1 (parachute)
    QHBoxLayout *parachuteColorLayout = new QHBoxLayout();
    QLabel *parachuteColorLabel = new QLabel(tr("Color for Bits 1:"), colorGroupBox);
    QPushButton *parachuteColorButton = new QPushButton(tr("Choose"), colorGroupBox);
    parachuteColorLayout->addWidget(parachuteColorLabel);
    parachuteColorLayout->addWidget(parachuteColorButton);
    colorLayout->addLayout(parachuteColorLayout);
    
    // Couleur des bits à 0 (secteurs)
    QHBoxLayout *sectorColorLayout = new QHBoxLayout();
    QLabel *sectorColorLabel = new QLabel(tr("Color for Bits 0:"), colorGroupBox);
    QPushButton *sectorColorButton = new QPushButton(tr("Choose"), colorGroupBox);
    sectorColorLayout->addWidget(sectorColorLabel);
    sectorColorLayout->addWidget(sectorColorButton);
    colorLayout->addLayout(sectorColorLayout);
    
    // Mode couleurs aléatoires
    QCheckBox *randomColorCheckBox = new QCheckBox(tr("Random Color Mode"), colorGroupBox);
    colorLayout->addWidget(randomColorCheckBox);
    
    // Ajouter le mode 10
    QCheckBox *mode10CheckBox = new QCheckBox(tr("Mode 10 (10 bits per character)"), colorGroupBox);
    mode10CheckBox->setObjectName("mode10CheckBox");
    colorLayout->addWidget(mode10CheckBox);
    
    // Ajouter les contrôles de couleur au layout principal
    QVBoxLayout *mainLayout = qobject_cast<QVBoxLayout*>(ui->centralwidget->layout());
    if (mainLayout) {
        mainLayout->insertWidget(mainLayout->indexOf(ui->exportButton), colorGroupBox);
    }

    // Ajouter un ComboBox pour sélectionner des multiples de 7 ou 10 selon le mode
    QHBoxLayout *sectorsPresetLayout = new QHBoxLayout();
    QLabel *sectorsPresetLabel = new QLabel(tr("Sector Presets:"), ui->centralwidget);
    QComboBox *sectorsPresetComboBox = new QComboBox(ui->centralwidget);
    sectorsPresetComboBox->setObjectName("sectorsPresetComboBox");
    
    // Remplir avec des multiples de 7 par défaut (mode standard)
    updateSectorsPresets(sectorsPresetComboBox, false);
    
    sectorsPresetLayout->addWidget(sectorsPresetLabel);
    sectorsPresetLayout->addWidget(sectorsPresetComboBox);
    
    // Insérer le layout des préréglages de secteurs au-dessus des contrôles de secteurs existants
    QVBoxLayout *sectorsControlsLayout = qobject_cast<QVBoxLayout*>(ui->sliderSectors->parentWidget()->layout());
    if (sectorsControlsLayout) {
        sectorsControlsLayout->insertLayout(0, sectorsPresetLayout);
    }
    
    // Ajouter un ComboBox pour les pistes (tracks)
    QHBoxLayout *tracksPresetLayout = new QHBoxLayout();
    QLabel *tracksPresetLabel = new QLabel(tr("Track Presets:"), ui->centralwidget);
    QComboBox *tracksPresetComboBox = new QComboBox(ui->centralwidget);
    tracksPresetComboBox->setObjectName("tracksPresetComboBox");
    
    // Remplir avec les options de pistes
    updateTracksPresets(tracksPresetComboBox);
    
    tracksPresetLayout->addWidget(tracksPresetLabel);
    tracksPresetLayout->addWidget(tracksPresetComboBox);
    
    // Insérer le layout des préréglages de pistes au-dessus des contrôles de pistes existants
    QVBoxLayout *tracksControlsLayout = qobject_cast<QVBoxLayout*>(ui->sliderTracks->parentWidget()->layout());
    if (tracksControlsLayout) {
        tracksControlsLayout->insertLayout(0, tracksPresetLayout);
    }
    
    // Connecter les signaux des nouveaux contrôles
    connect(parachuteColorButton, &QPushButton::clicked, this, &MainWindow::onParachuteColorChanged);
    connect(sectorColorButton, &QPushButton::clicked, this, &MainWindow::onSectorColorChanged);
    connect(randomColorCheckBox, &QCheckBox::toggled, this, &MainWindow::onRandomColorModeToggled);
    connect(mode10CheckBox, &QCheckBox::toggled, this, &MainWindow::onMode10Toggled);
    connect(sectorsPresetComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), 
            this, &MainWindow::onSectorsPresetSelected);
    connect(tracksPresetComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), 
            this, &MainWindow::onTracksPresetSelected);
    
    // Autres connexions existantes
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

    ui->sliderSectors->setRange(7, 40);
    ui->spinSectors->setRange(7, 40);
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
    std::vector<int> encodedMessage = MessageEncoder::encodeMessage(message, mode10Enabled);
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

void MainWindow::onParachuteColorChanged() {
    QColor color = QColorDialog::getColor(Qt::red, this, "Choose Color for Bits 1");
    if (color.isValid()) {
        parachuteView->setParachuteColor(color);
        binaryWidget->setBitOnColor(color);
    }
}

void MainWindow::onSectorColorChanged() {
    QColor color = QColorDialog::getColor(Qt::white, this, "Choose Color for Bits 0");
    if (color.isValid()) {
        parachuteView->setSectorColor(color);
        binaryWidget->setBitOffColor(color);
    }
}

void MainWindow::onRandomColorModeToggled(bool checked) {
    parachuteView->setRandomColorMode(checked);
    binaryWidget->setRandomColorMode(checked);
}

void MainWindow::onMode10Toggled(bool checked) {
    // Mettre à jour le mode
    mode10Enabled = checked;
    
    // Mettre à jour la vue parachute
    parachuteView->setMode10(checked);
    
    // Mettre à jour les préréglages de secteurs
    QComboBox* comboBox = findChild<QComboBox*>("sectorsPresetComboBox");
    updateSectorsPresets(comboBox, checked);
    
    // Si le mode 10 est activé, le nombre de secteurs doit être un multiple de 10
    if (mode10Enabled) {
        int sectors = ui->spinSectors->value();
        int adjustedSectors = (int)(std::ceil(sectors / 10.0) * 10);
        if (sectors != adjustedSectors) {
            ui->spinSectors->setValue(adjustedSectors);
            ui->sliderSectors->setValue(adjustedSectors);
        }
    } else {
        // En mode 7, récupérer le nombre de secteurs original depuis la vue
        int originalSectors = parachuteView->getOriginalSectors();
        ui->spinSectors->setValue(originalSectors);
        ui->sliderSectors->setValue(originalSectors);
    }
    
    // Mettre à jour le message pour qu'il soit encodé avec le nouveau mode
    onMessageChanged();
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
            out << "[Message]" << Qt::endl;
            out << ui->messageInput->text() << Qt::endl;
            out << "[Parameters]" << Qt::endl;
            out << "Sectors=" << ui->spinSectors->value() << Qt::endl;
            out << "Tracks=" << ui->spinTracks->value() << Qt::endl;
            out << "Mode10=" << (mode10Enabled ? "true" : "false") << Qt::endl;
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
            bool mode10 = false;
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
                        } else if (line.startsWith("Mode10=")) {
                            QString mode10Str = line.mid(QString("Mode10=").length());
                            mode10 = (mode10Str.toLower() == "true");
                        }
                    }
                }
            }
            // Mettre à jour le mode 10 avant de modifier les secteurs
            mode10Enabled = mode10;
            parachuteView->setMode10(mode10);
            
            // Mettre à jour les contrôles de l'interface
            QCheckBox *mode10CheckBox = findChild<QCheckBox*>("mode10CheckBox");
            if (mode10CheckBox) {
                mode10CheckBox->setChecked(mode10);
            }
            
            // Mettre à jour les préréglages de secteurs selon le mode
            QComboBox *sectorsPresetComboBox = findChild<QComboBox*>("sectorsPresetComboBox");
            if (sectorsPresetComboBox) {
                updateSectorsPresets(sectorsPresetComboBox, mode10);
            }
            
            // Mettre à jour la sélection de pistes
            QComboBox *tracksPresetComboBox = findChild<QComboBox*>("tracksPresetComboBox");
            if (tracksPresetComboBox) {
                // Trouver l'index correspondant à la valeur de pistes chargée
                int index = -1;
                for (int i = 0; i < tracksPresetComboBox->count(); i++) {
                    if (tracksPresetComboBox->itemData(i).toInt() == tracks) {
                        index = i;
                        break;
                    }
                }
                if (index >= 0) {
                    tracksPresetComboBox->setCurrentIndex(index);
                }
            }
            
            ui->messageInput->setText(message);
            ui->spinSectors->setValue(sectors);
            ui->spinTracks->setValue(tracks);
            onSectorsOrTracksChanged(); // Cela mettra aussi à jour la sélection dans le ComboBox
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
    int sectors = ui->spinSectors->value();
    
    // Si le mode 10 est activé, le nombre de secteurs doit être un multiple de 10
    if (mode10Enabled) {
        int adjustedSectors = (int)(std::ceil(sectors / 10.0) * 10);
        if (sectors != adjustedSectors) {
            sectors = adjustedSectors;
            ui->spinSectors->blockSignals(true);
            ui->spinSectors->setValue(sectors);
            ui->spinSectors->blockSignals(false);
            ui->sliderSectors->blockSignals(true);
            ui->sliderSectors->setValue(sectors);
            ui->sliderSectors->blockSignals(false);
        }
    }
    
    // Mettre à jour la sélection dans le ComboBox si la valeur correspond à un des préréglages
    QComboBox* comboBox = findChild<QComboBox*>("sectorsPresetComboBox");
    if (comboBox) {
        int index = -1;
        // Chercher si la valeur actuelle correspond à un des préréglages
        for (int i = 0; i < comboBox->count(); i++) {
            if (comboBox->itemData(i).toInt() == sectors) {
                index = i;
                break;
            }
        }
        
        // Si on a trouvé un préréglage correspondant, le sélectionner
        if (index >= 0) {
            comboBox->blockSignals(true);
            comboBox->setCurrentIndex(index);
            comboBox->blockSignals(false);
        }
    }
    
    // On utilise le message existant (qui pourrait être vide)
    QString message = ui->messageInput->text();
    std::vector<int> encodedMessage = MessageEncoder::encodeMessage(message, mode10Enabled);
    
    // Mettre à jour la vue du parachute
    parachuteView->setParachuteData(sectors, ui->spinTracks->value(), encodedMessage);
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

// Méthode utilitaire pour mettre à jour les préréglages dans le ComboBox
void MainWindow::updateSectorsPresets(QComboBox* comboBox, bool mode10) {
    if (!comboBox) return;
    
    comboBox->clear();
    
    if (mode10) {
        // Remplir avec des multiples de 10
        comboBox->addItem(tr("10 sectors"), 10);
        comboBox->addItem(tr("20 sectors"), 20);
        comboBox->addItem(tr("30 sectors"), 30);
        comboBox->addItem(tr("40 sectors"), 40);
    } else {
        // Remplir avec des multiples de 7
        comboBox->addItem(tr("7 sectors"), 7);
        comboBox->addItem(tr("14 sectors"), 14);
        comboBox->addItem(tr("21 sectors"), 21);
        comboBox->addItem(tr("28 sectors"), 28);
    }
}

// Méthode utilitaire pour mettre à jour les préréglages de pistes
void MainWindow::updateTracksPresets(QComboBox* comboBox) {
    if (!comboBox) return;
    
    comboBox->clear();
    
    // Ajouter les options de pistes
    comboBox->addItem(tr("3 tracks"), 3);
    comboBox->addItem(tr("4 tracks"), 4);
    comboBox->addItem(tr("5 tracks"), 5);
    comboBox->addItem(tr("6 tracks"), 6);
    comboBox->addItem(tr("7 tracks"), 7);
    comboBox->addItem(tr("8 tracks"), 8);
    comboBox->addItem(tr("9 tracks"), 9);
    comboBox->addItem(tr("10 tracks"), 10);
}

// Slot pour gérer la sélection d'un préréglage
void MainWindow::onSectorsPresetSelected(int index) {
    QComboBox* comboBox = qobject_cast<QComboBox*>(sender());
    if (!comboBox) return;
    
    // Obtenir la valeur sélectionnée (le nombre de secteurs)
    int selectedSectors = comboBox->itemData(index).toInt();
    
    // Mettre à jour le slider et le spinbox
    ui->spinSectors->setValue(selectedSectors);
    ui->sliderSectors->setValue(selectedSectors);
    
    // Mettre à jour le parachute
    onSectorsOrTracksChanged();
}

// Slot pour gérer la sélection d'un préréglage de pistes
void MainWindow::onTracksPresetSelected(int index) {
    QComboBox* comboBox = qobject_cast<QComboBox*>(sender());
    if (!comboBox) return;
    
    // Obtenir la valeur sélectionnée (le nombre de pistes)
    int selectedTracks = comboBox->itemData(index).toInt();
    
    // Mettre à jour le slider et le spinbox
    ui->spinTracks->setValue(selectedTracks);
    ui->sliderTracks->setValue(selectedTracks);
    
    // Mettre à jour le parachute
    onSectorsOrTracksChanged();
}