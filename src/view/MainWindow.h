#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ParachuteView.h"
#include "BinaryWidget.h"
#include "../utils/LanguageManager.h" // Include LanguageManager
#include "HexView.h"


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
    
private slots:
    void onMessageChanged();
    void onBackgroundColorChanged();
    void onSaveParachute();
    void onSaveFile();
    void onOpenFile();
    void onExit();
    void onSectorsOrTracksChanged();
    void onLanguageEnglish();
    void onLanguageFrench();
    void onLanguageArabic();
    void retranslateUi() ;

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
};

#endif // MAINWINDOW_H