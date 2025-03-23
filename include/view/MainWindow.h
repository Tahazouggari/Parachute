#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ParachuteView.h"
#include "BinaryWidget.h"
#include "../utils/LanguageManager.h" // Include LanguageManager
#include "HexView.h"

class QComboBox;
class QPushButton;
class QLabel;
class QGroupBox;
class QCheckBox;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

private:
    Ui::MainWindow *ui; // Pointer to the generated UI class
    ParachuteView *parachuteView; // Parachute visualization widget
    BinaryWidget *binaryWidget;   // Binary visualization widget
    LanguageManager *languageManager; // Language manager
    HexView *hexView; // Hexadecimal visualization widget
    bool mode10Enabled; // Mode 10 (10 bits par caractère)
    QString backgroundImagePath; // Chemin vers l'image d'arrière-plan
    
    // Boutons de couleur qui doivent être traduits
    QPushButton *parachuteColorButton;
    QPushButton *sectorColorButton;
    
    // Contrôles de l'interface à traduire
    QGroupBox *colorCustomizationGroup;
    QLabel *backgroundColorLabel;
    QLabel *colorBits1Label;
    QLabel *colorBits0Label;
    QPushButton *parachuteButton;
    QPushButton *sectorButton;
    QLabel *trackPresetsLabel;
    QLabel *sectorPresetsLabel;
    QCheckBox *randomColorModeCheckBox;
    QCheckBox *mode10CheckBox;
    QComboBox *sectorsPresetComboBox;
    QComboBox *tracksPresetComboBox;
    QPushButton *exportButton;
    QPushButton *backgroundImageButton;
    QPushButton *clearBackgroundImageButton;
    
    // Méthodes utilitaires
    void updateSectorsPresets(QComboBox* comboBox, bool mode10); // Met à jour les préréglages de secteurs
    void updateTracksPresets(QComboBox* comboBox); // Met à jour les préréglages de pistes
    
private slots:
    void onMessageChanged();
    void onBackgroundColorChanged();
    void onParachuteColorChanged();
    void onSectorColorChanged();
    void onRandomColorModeToggled(bool checked);
    void onMode10Toggled(bool checked); // Slot pour le mode 10
    void onSectorsPresetSelected(int index); // Slot pour les préréglages de secteurs
    void onTracksPresetSelected(int index); // Slot pour les préréglages de pistes
    void onExportImage();
    void onSaveFile();
    void onOpenFile();
    void onExit();
    void onSectorsChanged(int value);
    void onTracksChanged(int value);
    void onSectorsOrTracksChanged();
    void onLanguageEnglish();
    void onLanguageFrench();
    void onLanguageArabic();
    void retranslateUi();
    void onBackgroundImageSelect(); // Slot pour sélectionner une image d'arrière-plan
    void onClearBackgroundImage(); // Slot pour supprimer l'image d'arrière-plan

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
};

#endif // MAINWINDOW_H