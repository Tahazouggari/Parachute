#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ParachuteView.h"
#include "BinaryWidget.h"
#include "../presenter/ParachutePresenter.h"
#include "../model/MessageEncoder.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

private:
    Ui::MainWindow *ui; // Pointer to the generated UI class
    ParachuteView *parachuteView; // Parachute visualization widget
    BinaryWidget *binaryWidget;   // Binary visualization widget

private slots:
    void onMessageChanged();
    void onBackgroundColorChanged();
    void onSaveParachute();
    void onSaveFile();
    void onOpenFile();
    void onExit();
    void onSectorsOrTracksChanged();

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
};

#endif // MAINWINDOW_H
