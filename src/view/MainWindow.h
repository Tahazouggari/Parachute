#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSlider>
#include <QSpinBox>
#include <QLineEdit>
#include <QPushButton>
#include <QColorDialog>
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
    ParachuteView *parachuteView;

private slots:
    void onMessageChanged();
    void onBackgroundColorChanged();
    void onSaveParachute();

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
};

#endif // MAINWINDOW_H
