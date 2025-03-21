#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ui_MainWindow.h" // Include the generated header file
#include "ParachuteView.h"
#include "BinaryWidget.h" // Include BinaryWidget

class MainWindow : public QMainWindow {
    Q_OBJECT

private:
    Ui::MainWindow ui; // Use the generated UI class
    ParachuteView *parachuteView; // Declare parachuteView as a private member
    BinaryWidget *binaryWidget; // Declare binaryWidget as a private member

private slots:
    void onMessageChanged();
    void onBackgroundColorChanged();
    void onSaveParachute();
    void onSectorsOrTracksChanged(); // Ensure this is declared
    void onSaveFile();
    void onOpenFile();
    void onExit();

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
};

#endif // MAINWINDOW_H
