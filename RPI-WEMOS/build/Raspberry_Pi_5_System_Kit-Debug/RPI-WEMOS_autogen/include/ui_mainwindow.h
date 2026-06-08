/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QLCDNumber>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QLCDNumber *lcdNumber_Temperatuur;
    QLCDNumber *lcdNumber_CO2;
    QLCDNumber *lcdNumber_Luchtvochtigheid;
    QTextEdit *textEdit;
    QTextEdit *textEdit_2;
    QTextEdit *textEdit_3;
    QGraphicsView *graphicsView_Temperatuur;
    QGraphicsView *graphicsView_CO2;
    QGraphicsView *graphicsView_Luchtvochtigheid;
    QLabel *label_Status_Branddetectie;
    QLabel *label_Status_Ventilator;
    QLabel *label_Status_Brandoverruleknop;
    QLabel *label_Status_Bewegingssensor;
    QTextEdit *textEdit_5;
    QTextEdit *textEdit_6;
    QTextEdit *textEdit_7;
    QTextEdit *textEdit_8;
    QLabel *label_Status_Socketverbinding;
    QTextEdit *textEdit_9;
    QListWidget *listWidget_Actief;
    QTextEdit *textEdit_10;
    QTextEdit *textEdit_11;
    QTextBrowser *textBrowser_Logboek;
    QTextEdit *textEdit_12;
    QLineEdit *lineEdit_Lichtkrant_Nieuw;
    QPushButton *pushButton_Update_Lichtkrant;
    QLabel *label_Lichtkrant_Huidig;
    QTextEdit *textEdit_13;
    QTextEdit *textEdit_14;
    QTextEdit *textEdit_15;
    QLabel *label_Status_Verlichting_RGB;
    QTextEdit *textEdit_16;
    QPushButton *pushButton_Brandoverruleknop;
    QSpinBox *spinBox_Tafel_ID;
    QPushButton *pushButton_Reset_Tafel;
    QComboBox *comboBox_RGB_Kleur;
    QPushButton *pushButton_RGB_Set;
    QPushButton *pushButton_RGB_Uit;
    QCheckBox *checkBox_RGB_Auto;
    QPushButton *pushButton_Stuur_Menu;
    QTextBrowser *textBrowser_Socket;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1423, 741);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        lcdNumber_Temperatuur = new QLCDNumber(centralwidget);
        lcdNumber_Temperatuur->setObjectName("lcdNumber_Temperatuur");
        lcdNumber_Temperatuur->setGeometry(QRect(10, 30, 141, 61));
        lcdNumber_CO2 = new QLCDNumber(centralwidget);
        lcdNumber_CO2->setObjectName("lcdNumber_CO2");
        lcdNumber_CO2->setGeometry(QRect(310, 30, 141, 61));
        lcdNumber_Luchtvochtigheid = new QLCDNumber(centralwidget);
        lcdNumber_Luchtvochtigheid->setObjectName("lcdNumber_Luchtvochtigheid");
        lcdNumber_Luchtvochtigheid->setGeometry(QRect(610, 30, 141, 61));
        textEdit = new QTextEdit(centralwidget);
        textEdit->setObjectName("textEdit");
        textEdit->setEnabled(true);
        textEdit->setGeometry(QRect(10, 0, 191, 31));
        textEdit->setReadOnly(true);
        textEdit_2 = new QTextEdit(centralwidget);
        textEdit_2->setObjectName("textEdit_2");
        textEdit_2->setGeometry(QRect(310, 0, 104, 31));
        textEdit_2->setReadOnly(true);
        textEdit_3 = new QTextEdit(centralwidget);
        textEdit_3->setObjectName("textEdit_3");
        textEdit_3->setGeometry(QRect(610, 0, 161, 31));
        textEdit_3->setReadOnly(true);
        graphicsView_Temperatuur = new QGraphicsView(centralwidget);
        graphicsView_Temperatuur->setObjectName("graphicsView_Temperatuur");
        graphicsView_Temperatuur->setGeometry(QRect(10, 90, 300, 250));
        graphicsView_CO2 = new QGraphicsView(centralwidget);
        graphicsView_CO2->setObjectName("graphicsView_CO2");
        graphicsView_CO2->setGeometry(QRect(310, 90, 300, 250));
        graphicsView_Luchtvochtigheid = new QGraphicsView(centralwidget);
        graphicsView_Luchtvochtigheid->setObjectName("graphicsView_Luchtvochtigheid");
        graphicsView_Luchtvochtigheid->setGeometry(QRect(610, 90, 300, 250));
        label_Status_Branddetectie = new QLabel(centralwidget);
        label_Status_Branddetectie->setObjectName("label_Status_Branddetectie");
        label_Status_Branddetectie->setGeometry(QRect(140, 390, 90, 90));
        label_Status_Branddetectie->setAutoFillBackground(false);
        label_Status_Branddetectie->setStyleSheet(QString::fromUtf8("background-color: rgb(170, 0, 0);\n"
"border-radius: 45px;"));
        label_Status_Ventilator = new QLabel(centralwidget);
        label_Status_Ventilator->setObjectName("label_Status_Ventilator");
        label_Status_Ventilator->setGeometry(QRect(20, 390, 90, 90));
        label_Status_Ventilator->setAutoFillBackground(false);
        label_Status_Ventilator->setStyleSheet(QString::fromUtf8("background-color: rgb(170, 0, 0);\n"
"border-radius: 45px;"));
        label_Status_Brandoverruleknop = new QLabel(centralwidget);
        label_Status_Brandoverruleknop->setObjectName("label_Status_Brandoverruleknop");
        label_Status_Brandoverruleknop->setGeometry(QRect(270, 390, 90, 90));
        label_Status_Brandoverruleknop->setAutoFillBackground(false);
        label_Status_Brandoverruleknop->setStyleSheet(QString::fromUtf8("background-color: rgb(170, 0, 0);\n"
"border-radius: 45px;"));
        label_Status_Bewegingssensor = new QLabel(centralwidget);
        label_Status_Bewegingssensor->setObjectName("label_Status_Bewegingssensor");
        label_Status_Bewegingssensor->setGeometry(QRect(20, 530, 90, 90));
        label_Status_Bewegingssensor->setAutoFillBackground(false);
        label_Status_Bewegingssensor->setStyleSheet(QString::fromUtf8("background-color: rgb(170, 0, 0);\n"
"border-radius: 45px;"));
        textEdit_5 = new QTextEdit(centralwidget);
        textEdit_5->setObjectName("textEdit_5");
        textEdit_5->setGeometry(QRect(20, 360, 101, 31));
        textEdit_5->setReadOnly(true);
        textEdit_6 = new QTextEdit(centralwidget);
        textEdit_6->setObjectName("textEdit_6");
        textEdit_6->setGeometry(QRect(250, 360, 161, 31));
        textEdit_6->setReadOnly(true);
        textEdit_7 = new QTextEdit(centralwidget);
        textEdit_7->setObjectName("textEdit_7");
        textEdit_7->setGeometry(QRect(130, 360, 111, 31));
        textEdit_7->setReadOnly(true);
        textEdit_8 = new QTextEdit(centralwidget);
        textEdit_8->setObjectName("textEdit_8");
        textEdit_8->setGeometry(QRect(10, 500, 151, 31));
        textEdit_8->setReadOnly(true);
        label_Status_Socketverbinding = new QLabel(centralwidget);
        label_Status_Socketverbinding->setObjectName("label_Status_Socketverbinding");
        label_Status_Socketverbinding->setGeometry(QRect(200, 570, 90, 90));
        label_Status_Socketverbinding->setAutoFillBackground(false);
        label_Status_Socketverbinding->setStyleSheet(QString::fromUtf8("background-color: rgb(170, 0, 0);\n"
"border-radius: 45px;"));
        textEdit_9 = new QTextEdit(centralwidget);
        textEdit_9->setObjectName("textEdit_9");
        textEdit_9->setGeometry(QRect(170, 530, 151, 31));
        textEdit_9->setReadOnly(true);
        listWidget_Actief = new QListWidget(centralwidget);
        listWidget_Actief->setObjectName("listWidget_Actief");
        listWidget_Actief->setGeometry(QRect(920, 30, 491, 141));
        textEdit_10 = new QTextEdit(centralwidget);
        textEdit_10->setObjectName("textEdit_10");
        textEdit_10->setGeometry(QRect(920, 0, 311, 31));
        textEdit_10->setReadOnly(true);
        textEdit_11 = new QTextEdit(centralwidget);
        textEdit_11->setObjectName("textEdit_11");
        textEdit_11->setGeometry(QRect(410, 340, 191, 51));
        textEdit_11->setReadOnly(true);
        textBrowser_Logboek = new QTextBrowser(centralwidget);
        textBrowser_Logboek->setObjectName("textBrowser_Logboek");
        textBrowser_Logboek->setGeometry(QRect(920, 200, 491, 321));
        textEdit_12 = new QTextEdit(centralwidget);
        textEdit_12->setObjectName("textEdit_12");
        textEdit_12->setGeometry(QRect(920, 170, 91, 31));
        textEdit_12->setReadOnly(true);
        lineEdit_Lichtkrant_Nieuw = new QLineEdit(centralwidget);
        lineEdit_Lichtkrant_Nieuw->setObjectName("lineEdit_Lichtkrant_Nieuw");
        lineEdit_Lichtkrant_Nieuw->setGeometry(QRect(580, 620, 171, 31));
        pushButton_Update_Lichtkrant = new QPushButton(centralwidget);
        pushButton_Update_Lichtkrant->setObjectName("pushButton_Update_Lichtkrant");
        pushButton_Update_Lichtkrant->setGeometry(QRect(760, 620, 131, 29));
        label_Lichtkrant_Huidig = new QLabel(centralwidget);
        label_Lichtkrant_Huidig->setObjectName("label_Lichtkrant_Huidig");
        label_Lichtkrant_Huidig->setGeometry(QRect(580, 580, 311, 31));
        label_Lichtkrant_Huidig->setFrameShape(QFrame::Shape::NoFrame);
        label_Lichtkrant_Huidig->setWordWrap(false);
        textEdit_13 = new QTextEdit(centralwidget);
        textEdit_13->setObjectName("textEdit_13");
        textEdit_13->setGeometry(QRect(460, 580, 121, 31));
        textEdit_13->setReadOnly(true);
        textEdit_14 = new QTextEdit(centralwidget);
        textEdit_14->setObjectName("textEdit_14");
        textEdit_14->setGeometry(QRect(460, 620, 121, 31));
        textEdit_14->setReadOnly(true);
        textEdit_15 = new QTextEdit(centralwidget);
        textEdit_15->setObjectName("textEdit_15");
        textEdit_15->setGeometry(QRect(550, 540, 101, 31));
        textEdit_15->setReadOnly(true);
        label_Status_Verlichting_RGB = new QLabel(centralwidget);
        label_Status_Verlichting_RGB->setObjectName("label_Status_Verlichting_RGB");
        label_Status_Verlichting_RGB->setGeometry(QRect(770, 390, 90, 90));
        label_Status_Verlichting_RGB->setAutoFillBackground(false);
        label_Status_Verlichting_RGB->setStyleSheet(QString::fromUtf8("background-color: rgb(170, 0, 0);\n"
"border-radius: 45px;"));
        textEdit_16 = new QTextEdit(centralwidget);
        textEdit_16->setObjectName("textEdit_16");
        textEdit_16->setGeometry(QRect(770, 360, 141, 31));
        textEdit_16->setReadOnly(true);
        pushButton_Brandoverruleknop = new QPushButton(centralwidget);
        pushButton_Brandoverruleknop->setObjectName("pushButton_Brandoverruleknop");
        pushButton_Brandoverruleknop->setGeometry(QRect(240, 490, 141, 29));
        spinBox_Tafel_ID = new QSpinBox(centralwidget);
        spinBox_Tafel_ID->setObjectName("spinBox_Tafel_ID");
        spinBox_Tafel_ID->setGeometry(QRect(410, 490, 90, 29));
        spinBox_Tafel_ID->setMinimum(1);
        spinBox_Tafel_ID->setMaximum(20);
        pushButton_Reset_Tafel = new QPushButton(centralwidget);
        pushButton_Reset_Tafel->setObjectName("pushButton_Reset_Tafel");
        pushButton_Reset_Tafel->setGeometry(QRect(410, 525, 90, 29));
        comboBox_RGB_Kleur = new QComboBox(centralwidget);
        comboBox_RGB_Kleur->addItem(QString());
        comboBox_RGB_Kleur->addItem(QString());
        comboBox_RGB_Kleur->addItem(QString());
        comboBox_RGB_Kleur->addItem(QString());
        comboBox_RGB_Kleur->addItem(QString());
        comboBox_RGB_Kleur->setObjectName("comboBox_RGB_Kleur");
        comboBox_RGB_Kleur->setGeometry(QRect(760, 490, 141, 29));
        pushButton_RGB_Set = new QPushButton(centralwidget);
        pushButton_RGB_Set->setObjectName("pushButton_RGB_Set");
        pushButton_RGB_Set->setGeometry(QRect(760, 525, 66, 29));
        pushButton_RGB_Uit = new QPushButton(centralwidget);
        pushButton_RGB_Uit->setObjectName("pushButton_RGB_Uit");
        pushButton_RGB_Uit->setGeometry(QRect(835, 525, 66, 29));
        checkBox_RGB_Auto = new QCheckBox(centralwidget);
        checkBox_RGB_Auto->setObjectName("checkBox_RGB_Auto");
        checkBox_RGB_Auto->setGeometry(QRect(680, 560, 221, 24));
        checkBox_RGB_Auto->setChecked(true);
        pushButton_Stuur_Menu = new QPushButton(centralwidget);
        pushButton_Stuur_Menu->setObjectName("pushButton_Stuur_Menu");
        pushButton_Stuur_Menu->setGeometry(QRect(760, 655, 131, 29));
        textBrowser_Socket = new QTextBrowser(centralwidget);
        textBrowser_Socket->setObjectName("textBrowser_Socket");
        textBrowser_Socket->setGeometry(QRect(920, 530, 491, 141));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1423, 19));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        textEdit->setHtml(QCoreApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"hr { height: 1px; border-width: 0; }\n"
"li.unchecked::marker { content: \"\\2610\"; }\n"
"li.checked::marker { content: \"\\2612\"; }\n"
"</style></head><body style=\" font-family:'Sans Serif'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Segoe UI'; font-weight:700;\">Temperatuur (Graden)</span></p></body></html>", nullptr));
        textEdit_2->setHtml(QCoreApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"hr { height: 1px; border-width: 0; }\n"
"li.unchecked::marker { content: \"\\2610\"; }\n"
"li.checked::marker { content: \"\\2612\"; }\n"
"</style></head><body style=\" font-family:'Sans Serif'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Segoe UI'; font-weight:700;\">CO2 (PPM)</span></p></body></html>", nullptr));
        textEdit_3->setHtml(QCoreApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"hr { height: 1px; border-width: 0; }\n"
"li.unchecked::marker { content: \"\\2610\"; }\n"
"li.checked::marker { content: \"\\2612\"; }\n"
"</style></head><body style=\" font-family:'Sans Serif'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Segoe UI'; font-weight:700;\">Luchtvochtigheid (%)</span></p></body></html>", nullptr));
        label_Status_Branddetectie->setText(QString());
        label_Status_Ventilator->setText(QString());
        label_Status_Brandoverruleknop->setText(QString());
        label_Status_Bewegingssensor->setText(QString());
        textEdit_5->setHtml(QCoreApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"hr { height: 1px; border-width: 0; }\n"
"li.unchecked::marker { content: \"\\2610\"; }\n"
"li.checked::marker { content: \"\\2612\"; }\n"
"</style></head><body style=\" font-family:'Sans Serif'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Segoe UI'; font-weight:700;\">Ventilator</span></p></body></html>", nullptr));
        textEdit_6->setHtml(QCoreApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"hr { height: 1px; border-width: 0; }\n"
"li.unchecked::marker { content: \"\\2610\"; }\n"
"li.checked::marker { content: \"\\2612\"; }\n"
"</style></head><body style=\" font-family:'Sans Serif'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Segoe UI'; font-weight:700;\">Brandoverruleknop</span></p></body></html>", nullptr));
        textEdit_7->setHtml(QCoreApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"hr { height: 1px; border-width: 0; }\n"
"li.unchecked::marker { content: \"\\2610\"; }\n"
"li.checked::marker { content: \"\\2612\"; }\n"
"</style></head><body style=\" font-family:'Sans Serif'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Segoe UI'; font-weight:700;\">Branddetectie</span></p></body></html>", nullptr));
        textEdit_8->setHtml(QCoreApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"hr { height: 1px; border-width: 0; }\n"
"li.unchecked::marker { content: \"\\2610\"; }\n"
"li.checked::marker { content: \"\\2612\"; }\n"
"</style></head><body style=\" font-family:'Sans Serif'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Segoe UI'; font-weight:700;\">Bewegingssensor</span></p></body></html>", nullptr));
        label_Status_Socketverbinding->setText(QCoreApplication::translate("MainWindow", "     RPI-BUS", nullptr));
        textEdit_9->setHtml(QCoreApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"hr { height: 1px; border-width: 0; }\n"
"li.unchecked::marker { content: \"\\2610\"; }\n"
"li.checked::marker { content: \"\\2612\"; }\n"
"</style></head><body style=\" font-family:'Sans Serif'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Segoe UI'; font-weight:700;\">Socketverbinding</span></p></body></html>", nullptr));
        textEdit_10->setHtml(QCoreApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"hr { height: 1px; border-width: 0; }\n"
"li.unchecked::marker { content: \"\\2610\"; }\n"
"li.checked::marker { content: \"\\2612\"; }\n"
"</style></head><body style=\" font-family:'Sans Serif'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Segoe UI'; font-weight:700;\">Actieve waarschuwingen &amp; Foutmeldingen</span></p></body></html>", nullptr));
        textEdit_11->setHtml(QCoreApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"hr { height: 1px; border-width: 0; }\n"
"li.unchecked::marker { content: \"\\2610\"; }\n"
"li.checked::marker { content: \"\\2612\"; }\n"
"</style></head><body style=\" font-family:'Sans Serif'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Segoe UI'; font-weight:700;\">Oproepsysteem drukknop </span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Segoe UI'; font-weight:700;\">tafels</span></p></body></html>", nullptr));
        textEdit_12->setHtml(QCoreApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"hr { height: 1px; border-width: 0; }\n"
"li.unchecked::marker { content: \"\\2610\"; }\n"
"li.checked::marker { content: \"\\2612\"; }\n"
"</style></head><body style=\" font-family:'Sans Serif'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Segoe UI'; font-weight:700;\">Logboek</span></p></body></html>", nullptr));
        pushButton_Update_Lichtkrant->setText(QCoreApplication::translate("MainWindow", "Update Lichtkrant", nullptr));
        label_Lichtkrant_Huidig->setText(QCoreApplication::translate("MainWindow", "TextLabel", nullptr));
        textEdit_13->setHtml(QCoreApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"hr { height: 1px; border-width: 0; }\n"
"li.unchecked::marker { content: \"\\2610\"; }\n"
"li.checked::marker { content: \"\\2612\"; }\n"
"</style></head><body style=\" font-family:'Sans Serif'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Segoe UI'; font-weight:700;\">Huidige Tekst:</span></p></body></html>", nullptr));
        textEdit_14->setHtml(QCoreApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"hr { height: 1px; border-width: 0; }\n"
"li.unchecked::marker { content: \"\\2610\"; }\n"
"li.checked::marker { content: \"\\2612\"; }\n"
"</style></head><body style=\" font-family:'Sans Serif'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Segoe UI'; font-weight:700;\">Nieuwe Tekst:</span></p></body></html>", nullptr));
        textEdit_15->setHtml(QCoreApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"hr { height: 1px; border-width: 0; }\n"
"li.unchecked::marker { content: \"\\2610\"; }\n"
"li.checked::marker { content: \"\\2612\"; }\n"
"</style></head><body style=\" font-family:'Sans Serif'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Segoe UI'; font-weight:700;\">Lichtkrant</span></p></body></html>", nullptr));
        label_Status_Verlichting_RGB->setText(QString());
        textEdit_16->setHtml(QCoreApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"hr { height: 1px; border-width: 0; }\n"
"li.unchecked::marker { content: \"\\2610\"; }\n"
"li.checked::marker { content: \"\\2612\"; }\n"
"</style></head><body style=\" font-family:'Sans Serif'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Segoe UI'; font-weight:700;\">Verlichting (RGB)</span></p></body></html>", nullptr));
        pushButton_Brandoverruleknop->setText(QCoreApplication::translate("MainWindow", "Brandoverruleknop", nullptr));
        pushButton_Reset_Tafel->setText(QCoreApplication::translate("MainWindow", "Reset Tafel", nullptr));
        comboBox_RGB_Kleur->setItemText(0, QCoreApplication::translate("MainWindow", "Wit", nullptr));
        comboBox_RGB_Kleur->setItemText(1, QCoreApplication::translate("MainWindow", "Warm", nullptr));
        comboBox_RGB_Kleur->setItemText(2, QCoreApplication::translate("MainWindow", "Rood", nullptr));
        comboBox_RGB_Kleur->setItemText(3, QCoreApplication::translate("MainWindow", "Blauw", nullptr));
        comboBox_RGB_Kleur->setItemText(4, QCoreApplication::translate("MainWindow", "Groen", nullptr));

        pushButton_RGB_Set->setText(QCoreApplication::translate("MainWindow", "Stel In", nullptr));
        pushButton_RGB_Uit->setText(QCoreApplication::translate("MainWindow", "Lamp Uit", nullptr));
        checkBox_RGB_Auto->setText(QCoreApplication::translate("MainWindow", "RGB volgt bewegingssensor", nullptr));
        pushButton_Stuur_Menu->setText(QCoreApplication::translate("MainWindow", "Stuur als Menu", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
