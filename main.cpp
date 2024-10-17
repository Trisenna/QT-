#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    int MAXSIZE = 8;
    int Maxqueue = 4;
    MainWindow w(MAXSIZE, Maxqueue);

    w.show();

    return a.exec();
}
