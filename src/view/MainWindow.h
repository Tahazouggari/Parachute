#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ParachuteView.h"
#include "BinaryWidget.h"
#include "../utils/LanguageManager.h" // Include LanguageManager
#include "HexView.h"

class QComboBox;

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
    
    // Méthodes utilitaires
    void updateSectorsPresets(QComboBox* comboBox, bool mode10); // Met à jour les préréglages de secteurs
    
private slots:
    void onMessageChanged();
    void onBackgroundColorChanged();
    void onParachuteColorChanged();
    void onSectorColorChanged();
    void onRandomColorModeToggled(bool checked);
    void onMode10Toggled(bool checked); // Slot pour le mode 10
    void onSectorsPresetSelected(int index); // Slot pour les préréglages de secteurs
    void onSaveParachute();
    void onSaveFile();
    void onOpenFile();
    void onExit();
    void onSectorsOrTracksChanged();
    void onLanguageEnglish();
    void onLanguageFrench();
    void onLanguageArabic();
    void retranslateUi() ;

    // Track presets methods
    void onTracksPresetSelected(int index);
    void updateTracksPresets(QComboBox* comboBox);

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
};

#endif // MAINWINDOW_H