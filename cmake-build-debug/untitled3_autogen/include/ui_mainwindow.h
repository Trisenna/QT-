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
    QPushButton *pushButton_2;
    QListWidget *listWidget;
    QLabel *label_3;
    QListWidget *listWidget_3;
    QPushButton *leaveButton;
    QLabel *label_5;
    QPushButton *pushButton_8;
    QLabel *inputlabel;
    QLabel *label;
    QLabel *label_4;
    QLabel *label_6;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(982, 695);
        MainWindow->setAutoFillBackground(false);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        lineEdit = new QLineEdit(centralwidget);
        lineEdit->setObjectName("lineEdit");
        lineEdit->setGeometry(QRect(330, 470, 111, 31));
        enterButton = new QPushButton(centralwidget);
        enterButton->setObjectName("enterButton");
        enterButton->setGeometry(QRect(110, 560, 101, 41));
        pushButton_2 = new QPushButton(centralwidget);
        pushButton_2->setObjectName("pushButton_2");
        pushButton_2->setGeometry(QRect(440, 560, 101, 41));
        listWidget = new QListWidget(centralwidget);
        listWidget->setObjectName("listWidget");
        listWidget->setGeometry(QRect(660, 30, 291, 341));
        label_3 = new QLabel(centralwidget);
        label_3->setObjectName("label_3");
        label_3->setGeometry(QRect(0, 180, 16, 31));
        QFont font;
        font.setPointSize(12);
        label_3->setFont(font);
        listWidget_3 = new QListWidget(centralwidget);
        listWidget_3->setObjectName("listWidget_3");
        listWidget_3->setGeometry(QRect(660, 390, 291, 151));
        leaveButton = new QPushButton(centralwidget);
        leaveButton->setObjectName("leaveButton");
        leaveButton->setGeometry(QRect(280, 560, 101, 41));
        label_5 = new QLabel(centralwidget);
        label_5->setObjectName("label_5");
        label_5->setGeometry(QRect(900, 520, 41, 16));
        pushButton_8 = new QPushButton(centralwidget);
        pushButton_8->setObjectName("pushButton_8");
        pushButton_8->setGeometry(QRect(590, 560, 101, 41));
        inputlabel = new QLabel(centralwidget);
        inputlabel->setObjectName("inputlabel");
        inputlabel->setGeometry(QRect(180, 470, 151, 31));
        inputlabel->setFont(font);
        inputlabel->setStyleSheet(QString::fromUtf8("QLabel{\n"
"                                    border: 2px solid \357\274\233\n"
"                                    font:bold italic 16px \"\345\276\256\350\275\257\351\233\205\351\273\221\";\n"
"                                    }\n"
"                                "));
        label = new QLabel(centralwidget);
        label->setObjectName("label");
        label->setGeometry(QRect(30, 420, 61, 41));
        QFont font1;
        font1.setPointSize(18);
        label->setFont(font1);
        label_4 = new QLabel(centralwidget);
        label_4->setObjectName("label_4");
        label_4->setGeometry(QRect(540, 100, 31, 31));
        label_4->setFont(font);
        label_6 = new QLabel(centralwidget);
        label_6->setObjectName("label_6");
        label_6->setGeometry(QRect(0, 210, 21, 31));
        label_6->setFont(font);
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
        pushButton_2->setText(QCoreApplication::translate("MainWindow", "\350\275\246\344\275\215\346\237\245\350\257\242", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "\344\276\277", nullptr));
        leaveButton->setText(QCoreApplication::translate("MainWindow", "\350\275\246\350\276\206\351\200\200\345\207\272", nullptr));
#if QT_CONFIG(shortcut)
        leaveButton->setShortcut(QCoreApplication::translate("MainWindow", "Esc", nullptr));
#endif // QT_CONFIG(shortcut)
        label_5->setText(QCoreApplication::translate("MainWindow", "\350\256\241\350\264\271\345\217\260", nullptr));
        pushButton_8->setText(QCoreApplication::translate("MainWindow", "\345\201\234\350\275\246\345\234\272\346\237\245\350\257\242", nullptr));
        inputlabel->setText(QCoreApplication::translate("MainWindow", "\350\257\267\350\276\223\345\205\245\350\275\246\347\211\214\346\210\226\350\275\246\344\275\215\345\217\267\357\274\232", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "\345\205\245\345\217\243", nullptr));
        label_4->setText(QCoreApplication::translate("MainWindow", "\345\207\272\345\217\243", nullptr));
        label_6->setText(QCoreApplication::translate("MainWindow", "\351\201\223", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
