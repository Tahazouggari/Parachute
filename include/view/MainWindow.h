#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ParachuteView.h"
#include "BinaryWidget.h"
#include "../utils/LanguageManager.h" 
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
    Ui::MainWindow *ui; 
    ParachuteView *parachuteView; 
    BinaryWidget *binaryWidget;   
    LanguageManager *languageManager;
    HexView *hexView; 
    bool mode10Enabled; 
    QString backgroundImagePath; 
    
    
    QPushButton *parachuteColorButton;
    QPushButton *sectorColorButton;
    
    
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
    
    
    void updateSectorsPresets(QComboBox* comboBox, bool mode10); 
    void updateTracksPresets(QComboBox* comboBox); 
    
private slots:
    void onMessageChanged();
    void onBackgroundColorChanged();
    void onParachuteColorChanged();
    void onSectorColorChanged();
    void onRandomColorModeToggled(bool checked);
    void onMode10Toggled(bool checked); 
    void onSectorsPresetSelected(int index); 
    void onTracksPresetSelected(int index); 
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
    void onBackgroundImageSelect(); 
    void onClearBackgroundImage(); 

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
};

#endif // MAINWINDOW_H