/********************************************************************************
** Form generated from reading UI file 'hand_detector.ui'
**
** Created by: Qt User Interface Compiler version 5.9.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_HAND_DETECTOR_H
#define UI_HAND_DETECTOR_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Hand_Detector
{
public:
    QAction *actionCalibrar;
    QAction *actionDetecci_n_de_Volales;
    QAction *actionDetecci_n_Abecedario;
    QAction *actionDetecci_n_Palarbas;
    QAction *actionDetecci_n_Frasee;
    QAction *actionManos;
    QAction *actionLetras;
    QAction *actionPalabras;
    QAction *actionFrases;
    QAction *actionLSCh;
    QWidget *centralWidget;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QScrollArea *scrollArea_2;
    QWidget *scrollAreaWidgetContents_2;
    QPushButton *pushButton;
    QTextEdit *textEdit;
    QTextEdit *textEdit_2;
    QMenuBar *menuBar;
    QMenu *menuMen;
    QMenu *menuDetecci_n;
    QMenu *menuAbout;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *Hand_Detector)
    {
        if (Hand_Detector->objectName().isEmpty())
            Hand_Detector->setObjectName(QStringLiteral("Hand_Detector"));
        Hand_Detector->resize(819, 501);
        actionCalibrar = new QAction(Hand_Detector);
        actionCalibrar->setObjectName(QStringLiteral("actionCalibrar"));
        actionDetecci_n_de_Volales = new QAction(Hand_Detector);
        actionDetecci_n_de_Volales->setObjectName(QStringLiteral("actionDetecci_n_de_Volales"));
        actionDetecci_n_Abecedario = new QAction(Hand_Detector);
        actionDetecci_n_Abecedario->setObjectName(QStringLiteral("actionDetecci_n_Abecedario"));
        actionDetecci_n_Palarbas = new QAction(Hand_Detector);
        actionDetecci_n_Palarbas->setObjectName(QStringLiteral("actionDetecci_n_Palarbas"));
        actionDetecci_n_Frasee = new QAction(Hand_Detector);
        actionDetecci_n_Frasee->setObjectName(QStringLiteral("actionDetecci_n_Frasee"));
        actionManos = new QAction(Hand_Detector);
        actionManos->setObjectName(QStringLiteral("actionManos"));
        actionLetras = new QAction(Hand_Detector);
        actionLetras->setObjectName(QStringLiteral("actionLetras"));
        actionPalabras = new QAction(Hand_Detector);
        actionPalabras->setObjectName(QStringLiteral("actionPalabras"));
        actionFrases = new QAction(Hand_Detector);
        actionFrases->setObjectName(QStringLiteral("actionFrases"));
        actionLSCh = new QAction(Hand_Detector);
        actionLSCh->setObjectName(QStringLiteral("actionLSCh"));
        centralWidget = new QWidget(Hand_Detector);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        scrollArea = new QScrollArea(centralWidget);
        scrollArea->setObjectName(QStringLiteral("scrollArea"));
        scrollArea->setGeometry(QRect(10, 10, 391, 321));
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QStringLiteral("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 389, 319));
        scrollArea->setWidget(scrollAreaWidgetContents);
        scrollArea_2 = new QScrollArea(centralWidget);
        scrollArea_2->setObjectName(QStringLiteral("scrollArea_2"));
        scrollArea_2->setGeometry(QRect(420, 10, 381, 321));
        scrollArea_2->setWidgetResizable(true);
        scrollAreaWidgetContents_2 = new QWidget();
        scrollAreaWidgetContents_2->setObjectName(QStringLiteral("scrollAreaWidgetContents_2"));
        scrollAreaWidgetContents_2->setGeometry(QRect(0, 0, 379, 319));
        scrollArea_2->setWidget(scrollAreaWidgetContents_2);
        pushButton = new QPushButton(centralWidget);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(360, 400, 101, 31));
        textEdit = new QTextEdit(centralWidget);
        textEdit->setObjectName(QStringLiteral("textEdit"));
        textEdit->setGeometry(QRect(40, 340, 321, 31));
        textEdit_2 = new QTextEdit(centralWidget);
        textEdit_2->setObjectName(QStringLiteral("textEdit_2"));
        textEdit_2->setGeometry(QRect(450, 340, 321, 31));
        Hand_Detector->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(Hand_Detector);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 819, 23));
        menuMen = new QMenu(menuBar);
        menuMen->setObjectName(QStringLiteral("menuMen"));
        menuDetecci_n = new QMenu(menuMen);
        menuDetecci_n->setObjectName(QStringLiteral("menuDetecci_n"));
        menuAbout = new QMenu(menuBar);
        menuAbout->setObjectName(QStringLiteral("menuAbout"));
        Hand_Detector->setMenuBar(menuBar);
        mainToolBar = new QToolBar(Hand_Detector);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        Hand_Detector->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(Hand_Detector);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        Hand_Detector->setStatusBar(statusBar);

        menuBar->addAction(menuMen->menuAction());
        menuBar->addAction(menuAbout->menuAction());
        menuMen->addAction(actionCalibrar);
        menuMen->addAction(menuDetecci_n->menuAction());
        menuDetecci_n->addAction(actionManos);
        menuDetecci_n->addAction(actionLetras);
        menuDetecci_n->addAction(actionPalabras);
        menuDetecci_n->addAction(actionFrases);

        retranslateUi(Hand_Detector);
        QObject::connect(pushButton, SIGNAL(clicked()), Hand_Detector, SLOT(handCharge()));

        QMetaObject::connectSlotsByName(Hand_Detector);
    } // setupUi

    void retranslateUi(QMainWindow *Hand_Detector)
    {
        Hand_Detector->setWindowTitle(QApplication::translate("Hand_Detector", "Hand_Detector", Q_NULLPTR));
        actionCalibrar->setText(QApplication::translate("Hand_Detector", "Calibraci\303\263n", Q_NULLPTR));
        actionDetecci_n_de_Volales->setText(QApplication::translate("Hand_Detector", "Detecci\303\263n Vocales", Q_NULLPTR));
        actionDetecci_n_Abecedario->setText(QApplication::translate("Hand_Detector", "Detecci\303\263n Abecedario", Q_NULLPTR));
        actionDetecci_n_Palarbas->setText(QApplication::translate("Hand_Detector", "Detecci\303\263n Palabras", Q_NULLPTR));
        actionDetecci_n_Frasee->setText(QApplication::translate("Hand_Detector", "Detecci\303\263n Frases", Q_NULLPTR));
        actionManos->setText(QApplication::translate("Hand_Detector", "Manos", Q_NULLPTR));
        actionLetras->setText(QApplication::translate("Hand_Detector", "Letras", Q_NULLPTR));
        actionPalabras->setText(QApplication::translate("Hand_Detector", "Palabras", Q_NULLPTR));
        actionFrases->setText(QApplication::translate("Hand_Detector", "Frases", Q_NULLPTR));
        actionLSCh->setText(QApplication::translate("Hand_Detector", "LSCh", Q_NULLPTR));
        pushButton->setText(QApplication::translate("Hand_Detector", "Start", Q_NULLPTR));
        menuMen->setTitle(QApplication::translate("Hand_Detector", "Men\303\272", Q_NULLPTR));
        menuDetecci_n->setTitle(QApplication::translate("Hand_Detector", "Detecci\303\263n", Q_NULLPTR));
        menuAbout->setTitle(QApplication::translate("Hand_Detector", "About", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class Hand_Detector: public Ui_Hand_Detector {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_HAND_DETECTOR_H
