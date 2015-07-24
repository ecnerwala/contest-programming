/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 4.8.7
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout_2;
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout;
    QPushButton *openButton;
    QPushButton *reloadButton;
    QCheckBox *mouse_tracking;
    QSpacerItem *verticalSpacer_2;
    QLabel *position;
    QSpacerItem *verticalSpacer;
    QLabel *label;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(600, 313);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icon/icon"), QSize(), QIcon::Normal, QIcon::Off);
        MainWindow->setWindowIcon(icon);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        horizontalLayout_2 = new QHBoxLayout(centralWidget);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setSizeConstraint(QLayout::SetDefaultConstraint);
        openButton = new QPushButton(centralWidget);
        openButton->setObjectName(QString::fromUtf8("openButton"));
        openButton->setDefault(true);

        verticalLayout->addWidget(openButton);

        reloadButton = new QPushButton(centralWidget);
        reloadButton->setObjectName(QString::fromUtf8("reloadButton"));

        verticalLayout->addWidget(reloadButton);

        mouse_tracking = new QCheckBox(centralWidget);
        mouse_tracking->setObjectName(QString::fromUtf8("mouse_tracking"));

        verticalLayout->addWidget(mouse_tracking);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_2);

        position = new QLabel(centralWidget);
        position->setObjectName(QString::fromUtf8("position"));
        position->setMinimumSize(QSize(150, 0));
        position->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(position);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        label = new QLabel(centralWidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(label);


        horizontalLayout->addLayout(verticalLayout);


        horizontalLayout_2->addLayout(horizontalLayout);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 600, 25));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Zadanie Labirynt - Olimpiada Informatyczna", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        openButton->setToolTip(QApplication::translate("MainWindow", "Wczytuje plik z podanej \305\233cie\305\274ki.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        openButton->setText(QApplication::translate("MainWindow", "&Wczytaj", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        reloadButton->setToolTip(QApplication::translate("MainWindow", "Wczytuje ponownie ostatni plik.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        reloadButton->setText(QApplication::translate("MainWindow", "&Prze\305\202aduj", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        mouse_tracking->setToolTip(QApplication::translate("MainWindow", "Pokazuje aktualne po\305\202o\305\274enie myszki.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        mouse_tracking->setText(QApplication::translate("MainWindow", "\305\232ledzenie pozycji &myszki", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        position->setToolTip(QApplication::translate("MainWindow", "Pokazuje aktualne po\305\202o\305\274enie myszki.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        position->setText(QString());
        label->setText(QApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Sans'; font-size:10pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-weight:600;\">Porada</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Wizualizator mo\305\274esz r\303\263wnie\305\274</p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">uruchamia\304\207 z linii komend:</p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-weight:600;\">$ ./labwiz plik.out<"
                        "/span></p></body></html>", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
