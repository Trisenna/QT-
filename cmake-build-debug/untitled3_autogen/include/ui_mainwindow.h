/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.5.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QLineEdit *lineEdit;
    QPushButton *enterButton;
    QPushButton *queryButton_1;
    QListWidget *listWidget;
    QListWidget *listWidget_3;
    QPushButton *leaveButton;
    QLabel *label_5;
    QPushButton *queryButton_2;
    QLabel *inputlabel;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_4;
    QLabel *label_7;
    QListWidget *listWidget_2;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(982, 652);
        MainWindow->setAutoFillBackground(false);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        lineEdit = new QLineEdit(centralwidget);
        lineEdit->setObjectName("lineEdit");
        lineEdit->setGeometry(QRect(280, 420, 111, 31));
        enterButton = new QPushButton(centralwidget);
        enterButton->setObjectName("enterButton");
        enterButton->setGeometry(QRect(140, 470, 91, 41));
        queryButton_1 = new QPushButton(centralwidget);
        queryButton_1->setObjectName("queryButton_1");
        queryButton_1->setGeometry(QRect(140, 520, 91, 41));
        listWidget = new QListWidget(centralwidget);
        listWidget->setObjectName("listWidget");
        listWidget->setGeometry(QRect(640, 40, 261, 341));
        listWidget_3 = new QListWidget(centralwidget);
        listWidget_3->setObjectName("listWidget_3");
        listWidget_3->setGeometry(QRect(640, 390, 261, 151));
        leaveButton = new QPushButton(centralwidget);
        leaveButton->setObjectName("leaveButton");
        leaveButton->setGeometry(QRect(250, 470, 91, 41));
        label_5 = new QLabel(centralwidget);
        label_5->setObjectName("label_5");
        label_5->setGeometry(QRect(860, 520, 41, 16));
        queryButton_2 = new QPushButton(centralwidget);
        queryButton_2->setObjectName("queryButton_2");
        queryButton_2->setGeometry(QRect(250, 520, 91, 41));
        inputlabel = new QLabel(centralwidget);
        inputlabel->setObjectName("inputlabel");
        inputlabel->setGeometry(QRect(130, 420, 151, 31));
        QFont font;
        font.setPointSize(12);
        inputlabel->setFont(font);
        inputlabel->setStyleSheet(QString::fromUtf8("QLabel{\n"
"                                    border: 2px solid \357\274\233\n"
"                                    font:bold italic 16px \"\345\276\256\350\275\257\351\233\205\351\273\221\";\n"
"                                    }\n"
"                                "));
        label = new QLabel(centralwidget);
        label->setObjectName("label");
        label->setGeometry(QRect(20, 520, 91, 41));
        label->setFont(font);
        label->setPixmap(QPixmap(QString::fromUtf8("C:/Users/lenovo/Desktop/in.png")));
        label->setScaledContents(true);
        label_2 = new QLabel(centralwidget);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(860, 360, 41, 16));
        label_4 = new QLabel(centralwidget);
        label_4->setObjectName("label_4");
        label_4->setGeometry(QRect(590, 60, 31, 71));
        label_4->setPixmap(QPixmap(QString::fromUtf8("C:/Users/lenovo/Pictures/Screenshots/\345\261\217\345\271\225\346\210\252\345\233\276 2024-10-24 174346(1).png")));
        label_4->setScaledContents(true);
        label_7 = new QLabel(centralwidget);
        label_7->setObjectName("label_7");
        label_7->setGeometry(QRect(10, 550, 91, 31));
        label_7->setPixmap(QPixmap(QString::fromUtf8("C:/Users/lenovo/Pictures/Screenshots/\345\261\217\345\271\225\346\210\252\345\233\276 2024-10-24 174632.png")));
        label_7->setScaledContents(true);
        listWidget_2 = new QListWidget(centralwidget);
        listWidget_2->setObjectName("listWidget_2");
        listWidget_2->setGeometry(QRect(370, 290, 121, 81));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 982, 22));
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
        enterButton->setText(QCoreApplication::translate("MainWindow", "\350\275\246\350\276\206\350\277\233\345\205\245", nullptr));
#if QT_CONFIG(shortcut)
        enterButton->setShortcut(QCoreApplication::translate("MainWindow", "Up", nullptr));
#endif // QT_CONFIG(shortcut)
        queryButton_1->setText(QCoreApplication::translate("MainWindow", "\350\275\246\344\275\215\346\237\245\350\257\242", nullptr));
        leaveButton->setText(QCoreApplication::translate("MainWindow", "\350\275\246\350\276\206\351\200\200\345\207\272", nullptr));
#if QT_CONFIG(shortcut)
        leaveButton->setShortcut(QCoreApplication::translate("MainWindow", "Down", nullptr));
#endif // QT_CONFIG(shortcut)
        label_5->setText(QCoreApplication::translate("MainWindow", "\350\256\241\350\264\271\345\217\260", nullptr));
        queryButton_2->setText(QCoreApplication::translate("MainWindow", "\345\201\234\350\275\246\345\234\272\346\237\245\350\257\242", nullptr));
        inputlabel->setText(QCoreApplication::translate("MainWindow", "\350\257\267\350\276\223\345\205\245\350\275\246\347\211\214\346\210\226\350\275\246\344\275\215\345\217\267\357\274\232", nullptr));
        label->setText(QString());
        label_2->setText(QCoreApplication::translate("MainWindow", "\346\216\247\345\210\266\345\217\260", nullptr));
        label_4->setText(QString());
        label_7->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
