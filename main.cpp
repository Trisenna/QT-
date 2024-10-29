#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    int MAXSIZE = 12;
    int MAXQUEUE = 5;
    int T=200;
    MainWindow w(MAXSIZE, MAXQUEUE, T);

    w.show();
    return a.exec();
}