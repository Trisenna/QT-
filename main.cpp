#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    // 获取 Max 的值
  //  bool ok1;
  //  int Maxqueue = QInputDialog::getInt(nullptr, "输入", "请输入 Max 的值:", 2, 1, 100, 1, &ok1);

    // 如果用户点击了取消按钮，则退出应用
  //  if (!ok1)
   //     return 0;

    // 获取 MAXSIZE 的值
   // bool ok2;
  //  int MAXSIZE = QInputDialog::getInt(nullptr, "输入", "请输入 MAXSIZE 的值:", 6, 1, 100, 1, &ok2);

    // 如果用户点击了取消按钮，则退出应用
   // if (!ok2)
    //    return 0;

    int MAXSIZE = 5;
    int Maxqueue = 3;
    MainWindow w(MAXSIZE, Maxqueue);

    w.show();

    return a.exec();
}
