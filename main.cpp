#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    int MAXSIZE = 6;
    int Maxqueue = 3;
    int T=200;
    MainWindow w(MAXSIZE, Maxqueue, T);

    w.show();

    return a.exec();
}
