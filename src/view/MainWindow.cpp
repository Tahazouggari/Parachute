#include "view/MainWindow.h"
#include "view/ui_MainWindow.h"
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
#include <QDir>
#include <QSplitter>
#include <QScrollArea>

// Inclusions depuis le dossier include
#include "presenter/ParachutePresenter.h"
#include "view/ParachuteView.h"
#include "view/BinaryWidget.h"
#include "view/HexView.h"
#include "utils/LanguageManager.h"
#include "model/MessageEncoder.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), languageManager(new LanguageManager(this)), mode10Enabled(false) {
    ui->setupUi(this);
    
    // Set English as the default language
    languageManager->switchLanguage("en");

    // Créer le splitter principal pour diviser l'écran
    QSplitter *mainSplitter = new QSplitter(Qt::Horizontal, ui->centralwidget);
    QVBoxLayout *mainLayout = qobject_cast<QVBoxLayout*>(ui->centralwidget->layout());
    if (mainLayout) {
        // Supprimer tous les widgets du layout principal
        while (QLayoutItem* item = mainLayout->takeAt(0)) {
            if (item->widget()) {
                item->widget()->setParent(nullptr);
            }
            delete item;
        }
        // Ajouter le splitter comme widget principal
        mainLayout->addWidget(mainSplitter);
    }

    // Panneau principal (70%) - Vue du parachute
    QWidget *mainPanel = new QWidget();
    QVBoxLayout *mainPanelLayout = new QVBoxLayout(mainPanel);
    
    // Titre et message en haut du panneau principal
    QLabel *titleLabel = new QLabel(tr("Parachute Encoder"));
    titleLabel->setStyleSheet("font-size: 18px; font-weight: bold;");
    titleLabel->setAlignment(Qt::AlignCenter);
    mainPanelLayout->addWidget(titleLabel);
    
    QGroupBox *messageBox = new QGroupBox(tr("Message"));
    QVBoxLayout *messageLayout = new QVBoxLayout(messageBox);
    QLabel *labelMessage = new QLabel(tr("Your message:"));
    ui->messageInput->setParent(nullptr); // Détacher du layout original
    messageLayout->addWidget(labelMessage);
    messageLayout->addWidget(ui->messageInput);
    mainPanelLayout->addWidget(messageBox);
    
    // TabWidget pour les différentes vues
    QTabWidget *viewTabs = new QTabWidget();
    mainPanelLayout->addWidget(viewTabs);
    
    // Créer et configurer la vue du parachute
    QWidget *parachuteViewTab = new QWidget();
    QVBoxLayout *parachuteLayout = new QVBoxLayout(parachuteViewTab);
    parachuteView = new ParachuteView(this);
    parachuteLayout->addWidget(parachuteView);
    viewTabs->addTab(parachuteViewTab, tr("Parachute View"));
    
    // Configurer la vue binaire
    QWidget *binaryViewTab = new QWidget();
    QVBoxLayout *binaryLayout = new QVBoxLayout(binaryViewTab);
    binaryWidget = new BinaryWidget(this);
    binaryLayout->addWidget(binaryWidget);
    viewTabs->addTab(binaryViewTab, tr("Binary View"));
    
    // Configurer la vue hexadécimale
    QWidget *hexViewTab = new QWidget();
    QVBoxLayout *hexLayout = new QVBoxLayout(hexViewTab);
    hexView = new HexView(this);
    hexLayout->addWidget(hexView);
    viewTabs->addTab(hexViewTab, tr("Hexadecimal View"));
    
    // Panneau latéral (30%) - Contrôles
    QWidget *controlPanel = new QWidget();
    QVBoxLayout *controlPanelLayout = new QVBoxLayout(controlPanel);
    
    // Ajouter un ScrollArea pour permettre de défiler si nécessaire
    QScrollArea *scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);
    QWidget *scrollContent = new QWidget();
    QVBoxLayout *scrollLayout = new QVBoxLayout(scrollContent);
    
    // Groupe des contrôles
    QGroupBox *controlsGroup = new QGroupBox(tr("Controls"));
    QVBoxLayout *controlsLayout = new QVBoxLayout(controlsGroup);
    
    // Préréglages de pistes (tracks)
    QHBoxLayout *tracksPresetLayout = new QHBoxLayout();
    trackPresetsLabel = new QLabel(tr("Track Presets:"));
    tracksPresetComboBox = new QComboBox();
    tracksPresetComboBox->setObjectName("tracksPresetComboBox");
    updateTracksPresets(tracksPresetComboBox);
    tracksPresetLayout->addWidget(trackPresetsLabel);
    tracksPresetLayout->addWidget(tracksPresetComboBox);
    controlsLayout->addLayout(tracksPresetLayout);
    
    // Préréglages de secteurs
    QHBoxLayout *sectorsPresetLayout = new QHBoxLayout();
    sectorPresetsLabel = new QLabel(tr("Sector Presets:"));
    sectorsPresetComboBox = new QComboBox();
    sectorsPresetComboBox->setObjectName("sectorsPresetComboBox");
    updateSectorsPresets(sectorsPresetComboBox, false);
    sectorsPresetLayout->addWidget(sectorPresetsLabel);
    sectorsPresetLayout->addWidget(sectorsPresetComboBox);
    controlsLayout->addLayout(sectorsPresetLayout);
    
    // Contrôles pour les secteurs
    QLabel *sectorsLabel = new QLabel(tr("Sectors:"));
    QSlider *sliderSectors = ui->sliderSectors;
    QSpinBox *spinSectors = ui->spinSectors;
    sliderSectors->setParent(nullptr);
    spinSectors->setParent(nullptr);
    
    QHBoxLayout *sectorsLayout = new QHBoxLayout();
    sectorsLayout->addWidget(sectorsLabel);
    sectorsLayout->addWidget(sliderSectors);
    sectorsLayout->addWidget(spinSectors);
    controlsLayout->addLayout(sectorsLayout);
    
    // Contrôles pour les pistes
    QLabel *tracksLabel = new QLabel(tr("Tracks:"));
    QSlider *sliderTracks = ui->sliderTracks;
    QSpinBox *spinTracks = ui->spinTracks;
    sliderTracks->setParent(nullptr);
    spinTracks->setParent(nullptr);
    
    QHBoxLayout *tracksLayout = new QHBoxLayout();
    tracksLayout->addWidget(tracksLabel);
    tracksLayout->addWidget(sliderTracks);
    tracksLayout->addWidget(spinTracks);
    controlsLayout->addLayout(tracksLayout);
    
    // Bouton d'exportation
    exportButton = ui->exportButton;
    exportButton->setParent(nullptr);
    controlsLayout->addWidget(exportButton);
    
    scrollLayout->addWidget(controlsGroup);
    
    // Groupe de personnalisation des couleurs
    colorCustomizationGroup = new QGroupBox(tr("Color Customization"));
    QVBoxLayout *colorLayout = new QVBoxLayout(colorCustomizationGroup);
    
    // Couleur d'arrière-plan
    QHBoxLayout *bgColorLayout = new QHBoxLayout();
    backgroundColorLabel = new QLabel(tr("Background Color:"));
    QPushButton *bgColorButton = ui->colorButton;
    bgColorButton->setParent(nullptr);
    bgColorLayout->addWidget(backgroundColorLabel);
    bgColorLayout->addWidget(bgColorButton);
    colorLayout->addLayout(bgColorLayout);
    
    // Couleur des bits à 1 (parachute)
    QHBoxLayout *parachuteColorLayout = new QHBoxLayout();
    colorBits1Label = new QLabel(tr("Color for Bits 1:"));
    parachuteButton = new QPushButton(tr("Choose"));
    parachuteColorLayout->addWidget(colorBits1Label);
    parachuteColorLayout->addWidget(parachuteButton);
    colorLayout->addLayout(parachuteColorLayout);
    
    // Couleur des bits à 0 (secteurs)
    QHBoxLayout *sectorColorLayout = new QHBoxLayout();
    colorBits0Label = new QLabel(tr("Color for Bits 0:"));
    sectorButton = new QPushButton(tr("Choose"));
    sectorColorLayout->addWidget(colorBits0Label);
    sectorColorLayout->addWidget(sectorButton);
    colorLayout->addLayout(sectorColorLayout);
    
    // Mode couleurs aléatoires
    randomColorModeCheckBox = new QCheckBox(tr("Random Color Mode"));
    colorLayout->addWidget(randomColorModeCheckBox);
    
    // Mode 10 (10 bits par caractère)
    mode10CheckBox = new QCheckBox(tr("Mode 10 (10 bits per character)"));
    mode10CheckBox->setObjectName("mode10CheckBox");
    colorLayout->addWidget(mode10CheckBox);
    
    scrollLayout->addWidget(colorCustomizationGroup);
    scrollArea->setWidget(scrollContent);
    controlPanelLayout->addWidget(scrollArea);
    
    // Ajouter les panneaux au splitter
    mainSplitter->addWidget(mainPanel);
    mainSplitter->addWidget(controlPanel);
    
    // Définir les proportions du splitter (70/30)
    mainSplitter->setStretchFactor(0, 7); // panneau principal (70%)
    mainSplitter->setStretchFactor(1, 3); // panneau de contrôle (30%)
    
    // Connexions pour les contrôles
    connect(parachuteButton, &QPushButton::clicked, this, &MainWindow::onParachuteColorChanged);
    connect(sectorButton, &QPushButton::clicked, this, &MainWindow::onSectorColorChanged);
    connect(randomColorModeCheckBox, &QCheckBox::toggled, this, &MainWindow::onRandomColorModeToggled);
    connect(mode10CheckBox, &QCheckBox::toggled, this, &MainWindow::onMode10Toggled);
    connect(sectorsPresetComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), 
            this, &MainWindow::onSectorsPresetSelected);
    connect(tracksPresetComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), 
            this, &MainWindow::onTracksPresetSelected);
    
    // Autres connexions
    connect(ui->messageInput, &QLineEdit::textChanged, this, &MainWindow::onMessageChanged);
    connect(bgColorButton, &QPushButton::clicked, this, &MainWindow::onBackgroundColorChanged);
    connect(exportButton, &QPushButton::clicked, this, &MainWindow::onExportImage);
    connect(ui->saveAction, &QAction::triggered, this, &MainWindow::onSaveFile);
    connect(ui->openAction, &QAction::triggered, this, &MainWindow::onOpenFile);
    connect(ui->exitAction, &QAction::triggered, this, &MainWindow::onExit);

    connect(sliderSectors, &QSlider::valueChanged, spinSectors, &QSpinBox::setValue);
    connect(spinSectors, QOverload<int>::of(&QSpinBox::valueChanged), sliderSectors, &QSlider::setValue);
    connect(sliderSectors, &QSlider::valueChanged, this, &MainWindow::onSectorsOrTracksChanged);

    connect(sliderTracks, &QSlider::valueChanged, spinTracks, &QSpinBox::setValue);
    connect(spinTracks, QOverload<int>::of(&QSpinBox::valueChanged), sliderTracks, &QSlider::setValue);
    connect(sliderTracks, &QSlider::valueChanged, this, &MainWindow::onSectorsOrTracksChanged);

    sliderSectors->setRange(7, 40);
    spinSectors->setRange(7, 40);
    sliderTracks->setRange(3, 10);
    spinTracks->setRange(3, 10);

    sliderSectors->setValue(7);
    spinSectors->setValue(7);
    sliderTracks->setValue(3);
    spinTracks->setValue(3);

    // Connexions pour les changements de langue
    connect(ui->actionEnglish, &QAction::triggered, this, &MainWindow::onLanguageEnglish);
    connect(ui->actionfrensh, &QAction::triggered, this, &MainWindow::onLanguageFrench);
    connect(ui->actionArabic, &QAction::triggered, this, &MainWindow::onLanguageArabic);

    connect(languageManager, &LanguageManager::languageChanged, this, &MainWindow::retranslateUi);
    
    // Définir une taille raisonnable pour la fenêtre
    resize(1200, 800);
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
    QColorDialog dialog(Qt::white, this);
    dialog.setWindowTitle(tr("Choose Background Color"));
    if (dialog.exec() == QDialog::Accepted) {
        QColor color = dialog.selectedColor();
        parachuteView->setBackgroundColor(color);
        binaryWidget->setBackgroundColor(color);
        hexView->setBackgroundColor(color);
    }
}

void MainWindow::onParachuteColorChanged() {
    QColorDialog dialog(Qt::red, this);
    dialog.setWindowTitle(tr("Choose Color for Bits 1"));
    if (dialog.exec() == QDialog::Accepted) {
        QColor color = dialog.selectedColor();
        parachuteView->setParachuteColor(color);
        binaryWidget->setBitOnColor(color);
    }
}

void MainWindow::onSectorColorChanged() {
    QColorDialog dialog(Qt::white, this);
    dialog.setWindowTitle(tr("Choose Color for Bits 0"));
    if (dialog.exec() == QDialog::Accepted) {
        QColor color = dialog.selectedColor();
        parachuteView->setSectorColor(color);
        binaryWidget->setBitOffColor(color);
    }
}

void MainWindow::onRandomColorModeToggled(bool checked) {
    parachuteView->setRandomColorMode(checked);
    binaryWidget->setRandomColorMode(checked);
}

void MainWindow::onMode10Toggled(bool checked) {
    mode10Enabled = checked;
    
    // Mettre à jour la vue parachute
    parachuteView->setMode10(checked);
    
    // Trouver les contrôles par nom
    QSlider *sliderSectors = findChild<QSlider*>("sliderSectors");
    QSpinBox *spinSectors = findChild<QSpinBox*>("spinSectors");
    
    if (sliderSectors && spinSectors) {
        int currentValue = spinSectors->value();
        
        // Ajuster la valeur au multiple de 10 le plus proche si nécessaire
        if (checked) {
            int newValue = (int)(std::ceil(currentValue / 10.0) * 10);
            if (currentValue != newValue) {
                spinSectors->setValue(newValue);
            }
        } else {
            // En mode 7, récupérer le nombre de secteurs original depuis la vue
            int originalSectors = parachuteView->getOriginalSectors();
            spinSectors->setValue(originalSectors);
            sliderSectors->setValue(originalSectors);
        }
        
        // Mettre à jour les préréglages disponibles
        updateSectorsPresets(sectorsPresetComboBox, checked);
        
        // Forcer une mise à jour des vues
        onSectorsOrTracksChanged();
    }
    
    // Mettre à jour le message pour qu'il soit encodé avec le nouveau mode
    onMessageChanged();
}

void MainWindow::onExportImage() {
    QString defaultPath = QDir::homePath() + "/parachute.png";
    QString fileName = QFileDialog::getSaveFileName(this, tr("Export Image"), defaultPath, tr("Images (*.png *.jpg *.bmp)"));
    if (!fileName.isEmpty()) {
        parachuteView->saveParachuteImage(fileName);
        QMessageBox::information(this, tr("Export Image"), tr("Parachute image saved successfully."));
    }
}

void MainWindow::onSaveFile() {
    QString filename = QFileDialog::getSaveFileName(this, tr("Save File"), "", tr("Parachute Files (*.ep)"));
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
            QMessageBox::information(this, tr("Save File"), tr("File saved successfully."));
        } else {
            QMessageBox::warning(this, tr("Save File"), tr("Unable to open file for writing."));
        }
    }
}

void MainWindow::onOpenFile() {
    QString filename = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("Parachute Files (*.ep)"));
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
            
            // Mettre à jour l'état du mode 10
            mode10Enabled = mode10;
            parachuteView->setMode10(mode10);
            
            if (mode10CheckBox) {
                mode10CheckBox->setChecked(mode10);
            }
            
            // Mettre à jour les préréglages de secteurs selon le mode
            if (sectorsPresetComboBox) {
                updateSectorsPresets(sectorsPresetComboBox, mode10);
            }
            
            // Mettre à jour la sélection de pistes
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
            
            // Trouver les contrôles par nom
            QSpinBox *spinSectors = findChild<QSpinBox*>("spinSectors");
            QSpinBox *spinTracks = findChild<QSpinBox*>("spinTracks");
            
            if (spinSectors && spinTracks) {
                ui->messageInput->setText(message);
                spinSectors->setValue(sectors);
                spinTracks->setValue(tracks);
                onSectorsOrTracksChanged(); // Cela mettra aussi à jour la sélection dans le ComboBox
            }
            
            file.close();
            QMessageBox::information(this, tr("Open File"), tr("File loaded successfully."));
        } else {
            QMessageBox::warning(this, tr("Open File"), tr("Unable to open file for reading."));
        }
    }
}

void MainWindow::onExit() {
    close();
}

void MainWindow::onSectorsOrTracksChanged() {
    // Récupérer la valeur actuelle des secteurs et pistes
    int sectors = 0;
    int tracks = 0;
    
    // Trouver les contrôles par nom
    QSlider *sliderSectors = findChild<QSlider*>("sliderSectors");
    QSlider *sliderTracks = findChild<QSlider*>("sliderTracks");
    QSpinBox *spinSectors = findChild<QSpinBox*>("spinSectors");
    QSpinBox *spinTracks = findChild<QSpinBox*>("spinTracks");
    
    if (sliderSectors && sliderTracks && spinSectors && spinTracks) {
        sectors = spinSectors->value();
        tracks = spinTracks->value();
    } else {
        qDebug() << "Controls not found!";
        return;
    }
    
    // Si le mode 10 est activé, le nombre de secteurs doit être un multiple de 10
    if (mode10Enabled) {
        int adjustedSectors = (int)(std::ceil(sectors / 10.0) * 10);
        if (sectors != adjustedSectors) {
            sectors = adjustedSectors;
            spinSectors->blockSignals(true);
            spinSectors->setValue(sectors);
            spinSectors->blockSignals(false);
            sliderSectors->blockSignals(true);
            sliderSectors->setValue(sectors);
            sliderSectors->blockSignals(false);
        }
    }
    
    // Mettre à jour le préréglage de secteurs sélectionné
    if (sectorsPresetComboBox) {
        int index = -1;
        // Chercher si la valeur actuelle correspond à un des préréglages
        for (int i = 0; i < sectorsPresetComboBox->count(); i++) {
            if (sectorsPresetComboBox->itemData(i).toInt() == sectors) {
                index = i;
                break;
            }
        }
        
        // Si on a trouvé un préréglage correspondant, le sélectionner
        if (index >= 0) {
            sectorsPresetComboBox->blockSignals(true);
            sectorsPresetComboBox->setCurrentIndex(index);
            sectorsPresetComboBox->blockSignals(false);
        }
    }
    
    // Mettre à jour le préréglage de pistes sélectionné
    if (tracksPresetComboBox) {
        int index = -1;
        // Chercher si la valeur actuelle correspond à un des préréglages
        for (int i = 0; i < tracksPresetComboBox->count(); i++) {
            if (tracksPresetComboBox->itemData(i).toInt() == tracks) {
                index = i;
                break;
            }
        }
        
        // Si on a trouvé un préréglage correspondant, le sélectionner
        if (index >= 0) {
            tracksPresetComboBox->blockSignals(true);
            tracksPresetComboBox->setCurrentIndex(index);
            tracksPresetComboBox->blockSignals(false);
        }
    }
    
    // On utilise le message existant (qui pourrait être vide)
    QString message = ui->messageInput->text();
    std::vector<int> encodedMessage = MessageEncoder::encodeMessage(message, mode10Enabled);
    
    // Mettre à jour la vue du parachute
    parachuteView->setParachuteData(sectors, tracks, encodedMessage);
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
    // Mettre à jour l'UI générée par Qt Designer
    ui->retranslateUi(this);
    
    // Groupe de personnalisation des couleurs
    colorCustomizationGroup->setTitle(tr("Color Customization"));
    backgroundColorLabel->setText(tr("Background Color:"));
    colorBits1Label->setText(tr("Color for Bits 1:"));
    colorBits0Label->setText(tr("Color for Bits 0:"));
    
    // Boutons de couleur
    parachuteButton->setText(tr("Choose"));
    sectorButton->setText(tr("Choose"));
    
    // CheckBoxes
    randomColorModeCheckBox->setText(tr("Random Color Mode"));
    mode10CheckBox->setText(tr("Mode 10 (10 bits per character)"));
    
    // Préréglages
    trackPresetsLabel->setText(tr("Track Presets:"));
    sectorPresetsLabel->setText(tr("Sector Presets:"));
    
    // Mettre à jour les textes dans les ComboBox avec blocage des signaux
    int currentSectors = sectorsPresetComboBox->currentData().toInt();
    sectorsPresetComboBox->blockSignals(true);
    updateSectorsPresets(sectorsPresetComboBox, mode10Enabled);
    // Restaurer la sélection
    for (int i = 0; i < sectorsPresetComboBox->count(); i++) {
        if (sectorsPresetComboBox->itemData(i).toInt() == currentSectors) {
            sectorsPresetComboBox->setCurrentIndex(i);
            break;
        }
    }
    sectorsPresetComboBox->blockSignals(false);
    
    int currentTracks = tracksPresetComboBox->currentData().toInt();
    tracksPresetComboBox->blockSignals(true);
    updateTracksPresets(tracksPresetComboBox);
    // Restaurer la sélection
    for (int i = 0; i < tracksPresetComboBox->count(); i++) {
        if (tracksPresetComboBox->itemData(i).toInt() == currentTracks) {
            tracksPresetComboBox->setCurrentIndex(i);
            break;
        }
    }
    tracksPresetComboBox->blockSignals(false);
    
    // Bouton d'exportation
    exportButton->setText(tr("Export Parachute Image"));
    
    qDebug() << "UI completely retranslated to language:" << languageManager->getCurrentLanguage();
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