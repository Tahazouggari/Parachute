/********************************************************************************
** Form generated from reading UI file 'MainWindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.16
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *saveAction;
    QAction *openAction;
    QAction *exitAction;
    QAction *actionEnglish;
    QAction *actionfrensh;
    QAction *actionArabic;
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QLabel *titleLabel;
    QGroupBox *messageBox;
    QVBoxLayout *verticalLayout_2;
    QLabel *labelMessage;
    QLineEdit *messageInput;
    QTabWidget *viewTabs;
    QWidget *parachuteViewTab;
    QWidget *binaryViewTab;
    QVBoxLayout *verticalLayout_3;
    QWidget *hexViewTab;
    QGroupBox *controlsBox;
    QVBoxLayout *verticalLayout_4;
    QLabel *labelSectors;
    QSlider *sliderSectors;
    QSpinBox *spinSectors;
    QLabel *labelTracks;
    QSlider *sliderTracks;
    QSpinBox *spinTracks;
    QPushButton *colorButton;
    QPushButton *exportButton;
    QMenuBar *menubar;
    QMenu *fileMenu;
    QMenu *languageMenu;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(800, 600);
        saveAction = new QAction(MainWindow);
        saveAction->setObjectName(QString::fromUtf8("saveAction"));
        openAction = new QAction(MainWindow);
        openAction->setObjectName(QString::fromUtf8("openAction"));
        exitAction = new QAction(MainWindow);
        exitAction->setObjectName(QString::fromUtf8("exitAction"));
        actionEnglish = new QAction(MainWindow);
        actionEnglish->setObjectName(QString::fromUtf8("actionEnglish"));
        actionfrensh = new QAction(MainWindow);
        actionfrensh->setObjectName(QString::fromUtf8("actionfrensh"));
        actionArabic = new QAction(MainWindow);
        actionArabic->setObjectName(QString::fromUtf8("actionArabic"));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        titleLabel = new QLabel(centralwidget);
        titleLabel->setObjectName(QString::fromUtf8("titleLabel"));
        titleLabel->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(titleLabel);

        messageBox = new QGroupBox(centralwidget);
        messageBox->setObjectName(QString::fromUtf8("messageBox"));
        verticalLayout_2 = new QVBoxLayout(messageBox);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        labelMessage = new QLabel(messageBox);
        labelMessage->setObjectName(QString::fromUtf8("labelMessage"));

        verticalLayout_2->addWidget(labelMessage);

        messageInput = new QLineEdit(messageBox);
        messageInput->setObjectName(QString::fromUtf8("messageInput"));

        verticalLayout_2->addWidget(messageInput);


        verticalLayout->addWidget(messageBox);

        viewTabs = new QTabWidget(centralwidget);
        viewTabs->setObjectName(QString::fromUtf8("viewTabs"));
        viewTabs->setEnabled(true);
        viewTabs->setTabPosition(QTabWidget::North);
        parachuteViewTab = new QWidget();
        parachuteViewTab->setObjectName(QString::fromUtf8("parachuteViewTab"));
        viewTabs->addTab(parachuteViewTab, QString());
        binaryViewTab = new QWidget();
        binaryViewTab->setObjectName(QString::fromUtf8("binaryViewTab"));
        verticalLayout_3 = new QVBoxLayout(binaryViewTab);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        viewTabs->addTab(binaryViewTab, QString());
        hexViewTab = new QWidget();
        hexViewTab->setObjectName(QString::fromUtf8("hexViewTab"));
        viewTabs->addTab(hexViewTab, QString());

        verticalLayout->addWidget(viewTabs);

        controlsBox = new QGroupBox(centralwidget);
        controlsBox->setObjectName(QString::fromUtf8("controlsBox"));
        verticalLayout_4 = new QVBoxLayout(controlsBox);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        labelSectors = new QLabel(controlsBox);
        labelSectors->setObjectName(QString::fromUtf8("labelSectors"));

        verticalLayout_4->addWidget(labelSectors);

        sliderSectors = new QSlider(controlsBox);
        sliderSectors->setObjectName(QString::fromUtf8("sliderSectors"));
        sliderSectors->setOrientation(Qt::Horizontal);

        verticalLayout_4->addWidget(sliderSectors);

        spinSectors = new QSpinBox(controlsBox);
        spinSectors->setObjectName(QString::fromUtf8("spinSectors"));

        verticalLayout_4->addWidget(spinSectors);

        labelTracks = new QLabel(controlsBox);
        labelTracks->setObjectName(QString::fromUtf8("labelTracks"));

        verticalLayout_4->addWidget(labelTracks);

        sliderTracks = new QSlider(controlsBox);
        sliderTracks->setObjectName(QString::fromUtf8("sliderTracks"));
        sliderTracks->setOrientation(Qt::Horizontal);

        verticalLayout_4->addWidget(sliderTracks);

        spinTracks = new QSpinBox(controlsBox);
        spinTracks->setObjectName(QString::fromUtf8("spinTracks"));

        verticalLayout_4->addWidget(spinTracks);

        colorButton = new QPushButton(controlsBox);
        colorButton->setObjectName(QString::fromUtf8("colorButton"));

        verticalLayout_4->addWidget(colorButton);

        exportButton = new QPushButton(controlsBox);
        exportButton->setObjectName(QString::fromUtf8("exportButton"));

        verticalLayout_4->addWidget(exportButton);


        verticalLayout->addWidget(controlsBox);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 800, 24));
        fileMenu = new QMenu(menubar);
        fileMenu->setObjectName(QString::fromUtf8("fileMenu"));
        languageMenu = new QMenu(menubar);
        languageMenu->setObjectName(QString::fromUtf8("languageMenu"));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        menubar->addAction(fileMenu->menuAction());
        menubar->addAction(languageMenu->menuAction());
        fileMenu->addAction(saveAction);
        fileMenu->addAction(openAction);
        fileMenu->addAction(exitAction);
        languageMenu->addAction(actionEnglish);
        languageMenu->addAction(actionfrensh);
        languageMenu->addAction(actionArabic);

        retranslateUi(MainWindow);

        viewTabs->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "Parachute Encoder", nullptr));
        saveAction->setText(QCoreApplication::translate("MainWindow", "&Save", nullptr));
        openAction->setText(QCoreApplication::translate("MainWindow", "&Open", nullptr));
        exitAction->setText(QCoreApplication::translate("MainWindow", "E&xit", nullptr));
        actionEnglish->setText(QCoreApplication::translate("MainWindow", "English", nullptr));
        actionfrensh->setText(QCoreApplication::translate("MainWindow", "French", nullptr));
        actionArabic->setText(QCoreApplication::translate("MainWindow", "Arabic", nullptr));
        titleLabel->setStyleSheet(QCoreApplication::translate("MainWindow", "font-size: 18px; font-weight: bold;", nullptr));
        titleLabel->setText(QCoreApplication::translate("MainWindow", "Parachute Encoder", nullptr));
        messageBox->setTitle(QCoreApplication::translate("MainWindow", "Message", nullptr));
        labelMessage->setText(QCoreApplication::translate("MainWindow", "Your message:", nullptr));
        viewTabs->setTabText(viewTabs->indexOf(parachuteViewTab), QCoreApplication::translate("MainWindow", "Parachute View", nullptr));
        viewTabs->setTabText(viewTabs->indexOf(binaryViewTab), QCoreApplication::translate("MainWindow", "Binary View", nullptr));
        viewTabs->setTabText(viewTabs->indexOf(hexViewTab), QCoreApplication::translate("MainWindow", "Hexadecimal View", nullptr));
        controlsBox->setTitle(QCoreApplication::translate("MainWindow", "Controls", nullptr));
        labelSectors->setText(QCoreApplication::translate("MainWindow", "Sectors:", nullptr));
        labelTracks->setText(QCoreApplication::translate("MainWindow", "Tracks:", nullptr));
        colorButton->setText(QCoreApplication::translate("MainWindow", "Choose Background Color", nullptr));
        exportButton->setText(QCoreApplication::translate("MainWindow", "Export Parachute Image", nullptr));
        fileMenu->setTitle(QCoreApplication::translate("MainWindow", "&File", nullptr));
        languageMenu->setTitle(QCoreApplication::translate("MainWindow", "Change Language", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
