#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSlider>
#include <QSpinBox>
#include <QLineEdit>
#include <QPushButton>
#include <QTabWidget>
#include "ParachuteView.h"
#include "presenter/ParachutePresenter.h"
#include "model/MessageEncoder.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

private:
    ParachutePresenter *presenter;
    QSlider *sliderSectors;
    QSlider *sliderTracks;
    QSpinBox *spinSectors;
    QSpinBox *spinTracks;
    QLineEdit *messageInput;
    QPushButton *colorButton;
    QPushButton *exportButton;
    QTabWidget *viewTabs;
    ParachuteView *parachuteView;
    QWidget *binaryView;

    QMenu *fileMenu; // Keep this declaration
    QAction *saveAction;
    QAction *openAction;
    QAction *exitAction;

private slots:
    void onMessageChanged();
    void onBackgroundColorChanged();
    void onSaveParachute();
    void onSectorsOrTracksChanged();
    void onSaveFile();
    void onOpenFile();
    void onExit();

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
};

#endif // MAINWINDOW_H
