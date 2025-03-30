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

#include "presenter/ParachutePresenter.h"
#include "view/ParachuteView.h"
#include "view/BinaryWidget.h"
#include "view/HexView.h"
#include "utils/LanguageManager.h"
#include "model/MessageEncoder.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), languageManager(new LanguageManager(this)), mode10Enabled(false) {
    ui->setupUi(this);
    
    languageManager->switchLanguage("en");

    QSplitter *mainSplitter = new QSplitter(Qt::Horizontal, ui->centralwidget);
    QVBoxLayout *mainLayout = qobject_cast<QVBoxLayout*>(ui->centralwidget->layout());
    if (mainLayout) {
        while (QLayoutItem* item = mainLayout->takeAt(0)) {
            if (item->widget()) {
                item->widget()->setParent(nullptr);
            }
            delete item;
        }
        mainLayout->addWidget(mainSplitter);
    }

    QWidget *mainPanel = new QWidget();
    QVBoxLayout *mainPanelLayout = new QVBoxLayout(mainPanel);
    
    QLabel *titleLabel = new QLabel(tr("Perseverance Rover"));
    titleLabel->setStyleSheet("font-size: 18px; font-weight: bold;");
    titleLabel->setAlignment(Qt::AlignCenter);
    mainPanelLayout->addWidget(titleLabel);
    
    QGroupBox *messageBox = new QGroupBox(tr("Message"));
    QVBoxLayout *messageLayout = new QVBoxLayout(messageBox);
    QLabel *labelMessage = new QLabel(tr("Your message:"));
    ui->messageInput->setParent(nullptr);
    messageLayout->addWidget(labelMessage);
    messageLayout->addWidget(ui->messageInput);
    mainPanelLayout->addWidget(messageBox);
    
    QTabWidget *viewTabs = new QTabWidget();
    mainPanelLayout->addWidget(viewTabs);
    
    QWidget *parachuteViewTab = new QWidget();
    QVBoxLayout *parachuteLayout = new QVBoxLayout(parachuteViewTab);
    parachuteView = new ParachuteView(this);
    parachuteLayout->addWidget(parachuteView);
    viewTabs->addTab(parachuteViewTab, tr("Parachute View"));
    
    QWidget *binaryViewTab = new QWidget();
    QVBoxLayout *binaryLayout = new QVBoxLayout(binaryViewTab);
    binaryWidget = new BinaryWidget(this);
    binaryLayout->addWidget(binaryWidget);
    viewTabs->addTab(binaryViewTab, tr("Binary View"));
    
    QWidget *hexViewTab = new QWidget();
    QVBoxLayout *hexLayout = new QVBoxLayout(hexViewTab);
    hexView = new HexView(this);
    hexLayout->addWidget(hexView);
    viewTabs->addTab(hexViewTab, tr("Hexadecimal View"));
    
    QWidget *controlPanel = new QWidget();
    QVBoxLayout *controlPanelLayout = new QVBoxLayout(controlPanel);
    
    QScrollArea *scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);
    QWidget *scrollContent = new QWidget();
    QVBoxLayout *scrollLayout = new QVBoxLayout(scrollContent);
    
    QGroupBox *controlsGroup = new QGroupBox(tr("Controls"));
    QVBoxLayout *controlsLayout = new QVBoxLayout(controlsGroup);
    
    QHBoxLayout *tracksPresetLayout = new QHBoxLayout();
    trackPresetsLabel = new QLabel(tr("Track Presets:"));
    tracksPresetComboBox = new QComboBox();
    tracksPresetComboBox->setObjectName("tracksPresetComboBox");
    updateTracksPresets(tracksPresetComboBox);
    tracksPresetLayout->addWidget(trackPresetsLabel);
    tracksPresetLayout->addWidget(tracksPresetComboBox);
    controlsLayout->addLayout(tracksPresetLayout);
    
    QHBoxLayout *sectorsPresetLayout = new QHBoxLayout();
    sectorPresetsLabel = new QLabel(tr("Sector Presets:"));
    sectorsPresetComboBox = new QComboBox();
    sectorsPresetComboBox->setObjectName("sectorsPresetComboBox");
    updateSectorsPresets(sectorsPresetComboBox, false);
    sectorsPresetLayout->addWidget(sectorPresetsLabel);
    sectorsPresetLayout->addWidget(sectorsPresetComboBox);
    controlsLayout->addLayout(sectorsPresetLayout);
    
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
    
    exportButton = ui->exportButton;
    exportButton->setParent(nullptr);
    controlsLayout->addWidget(exportButton);
    
    scrollLayout->addWidget(controlsGroup);
    
    colorCustomizationGroup = new QGroupBox(tr("Color Customization"));
    QVBoxLayout *colorLayout = new QVBoxLayout(colorCustomizationGroup);
    
    QHBoxLayout *bgColorLayout = new QHBoxLayout();
    backgroundColorLabel = new QLabel(tr("Background Color:"));
    QPushButton *bgColorButton = ui->colorButton;
    bgColorButton->setParent(nullptr);
    bgColorLayout->addWidget(backgroundColorLabel);
    bgColorLayout->addWidget(bgColorButton);
    colorLayout->addLayout(bgColorLayout);
    
    QHBoxLayout *parachuteColorLayout = new QHBoxLayout();
    colorBits1Label = new QLabel(tr("Color for Bits 1:"));
    parachuteButton = new QPushButton(tr("Choose"));
    parachuteColorLayout->addWidget(colorBits1Label);
    parachuteColorLayout->addWidget(parachuteButton);
    colorLayout->addLayout(parachuteColorLayout);
    
    QHBoxLayout *sectorColorLayout = new QHBoxLayout();
    colorBits0Label = new QLabel(tr("Color for Bits 0:"));
    sectorButton = new QPushButton(tr("Choose"));
    sectorColorLayout->addWidget(colorBits0Label);
    sectorColorLayout->addWidget(sectorButton);
    colorLayout->addLayout(sectorColorLayout);
    
    QHBoxLayout *backgroundImageLayout = new QHBoxLayout();
    QLabel *backgroundImageLabel = new QLabel(tr("Background Image:"));
    backgroundImageButton = new QPushButton(tr("Select Image"));
    clearBackgroundImageButton = new QPushButton(tr("Clear Image"));
    clearBackgroundImageButton->setEnabled(false);
    
    backgroundImageLayout->addWidget(backgroundImageLabel);
    backgroundImageLayout->addWidget(backgroundImageButton);
    backgroundImageLayout->addWidget(clearBackgroundImageButton);
    colorLayout->addLayout(backgroundImageLayout);
    
    randomColorModeCheckBox = new QCheckBox(tr("Random Color Mode"));
    colorLayout->addWidget(randomColorModeCheckBox);
    
    mode10CheckBox = new QCheckBox(tr("Mode 10 (10 bits per character)"));
    mode10CheckBox->setObjectName("mode10CheckBox");
    colorLayout->addWidget(mode10CheckBox);
    
    QHBoxLayout *referenceCharLayout = new QHBoxLayout();
    referenceCharLabel = new QLabel(tr("Reference Character:"));
    referenceCharInput = new QLineEdit("@");
    referenceCharInput->setMaxLength(1);
    referenceCharInput->setFixedWidth(40);
    referenceCharButton = new QPushButton(tr("Apply"));
    
    referenceCharLayout->addWidget(referenceCharLabel);
    referenceCharLayout->addWidget(referenceCharInput);
    referenceCharLayout->addWidget(referenceCharButton);
    colorLayout->addLayout(referenceCharLayout);
    
    scrollLayout->addWidget(colorCustomizationGroup);
    scrollArea->setWidget(scrollContent);
    controlPanelLayout->addWidget(scrollArea);
    
    mainSplitter->addWidget(mainPanel);
    mainSplitter->addWidget(controlPanel);
    
    mainSplitter->setStretchFactor(0, 7);
    mainSplitter->setStretchFactor(1, 3);
    
    connect(parachuteButton, &QPushButton::clicked, this, &MainWindow::onParachuteColorChanged);
    connect(sectorButton, &QPushButton::clicked, this, &MainWindow::onSectorColorChanged);
    connect(randomColorModeCheckBox, &QCheckBox::toggled, this, &MainWindow::onRandomColorModeToggled);
    connect(mode10CheckBox, &QCheckBox::toggled, this, &MainWindow::onMode10Toggled);
    connect(sectorsPresetComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), 
            this, &MainWindow::onSectorsPresetSelected);
    connect(tracksPresetComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), 
            this, &MainWindow::onTracksPresetSelected);
    
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

    connect(ui->actionEnglish, &QAction::triggered, this, &MainWindow::onLanguageEnglish);
    connect(ui->actionfrensh, &QAction::triggered, this, &MainWindow::onLanguageFrench);

    connect(languageManager, &LanguageManager::languageChanged, this, &MainWindow::retranslateUi);
    
    resize(1200, 800);

    connect(backgroundImageButton, &QPushButton::clicked, this, &MainWindow::onBackgroundImageSelect);
    connect(clearBackgroundImageButton, &QPushButton::clicked, this, &MainWindow::onClearBackgroundImage);

    connect(referenceCharButton, &QPushButton::clicked, this, &MainWindow::onReferenceCharChanged);
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
    
    parachuteView->setMode10(checked);
    
    QSlider *sliderSectors = findChild<QSlider*>("sliderSectors");
    QSpinBox *spinSectors = findChild<QSpinBox*>("spinSectors");
    
    if (sliderSectors && spinSectors) {
        int currentValue = spinSectors->value();
        
        if (checked) {
            int newValue = (int)(std::ceil(currentValue / 10.0) * 10);
            if (currentValue != newValue) {
                spinSectors->setValue(newValue);
            }
        } else {
            int originalSectors = parachuteView->getOriginalSectors();
            spinSectors->setValue(originalSectors);
            sliderSectors->setValue(originalSectors);
        }
        
        updateSectorsPresets(sectorsPresetComboBox, checked);
        
        onSectorsOrTracksChanged();
    }
    
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
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Parachute"), QDir::homePath(), tr("Parachute Files (*.ep)"));
    if (!fileName.isEmpty()) {
        if (!fileName.endsWith(".ep", Qt::CaseInsensitive)) {
            fileName += ".ep";
        }
        
        QFile file(fileName);
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&file);
            out << ui->messageInput->text() << "\n";
            out << ui->spinSectors->value() << "\n";
            out << ui->spinTracks->value() << "\n";
            out << (mode10Enabled ? "1" : "0") << "\n";
            out << (backgroundImagePath.isEmpty() ? "no_image" : backgroundImagePath) << "\n";
            out << MessageEncoder::getReferenceCharacter().unicode() << "\n";
            file.close();
            QMessageBox::information(this, tr("File Saved"), tr("Parachute parameters saved successfully."));
        } else {
            QMessageBox::warning(this, tr("Error"), tr("Unable to save file."));
        }
    }
}

void MainWindow::onOpenFile() {
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Parachute"), QDir::homePath(), tr("Parachute Files (*.ep)"));
    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&file);
            QString message = in.readLine();
            ui->messageInput->setText(message);
            
            bool ok;
            int sectors = in.readLine().toInt(&ok);
            if (ok) ui->spinSectors->setValue(sectors);
            
            int tracks = in.readLine().toInt(&ok);
            if (ok) ui->spinTracks->setValue(tracks);
            
            if (!in.atEnd()) {
                QString mode10Str = in.readLine();
                bool mode10 = (mode10Str == "1");
                
                mode10Enabled = mode10;
                findChild<QCheckBox*>("mode10CheckBox")->setChecked(mode10);
                
                parachuteView->setMode10(mode10);
                
                updateSectorsPresets(sectorsPresetComboBox, mode10);
            }
            
            if (!in.atEnd()) {
                QString imagePath = in.readLine().trimmed();
                if (imagePath != "no_image") {
                    backgroundImagePath = imagePath;
                    parachuteView->setBackgroundImage(imagePath);
                    clearBackgroundImageButton->setEnabled(true);
                } else {
                    backgroundImagePath = "";
                    parachuteView->clearBackgroundImage();
                    clearBackgroundImageButton->setEnabled(false);
                }
            }
            
            if (!in.atEnd()) {
                int refCharCode = in.readLine().toInt(&ok);
                if (ok) {
                    QChar refChar(refCharCode);
                    MessageEncoder::setReferenceCharacter(refChar);
                    referenceCharInput->setText(refChar);
                }
            }
            
            file.close();
            onMessageChanged();
            QMessageBox::information(this, tr("File Loaded"), tr("Parachute parameters loaded successfully."));
        } else {
            QMessageBox::warning(this, tr("Error"), tr("Unable to open file."));
        }
    }
}

void MainWindow::onExit() {
    close();
}

void MainWindow::onSectorsOrTracksChanged() {
    int sectors = 0;
    int tracks = 0;
    
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
    
    if (sectorsPresetComboBox) {
        int index = -1;
        for (int i = 0; i < sectorsPresetComboBox->count(); i++) {
            if (sectorsPresetComboBox->itemData(i).toInt() == sectors) {
                index = i;
                break;
            }
        }
        
        if (index >= 0) {
            sectorsPresetComboBox->blockSignals(true);
            sectorsPresetComboBox->setCurrentIndex(index);
            sectorsPresetComboBox->blockSignals(false);
        }
    }
    
    if (tracksPresetComboBox) {
        int index = -1;
        for (int i = 0; i < tracksPresetComboBox->count(); i++) {
            if (tracksPresetComboBox->itemData(i).toInt() == tracks) {
                index = i;
                break;
            }
        }
        
        if (index >= 0) {
            tracksPresetComboBox->blockSignals(true);
            tracksPresetComboBox->setCurrentIndex(index);
            tracksPresetComboBox->blockSignals(false);
        }
    }
    
    QString message = ui->messageInput->text();
    std::vector<int> encodedMessage = MessageEncoder::encodeMessage(message, mode10Enabled);
    
    parachuteView->setParachuteData(sectors, tracks, encodedMessage);
}

void MainWindow::onLanguageEnglish() {
    languageManager->switchLanguage("en");
}

void MainWindow::onLanguageFrench() {
    languageManager->switchLanguage("fr");
}

void MainWindow::retranslateUi() {
    ui->retranslateUi(this);
    
    this->setWindowTitle(tr("Perseverance Rover"));
    
    QTabWidget* viewTabs = findChild<QTabWidget*>();
    if (viewTabs) {
        for (int i = 0; i < viewTabs->count(); i++) {
            if (i == 0) viewTabs->setTabText(i, tr("Parachute View"));
            else if (i == 1) viewTabs->setTabText(i, tr("Binary View"));  
            else if (i == 2) viewTabs->setTabText(i, tr("Hexadecimal View"));
        }
    }
    
    foreach (QGroupBox* group, findChildren<QGroupBox*>()) {
        QString title = group->title().toLower();
        
        if (title.contains("message")) {
            group->setTitle(tr("Message"));
        } 
        else if (title.contains("control") || title.contains("contrôle")) {
            group->setTitle(tr("Controls"));
        }
        else if (title.contains("color") || title.contains("couleur")) {
            group->setTitle(tr("Color Customization"));
        }
    }
    
    foreach (QLabel* label, findChildren<QLabel*>()) {
        QString text = label->text();
        
        if (text.contains("Sector") || text.contains("Secteur")) {
            if (text.contains("Preset") || text.contains("Préréglage")) {
                label->setText(tr("Sector Presets:"));
            } else {
                label->setText(tr("Sectors:"));
            }
        }
        else if (text.contains("Track") || text.contains("Piste")) {
            if (text.contains("Preset") || text.contains("Préréglage")) {
                label->setText(tr("Track Presets:"));
            } else {
                label->setText(tr("Tracks:"));
            }
        }
        else if (text.contains("message") || text.contains("Message")) {
            label->setText(tr("Your message:"));
        }
        else if ((text.contains("Background") || text.contains("Arrière")) && text.contains("Color")) {
            label->setText(tr("Background Color:"));
        }
        else if ((text.contains("Background") || text.contains("Arrière")) && text.contains("Image")) {
            label->setText(tr("Background Image:"));
        }
        else if (text.contains("Bits 1")) {
            label->setText(tr("Color for Bits 1:"));
        }
        else if (text.contains("Bits 0")) {
            label->setText(tr("Color for Bits 0:"));
        }
        else if (text.contains("Reference") || text.contains("référence")) {
            label->setText(tr("Reference Character:"));
        }
    }
    
    foreach (QPushButton* button, findChildren<QPushButton*>()) {
        QString text = button->text();
        
        if (text.contains("Choose") || text.contains("Choisir")) {
            if (text.contains("Background") || text.contains("Arrière")) {
                button->setText(tr("Choose Background Color"));
            } else {
                button->setText(tr("Choose"));
            }
        }
        else if (text.contains("Select") || text.contains("Sélectionner")) {
            button->setText(tr("Select Image"));
        }
        else if (text.contains("Clear") || text.contains("Effacer") || text.contains("Supprimer")) {
            button->setText(tr("Clear Image"));
        }
        else if (text.contains("Apply") || text.contains("Appliquer")) {
            button->setText(tr("Apply"));
        }
        else if (text.contains("Export") || text.contains("Exporter")) {
            button->setText(tr("Export Parachute Image"));
        }
    }
    
    foreach (QCheckBox* checkBox, findChildren<QCheckBox*>()) {
        QString text = checkBox->text();
        
        if (text.contains("Random") || text.contains("Aléatoire")) {
            checkBox->setText(tr("Random Color Mode"));
        }
        else if (text.contains("Mode 10")) {
            checkBox->setText(tr("Mode 10 (10 bits per character)"));
        }
    }
    
    if (sectorsPresetComboBox) {
        int currentSectors = sectorsPresetComboBox->currentData().toInt();
        sectorsPresetComboBox->blockSignals(true);
        sectorsPresetComboBox->clear();
        updateSectorsPresets(sectorsPresetComboBox, mode10Enabled);
        
        for (int i = 0; i < sectorsPresetComboBox->count(); i++) {
            if (sectorsPresetComboBox->itemData(i).toInt() == currentSectors) {
                sectorsPresetComboBox->setCurrentIndex(i);
                break;
            }
        }
        sectorsPresetComboBox->blockSignals(false);
    }
    
    if (tracksPresetComboBox) {
        int currentTracks = tracksPresetComboBox->currentData().toInt();
        tracksPresetComboBox->blockSignals(true);
        tracksPresetComboBox->clear();
        updateTracksPresets(tracksPresetComboBox);
        
        for (int i = 0; i < tracksPresetComboBox->count(); i++) {
            if (tracksPresetComboBox->itemData(i).toInt() == currentTracks) {
                tracksPresetComboBox->setCurrentIndex(i);
                break;
            }
        }
        tracksPresetComboBox->blockSignals(false);
    }
    
    if (colorCustomizationGroup) colorCustomizationGroup->setTitle(tr("Color Customization"));
    if (backgroundColorLabel) backgroundColorLabel->setText(tr("Background Color:"));
    if (colorBits1Label) colorBits1Label->setText(tr("Color for Bits 1:"));
    if (colorBits0Label) colorBits0Label->setText(tr("Color for Bits 0:"));
    if (parachuteButton) parachuteButton->setText(tr("Choose"));
    if (sectorButton) sectorButton->setText(tr("Choose"));
    if (backgroundImageButton) backgroundImageButton->setText(tr("Select Image"));
    if (clearBackgroundImageButton) clearBackgroundImageButton->setText(tr("Clear Image"));
    if (randomColorModeCheckBox) randomColorModeCheckBox->setText(tr("Random Color Mode"));
    if (mode10CheckBox) mode10CheckBox->setText(tr("Mode 10 (10 bits per character)"));
    if (trackPresetsLabel) trackPresetsLabel->setText(tr("Track Presets:"));
    if (sectorPresetsLabel) sectorPresetsLabel->setText(tr("Sector Presets:"));
    if (exportButton) exportButton->setText(tr("Export Parachute Image"));
    if (referenceCharLabel) referenceCharLabel->setText(tr("Reference Character:"));
    if (referenceCharButton) referenceCharButton->setText(tr("Apply"));
    
    qDebug() << "UI completely retranslated to language:" << languageManager->getCurrentLanguage();
}

void MainWindow::updateSectorsPresets(QComboBox* comboBox, bool mode10) {
    if (!comboBox) return;
    
    comboBox->clear();
    
    if (mode10) {
        comboBox->addItem(tr("10 sectors"), 10);
        comboBox->addItem(tr("20 sectors"), 20);
        comboBox->addItem(tr("30 sectors"), 30);
        comboBox->addItem(tr("40 sectors"), 40);
    } else {
        comboBox->addItem(tr("7 sectors"), 7);
        comboBox->addItem(tr("14 sectors"), 14);
        comboBox->addItem(tr("21 sectors"), 21);
        comboBox->addItem(tr("28 sectors"), 28);
    }
}

void MainWindow::updateTracksPresets(QComboBox* comboBox) {
    if (!comboBox) return;
    
    comboBox->clear();
    
    comboBox->addItem(tr("3 tracks"), 3);
    comboBox->addItem(tr("4 tracks"), 4);
    comboBox->addItem(tr("5 tracks"), 5);
    comboBox->addItem(tr("6 tracks"), 6);
    comboBox->addItem(tr("7 tracks"), 7);
    comboBox->addItem(tr("8 tracks"), 8);
    comboBox->addItem(tr("9 tracks"), 9);
    comboBox->addItem(tr("10 tracks"), 10);
}

void MainWindow::onSectorsPresetSelected(int index) {
    QComboBox* comboBox = qobject_cast<QComboBox*>(sender());
    if (!comboBox) return;
    
    int selectedSectors = comboBox->itemData(index).toInt();
    
    ui->spinSectors->setValue(selectedSectors);
    ui->sliderSectors->setValue(selectedSectors);
    
    onSectorsOrTracksChanged();
}

void MainWindow::onTracksPresetSelected(int index) {
    QComboBox* comboBox = qobject_cast<QComboBox*>(sender());
    if (!comboBox) return;
    
    int selectedTracks = comboBox->itemData(index).toInt();
    
    ui->spinTracks->setValue(selectedTracks);
    ui->sliderTracks->setValue(selectedTracks);
    
    onSectorsOrTracksChanged();
}

void MainWindow::onBackgroundImageSelect() {
    QString filePath = QFileDialog::getOpenFileName(this, tr("Select Background Image"), 
                                                QDir::homePath(), 
                                                tr("Image Files (*.png *.jpg *.jpeg *.bmp *.gif)"));
    if (!filePath.isEmpty()) {
        backgroundImagePath = filePath;
        parachuteView->setBackgroundImage(filePath);
        clearBackgroundImageButton->setEnabled(true);
    }
}

void MainWindow::onClearBackgroundImage() {
    parachuteView->clearBackgroundImage();
    backgroundImagePath = "";
    clearBackgroundImageButton->setEnabled(false);
}

void MainWindow::onSectorsChanged(int value) {
    onSectorsOrTracksChanged();
}

void MainWindow::onTracksChanged(int value) {
    onSectorsOrTracksChanged();
}

void MainWindow::onReferenceCharChanged() {
    QString input = referenceCharInput->text();
    if (!input.isEmpty()) {
        QChar refChar = input.at(0);
        MessageEncoder::setReferenceCharacter(refChar);
        QMessageBox::information(this, tr("Reference Character"), 
                               tr("Reference character set to '%1' (ASCII: %2).")
                               .arg(refChar)
                               .arg(refChar.unicode()));
    } else {
        QChar defaultChar('@');
        MessageEncoder::setReferenceCharacter(defaultChar);
        referenceCharInput->setText("@");
        QMessageBox::information(this, tr("Reference Character"), 
                               tr("Empty input. Reference character reset to default '@' (ASCII: 64)."));
    }
    
    onMessageChanged();
}