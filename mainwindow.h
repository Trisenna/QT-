#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "qsequentialanimationgroup.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>
#include <QPropertyAnimation>
#include <QPushButton>
#include <QInputDialog>
#include <iostream>
#include "carqueue.h"
#include "car.h"
#include <QMouseEvent>
#include <QDebug>

#include "rotatablelabel.h"

//停车位总长度
#define LONG 600






QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {

    Q_OBJECT

  public:
    MainWindow(int MAXSIZE, int Maxqueue, QWidget *parent = nullptr)

    ;
    ~MainWindow();

    void startAnimation();
    void animationFinished();
    void startAnimationGroup();
    void Creatimage();

    void createCar();
    void addCar();
    void deleteCar();
    void option();
    void showCar();
    void findCar();
    int getCarNum();
    int MAXSIZE;
    int Maxqueue;
    //临时停车位
    CarQueue carQueuel;
  protected:
    //重写父窗口的鼠标点击方法实现覆盖按钮的点击
    void mousePressEvent(QMouseEvent *event) override {
        // 在父窗口中捕获鼠标点击事件
        QPoint clickPos = event->pos();

        // 判断点击的位置是否在按钮的区域内
        for (QPushButton *button : findChildren<QPushButton * >()) {
            if (button->geometry().contains(clickPos)) {
                button->click();
                // 在这里执行相应的操作
                break;
            }
        }
    }
  private:
    Ui::MainWindow *ui;
    //调试用的两个按钮
    QPushButton *startButton;
    QPushButton *finishButton;

    QPixmap imagePixmap; // 用于存储动画图片
    QPixmap* imageTem;
    QPixmap imageWay;
    QPixmap imageWay2;

    QLabel *imageLabel; // 车辆横向图片
    QLabel *temLable;//车辆纵向图片
    RotatableLabel *rotationLable;//车辆旋转图片
    QLabel *imageWayLable;
    QLabel *imageWayLable2;
    QLabel*tem;

    QPropertyAnimation *animation,*animation0,*animation1,*animation2,*animation3, *rotationAnimation;

    QSequentialAnimationGroup animationGroup,animationGroup2;

    QVector<QLabel *>* previousImagesList;//停车位按钮上的图片
    QVector<QLabel *>* previousImagesList3;//停车场车辆的图片
    QVector<QLabel *>* previousImagesList2;//便道按钮上的图片
    QVector<QLabel *>* previousImagesList4;//便道车辆的图片
    QLabel *previousImage;//用于列表的删除的图片
    int parkingSpaceCount; // 停车位数量
    int parkingSpaceWidth; // 停车位宽度

    QHBoxLayout *parkingLayout; // 水平布局管理器
    QHBoxLayout *ButtonLayout; // 水平布局管理器
    QWidget *parkingWidget; // 用于承载水平布局的 QWidget
    QVBoxLayout *Vparkinglayout;
    QWidget * VparkingWidget;
    QVBoxLayout *mainLayout; // 主垂直布局管理器
    QSpacerItem *verticalSpacer; // 垂直间隔控件
    QWidget *ButtonWeight;

    //停车位
    std::vector<Car> carArray;
    //判断此处是否有车
    std::vector<bool> parkingSpaces;

    //动画使用的数组及其长宽高,停车场总长度为LONG,最大停车数MAX
    //定义每一个停车位的长度
    int parkingLong ;
    int parkingWid ;

  public slots:
    void onNameButtonClicked();
    void onFindButtonClicked();
    void onShowButtonClicked();
    void onLeaveButtonClicked();
    void onButtonClickedW();
    void onButtonClickedP();


};
#endif // MAINWINDOW_H
