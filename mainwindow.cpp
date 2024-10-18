#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QtCore>


//0为停车位进车，1为只有停车位出车，2为队列进车，3为停车位出车，队列进车，4为队列车辆
static int start = 0;
//停车位
static int spot = 0;
//队列位置
static int lineNum = 0;

//构造函数
MainWindow::MainWindow(int MAXSIZE1, int Maxqueue1,int T,  QWidget *parent) : QMainWindow(parent)  ,
     ui(new Ui::MainWindow)
{

    ui->setupUi(this);

    this->MAXSIZE = MAXSIZE1;
    this->Maxqueue = Maxqueue1;
    this->T = T;
    setWindowTitle("停车场管理系统");


    //车库
    QPushButton* locate[MAXSIZE];
    //便道
    QPushButton* locate2[Maxqueue];

    //动画使用的数组及其长宽高,停车场总长度为LONG,最大停车数MAX。定义每一个停车位的长度
    parkingLong = LONG / MAXSIZE;
    parkingWid = 30 * parkingLong / 54;

    // 初始化停车位数组，所有位置都设置为未占用（false）
    parkingSpaces.resize(MAXSIZE, false);

    // 创建按钮布局
    ButtonLayout = new QHBoxLayout();

    // 初始化停车位按钮
    for(int i = 1; i <= MAXSIZE; i++) {
        QString s = "停";
        s.append(QString::number(i));
        locate[i - 1] = new QPushButton(this);
        // 设置按钮的文本为 "停车位" 加上编号
        locate[i - 1]->setText(s);
        // 调整按钮的大小
        locate[i - 1]->resize(parkingLong, parkingWid);
        // 设置按钮的位置
        if (i <= MAXSIZE / 2) {
            // 上排
            locate[i - 1]->setGeometry((i - 1) * parkingLong + 120, 50, parkingLong, parkingWid);
        } else {
            // 下排
            locate[i - 1]->setGeometry((i - 1 - MAXSIZE / 2) * parkingLong + 120, 120 + parkingWid, parkingLong, parkingWid);
        }
        // 设置按钮遮罩（使得只有定义的区域可点击）
        QRect rect(0, 0,  parkingLong, parkingWid);
        QRegion region(rect, QRegion::Rectangle);
        locate[i - 1]->setMask(region);
        // 连接按钮的点击信号到槽函数 onButtonClickedP
        connect(locate[i - 1], &QPushButton::clicked, this, &MainWindow::onButtonClickedP);
    }

    //便道
    for(int i = 1; i <= Maxqueue; i++) {
        QString s = "候";
        s.append(QString::number(i));
        locate2[i - 1] = new QPushButton(this);
        locate2[i - 1]->setText(s);
        locate2[i - 1]->resize(parkingWid, parkingLong);
        // 设置按钮的位置
        if (i == 1) {
            // 第一辆车的位置在停车库上下两排的中间
            locate2[i - 1]->setGeometry(40, 75 + parkingWid / 2, parkingWid, parkingLong);
        } else {
            // 其他车的位置
            locate2[i - 1]->setGeometry(40, 75 + parkingWid / 2 + parkingLong * (i - 1), parkingWid, parkingLong);
        }
        // 设置按钮遮罩
        QRect rect(0, 0, parkingWid, parkingLong);
        QRegion region(rect, QRegion::Rectangle);
        locate2[i - 1]->setMask(region);
        connect(locate2[i - 1], &QPushButton::clicked, this, &MainWindow::onButtonClickedW);
    }

    //初始化汽车容器
    for(int i = 0; i < MAXSIZE; i++) {
        Car* newCar = new Car("", -1, QDateTime::currentDateTime());
        carArray.insert(carArray.begin() + i, *newCar);
    }

    // 设置布局
    ButtonWeight = new QWidget(this);
    ButtonWeight->setLayout(ButtonLayout);
    ButtonWeight->setGeometry(200, 10, 700, 50);

    // 创建水平布局管理器和承载 QWidget用于存放图片
    parkingLayout = new QHBoxLayout();
    parkingWidget = new QWidget(this);
    parkingWidget->setLayout(parkingLayout);
    parkingWidget->setGeometry(100, 100, 1000, 50);

    Vparkinglayout = new QVBoxLayout();
    VparkingWidget = new QWidget(this);
    VparkingWidget->setLayout(Vparkinglayout);
    VparkingWidget->setGeometry(50, 100, 100, 500);

    //动画调试按钮
    startButton = new QPushButton("Start", this);
    startButton->setGeometry(10, 10, 50, 30);
    finishButton = new QPushButton("Finish", this);
    finishButton->setGeometry(200, 10, 50, 30);


   // startButton->setVisible(false);
   // finishButton->setVisible(false);



    //用于动画的图片
    rightLabel = new QLabel(this);
    upLable = new QLabel(this);
    downLable = new QLabel(this);
    //RotatableLabel* rotationLable = new RotatableLabel(this);

    //设置图片
    QPixmap newPixmap(":car1.png");//向右
    QPixmap newPixmap2(":car2.png");//向上
    QPixmap newPixmap3(":car3.png");//向下
    newPixmap = newPixmap.scaled(parkingLong, parkingWid);
    newPixmap2 = newPixmap2.scaled(parkingWid, parkingLong);
    newPixmap3 = newPixmap3.scaled(parkingWid,parkingLong);

    upLable->setPixmap(newPixmap2);//向上
    rightLabel->setPixmap(newPixmap);//向右
    downLable->setPixmap(newPixmap3);//向下

    rightLabel->setScaledContents(true);
    upLable->setScaledContents(true);
    downLable->setScaledContents(true);
    //rotationLable->setScaledContents(true);

    rightLabel->setVisible(false);
    upLable->setVisible(false);
    downLable->setVisible(false);
    //rotationLable->setVisible(false);

   // tem->move(100, 200);
    //tem->setPixmap(newPixmap);

    //图片集合
    previousImagesList = new QList<QLabel *>();
    previousImagesList2 = new QList<QLabel *>();
    previousImagesList4 = new QList<QLabel *>();
    previousImagesList3 = new QList<QLabel *>();

    //停车位的车辆图片
    for(int i = 0; i < MAXSIZE; i++) {
        previousImage = new QLabel(this);
        previousImage->setPixmap(rightLabel->pixmap());
        if(i<MAXSIZE/2) {
            previousImage->setGeometry(120 + parkingLong * i, 50, parkingLong, parkingWid);
        } else {
            previousImage->setGeometry(120 + parkingLong * (i-MAXSIZE/2), 120 + parkingWid, parkingLong, parkingWid);
        }

        previousImage->setVisible(false);

        previousImagesList->append(previousImage);
    }
    //便道的车辆图片
    for(int i = 0; i < Maxqueue; i++) {
        previousImage = new QLabel(this);
        previousImage->setPixmap(upLable->pixmap());

        if (i == 0) {
            // 第一辆车的位置在停车库上下两排的中间
            previousImage->setGeometry(40, 75 + parkingWid / 2, parkingWid, parkingLong);
        } else {
            // 其他车的位置
            previousImage->setGeometry(40, 75 + parkingWid / 2 + parkingLong * i, parkingWid, parkingLong);
        }
        previousImage->setVisible(false);
        previousImagesList2->append(previousImage);
    }



    //停车动画
    connect(startButton, &QPushButton::clicked, [this, newPixmap2, newPixmap3, T]() {

        //0为停车位进车
        if(previousImagesList3->size() < MAXSIZE) {
            start = 0;
            //将动画的图片显示
            rightLabel->setVisible(true);
            upLable->setVisible(true);
            downLable->setVisible(true);
            //rotationLable->setVisible(true);
            //设置在界面外
            rightLabel->setGeometry(-400, -100, parkingLong, parkingWid);
            upLable->setGeometry(-400, -100, parkingWid, parkingLong);
            downLable->setGeometry(-400, -100, parkingWid, parkingLong);


            // 停止并清空之前的动画组
            animationGroup.stop();
            animationGroup.clear();

            // 创建新的动画
            animation0 = new QPropertyAnimation(upLable, "geometry");
           animation0->setDuration(T);
           animation0->setStartValue(QRect(40, 1000, parkingWid, parkingLong));
            animation0->setEndValue(QRect(40, 80, parkingWid, parkingLong));

            animation = new QPropertyAnimation(rightLabel, "geometry");
            animation->setDuration(T);
            animation->setStartValue(QRect(20, 100, parkingLong, parkingWid));
            animation->setEndValue(QRect(120 + parkingLong * (spot % (MAXSIZE / 2)), 100, parkingLong, parkingWid));

            //倒车入库
            int yCoordinate;
            if (spot < MAXSIZE / 2) {
           // 第一排使用 downLable
            yCoordinate = 50;
             } else {
             // 第二排使用 upLable
             downLable->setPixmap(newPixmap2);
             yCoordinate = 120 + parkingWid;
            }
            animation2 = new QPropertyAnimation(downLable, "geometry");
            animation2->setDuration(T);
            animation2->setStartValue(QRect(120 + parkingLong * (spot % (MAXSIZE / 2)), 100, parkingLong, parkingWid));
            animation2->setEndValue(QRect(120 + parkingLong * (spot % (MAXSIZE / 2)), yCoordinate, parkingLong, parkingWid));

            // 添加新的动画到动画组
            animationGroup.addAnimation(animation0);
            //animationGroup.addAnimation(rotationAnimation);
            animationGroup.addAnimation(animation);
            animationGroup.addAnimation(animation2);

            // 连接动画完成信号到隐藏相应的标签
connect(animation0, &QPropertyAnimation::finished, upLable, &QLabel::hide);
connect(animation, &QPropertyAnimation::finished, rightLabel, &QLabel::hide);
            connect(animation2, &QPropertyAnimation::finished, [this, newPixmap3]() {
    if (spot >= MAXSIZE / 2) {
        downLable->setPixmap(newPixmap3);
    }
});
connect(animation2, &QPropertyAnimation::finished, [this]() {
    if (spot < MAXSIZE / 2) {
        downLable->hide();
    } else {
        upLable->hide();
    }
});
            // 开始新的动画
            animationGroup.start();

        }

        //2为便道队列进车
        else if(previousImagesList4->size() <= Maxqueue) {
            start = 2;
            upLable->setVisible(true);

            // 停止并清空之前的动画组
            animationGroup.stop();
            animationGroup.clear();

            // 创建新的动画
            animation = new QPropertyAnimation(upLable, "geometry");
            animation->setDuration(T);
            animation->setStartValue(QRect(40, 1000, parkingWid, parkingLong));
            animation->setEndValue(QRect(40, 20+parkingLong * lineNum, parkingWid, parkingLong));

            // 添加新的动画到动画组
            animationGroup.addAnimation(animation);

            // 开始新的动画
            animationGroup.start();
        }

    });

    // 连接动画完成信号到槽函数，更新车辆的图片
    connect(&animationGroup, &QSequentialAnimationGroup::finished, [this]() {
        //利用start实现只有当开始按钮被点击后才有图片添加
        if(start == 0) {
            previousImage = new QLabel(this);
            previousImage->setPixmap(rightLabel->pixmap());
            previousImage->setVisible(false);
            previousImagesList3->append(previousImage);
            //显示停车位按钮上的图片
            previousImagesList->at(spot)->setVisible(true);

            // 更新布局
            parkingWidget->update();
        }

        else if(start == 2) {
            previousImage = new QLabel(this);
            previousImage->setPixmap(upLable->pixmap());
             previousImage->setVisible(false);
            previousImagesList4->append(previousImage);
            //显示便道按钮上的图片
            previousImagesList2->at(lineNum - 1)->setVisible(true);

            // 更新布局
            VparkingWidget->update();
        }
        else if(start == 3) {
            previousImage = new QLabel(this);
            previousImage->setPixmap(rightLabel->pixmap());
            previousImage->setVisible(false);
            previousImagesList3->append(previousImage);
            //在车辆出库后，立即显示从队列中移入的新车图片，确保UI始终反映停车场的最新状态。
            previousImagesList->at(spot)->setVisible(true);
            // 更新布局
            parkingWidget->update();

        }

        //隐藏动画的图片
        rightLabel->setVisible(false);
        upLable->setVisible(false);
        downLable->setVisible(false);
    });

    //离开动画
    connect(finishButton, &QPushButton::clicked, [this,T, newPixmap2, newPixmap3]() {
        // start = 1;//只有停车位出车
        if ( !previousImagesList3->isEmpty() && previousImagesList4->isEmpty()) {
            rightLabel->setVisible(true);
            upLable->setVisible(true);
            downLable->setVisible(true);
            //设置位置
            rightLabel->setGeometry(-400, -100, parkingLong, parkingWid);
            upLable->setGeometry(-400, -100, parkingWid, parkingLong);
            downLable->setGeometry(-400, -100, parkingWid, parkingLong);

            start = 1;//只有停车位出车
            previousImagesList->at(spot)->setVisible(false);
            previousImagesList3->pop_back();

            // 获取并移除最后一个图片
            animationGroup.stop();
            animationGroup.clear();

            // 进入过道的动画
            animation0 = new QPropertyAnimation(downLable, "geometry");
            animation0->setDuration(T);
            int yCoordinate;
            if (spot < MAXSIZE / 2) {
             // First row
                yCoordinate = 50;

                } else {
                // Second row
                    downLable->setPixmap(newPixmap2);
                  yCoordinate =  120 + parkingWid;
                    }
            animation0->setStartValue(QRect(120 + parkingLong * (spot % (MAXSIZE / 2)), yCoordinate, parkingLong, parkingWid));
            animation0->setEndValue(QRect(120 + parkingLong * (spot % (MAXSIZE / 2)), 100, parkingLong, parkingWid));


            // 由过道出车库的动画
           animation1 = new QPropertyAnimation(rightLabel, "geometry");
           animation1->setDuration(T);
           animation1->setStartValue(QRect(120 + parkingLong * (spot % (MAXSIZE / 2)), 100, parkingLong, parkingWid));
            animation1->setEndValue(QRect(500, 100, parkingLong, parkingWid));

            //向下出车库的动画
            animation2 = new QPropertyAnimation(downLable, "geometry");
            animation2->setDuration(T);
            animation2->setStartValue(QRect(500, 100, parkingWid, parkingLong));
            animation2->setEndValue(QRect(500, 1000, parkingWid, parkingLong));


            // 添加新的动画到动画组
            animationGroup.addAnimation(animation0);
            animationGroup.addAnimation(animation1);
            animationGroup.addAnimation(animation2);

            // 连接动画完成信号到隐藏相应的标签
            connect(animation0, &QPropertyAnimation::finished, [this]() {
                downLable->hide();
                rightLabel->show();
                animation1->start();
            });
            // 连接动画完成信号到隐藏相应的标签
            connect(animation1, &QPropertyAnimation::finished, [this]() {
            rightLabel->hide();
                downLable->show();
            animation2->start();
            });
            connect(animation2, &QPropertyAnimation::finished, downLable, &QLabel::hide);


            // 开始初始动画
            animationGroup.start();

            // 更新布局
            parkingWidget->update();

        }

        //start = 3：停车位出车，队列进车
        else if( !previousImagesList3->isEmpty() && !previousImagesList4->isEmpty()) {
            rightLabel->setVisible(true);
            upLable->setVisible(true);
            downLable->setVisible(true);
            rightLabel->setGeometry(-400, -100, parkingLong, parkingWid);
            upLable->setGeometry(-400, -100, parkingWid, parkingLong);
            downLable->setGeometry(-400, -100, parkingWid, parkingLong);

            start = 3;
            // Hide the car images in the parking space and queue
         previousImagesList->at(spot)->setVisible(false);


         // Remove the last car images from the lists
         previousImagesList3->pop_back();
         previousImagesList4->pop_back();

            // 获取并移除最后一个图片
            animationGroup.stop();
            animationGroup.clear();


            // 停车场车辆出库，进入过道
             animation0 = new QPropertyAnimation(downLable, "geometry");
             animation0->setDuration(500);
             int yCoordinate;
             if (spot < MAXSIZE / 2) {
              // First row
                 yCoordinate = 50;

                 } else {
                 // Second row
                     downLable->setPixmap(newPixmap2);
                   yCoordinate =  120 + parkingWid;
                     }
             animation0->setStartValue(QRect(120 + parkingLong * (spot % (MAXSIZE / 2)), yCoordinate, parkingLong, parkingWid));
             animation0->setEndValue(QRect(120 + parkingLong * (spot % (MAXSIZE / 2)), 100, parkingLong, parkingWid));


             // 由过道出车库的动画
            animation1 = new QPropertyAnimation(rightLabel, "geometry");
            animation1->setDuration(500);
            animation1->setStartValue(QRect(120 + parkingLong * (spot % (MAXSIZE / 2)), 100, parkingLong, parkingWid));
             animation1->setEndValue(QRect(500, 100, parkingLong, parkingWid));

             //向下出车库的动画
             animation2 = new QPropertyAnimation(downLable, "geometry");
             animation2->setDuration(500);
             animation2->setStartValue(QRect(500, 100, parkingWid, parkingLong));
             animation2->setEndValue(QRect(500, 1000, parkingWid, parkingLong));

            //队列入库
            animation = new QPropertyAnimation(rightLabel, "geometry");
            animation->setDuration(500);
            animation->setStartValue(QRect(20, 100, parkingLong, parkingWid));
            animation->setEndValue(QRect(120 + parkingLong * (spot % (MAXSIZE / 2)), 100, parkingLong, parkingWid));

            //倒车入库
            if (spot < MAXSIZE / 2) {
           // 第一排使用 downLable
            yCoordinate = 50;
             } else {
             // 第二排使用 upLable
             downLable->setPixmap(newPixmap2);
             yCoordinate = 120 + parkingWid;
            }
            animation4 = new QPropertyAnimation(downLable, "geometry");
            animation4->setDuration(500);
            animation4->setStartValue(QRect(120 + parkingLong * (spot % (MAXSIZE / 2)), 100, parkingLong, parkingWid));
            animation4->setEndValue(QRect(120 + parkingLong * (spot % (MAXSIZE / 2)), yCoordinate, parkingLong, parkingWid));

              // Add the animations to the animation group
              animationGroup.addAnimation(animation0);
              animationGroup.addAnimation(animation1);
              animationGroup.addAnimation(animation2);
            animationGroup.addAnimation(animation);
            animationGroup.addAnimation(animation4);

            // 连接动画完成信号到隐藏相应的标签
connect(animation0, &QPropertyAnimation::finished, [this]() {
    downLable->hide();
    rightLabel->show();
    animation1->start();
});
connect(animation1, &QPropertyAnimation::finished, [this]() {
    rightLabel->hide();
    downLable->show();
    animation2->start();
});
            //移除队列第一张图片并显示队列车辆补位的动画
connect(animation2, &QPropertyAnimation::finished, [this]() {
    if (previousImagesList2->isEmpty()) {
        return;
    }
    previousImagesList2->at(0)->setVisible(false);
    previousImagesList2->removeFirst(); // Remove the first car in the queue

    // Start the next animation for the remaining cars in the queue
    auto queueAdvanceGroup = new QSequentialAnimationGroup(this);
    connect(queueAdvanceGroup, &QSequentialAnimationGroup::finished, queueAdvanceGroup, &QObject::deleteLater); // Ensure proper deletion

    for (int i = 0; i < previousImagesList2->size(); ++i) {
          auto advanceAnimation = new QPropertyAnimation(previousImagesList2->at(i), "geometry");
        advanceAnimation->setDuration(500);
        advanceAnimation->setEasingCurve(QEasingCurve::InOutQuad); // Apply easing curve
        advanceAnimation->setStartValue(previousImagesList2->at(i)->geometry());
        advanceAnimation->setEndValue(QRect(40, 75 + parkingWid / 2 + parkingLong * i, parkingWid, parkingLong));
        queueAdvanceGroup->addAnimation(advanceAnimation);
    }
    queueAdvanceGroup->start();
});

connect(animation2, &QPropertyAnimation::finished, [this]() {
    downLable->hide();
    rightLabel->show();
    animation->start();
});
connect(animation, &QPropertyAnimation::finished, [this]() {
    rightLabel->hide();
    downLable->show();
    animation4->start();
});
            connect(animation4, &QPropertyAnimation::finished, [this]() {
                downLable->hide();
            });

      // Start the animation group
      animationGroup.start();

      // Update the layout
      parkingWidget->update();
      VparkingWidget->update();
        }

    });

    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(onNameButtonClicked()));
    connect(ui->pushButton_2, SIGNAL(clicked()), this, SLOT(onFindButtonClicked()));
    connect(ui->pushButton_8, SIGNAL(clicked()), this, SLOT(onShowButtonClicked()));
    connect(ui->pushButton_4, SIGNAL(clicked()), this, SLOT(onLeaveButtonClicked()));
}



// 实现槽函数
void MainWindow::onNameButtonClicked() {
    // 在这里编写与 "进入" 按钮相关的操作
    createCar();
}

void MainWindow::onFindButtonClicked() {
    // 在这里编写与 "查找" 按钮相关的操作
    findCar();
}

void MainWindow::onShowButtonClicked() {
    // 在这里编写与 "车库余位查询" 按钮相关的操作
    showCar();
    //ip++;
   // tem->move(100 * ip, 500);
}

void MainWindow::onLeaveButtonClicked() {
    // 在这里编写与 "离开" 按钮相关的操作
    deleteCar();
    addCar();
}
//析构函数
MainWindow::~MainWindow() {
    delete ui;
}

//0为停车位进车，2为队列进车，1为只有停车位出车，3为停车位出车，队列进车

void MainWindow::createCar() {
    QString st = ui->lineEdit->text();
    if(st != "") {
        Car* newCar = new Car(st, -1, QDateTime::currentDateTime());
        ui->lineEdit->clear();


        // 检查数组是否已满
        int i = 0;
        int sum = 0;
        while(sum < MAXSIZE) {
            if(parkingSpaces[sum]) {
                i++;
            }
            sum++;
        }
        if (i >= MAXSIZE) {
            // 数组已满，显示提示或执行其他操作
            if(carQueuel.getsize() < Maxqueue) {

                lineNum++;
                carQueuel.insert(newCar);
                ui->listWidget->addItem("停车场已满," + st + "进入便道\n");
                startButton->click();
            } else {
               ui->listWidget->addItem("停车场已满，便道已满，禁止进入\n");
            }

        } else {

            i = 0;
            // 将新的 Car 对象加入数组
            while(i < MAXSIZE) {
                if(parkingSpaces[i] == false) {
                    newCar->spot = i;
                    spot = i;

                    carArray[i].entreTime = newCar->entreTime;
                    carArray[i].spot = newCar->spot;
                    carArray[i].license = newCar->license;
                    parkingSpaces[i] = true;
                    ui->listWidget->addItem(carArray[i].license + "进入停车场\n");
                    break;
                }
                i++;
            }
            startButton->click();
        }
    }

}
void MainWindow::addCar() {
    if(!carQueuel.isEmpty()) {
        int i = 0;
        while(i <= MAXSIZE) {
            if(parkingSpaces[i] == false) {
                Car* c = carQueuel.get();

                spot = i;
                c->spot = i;
                c->entreTime = QDateTime::currentDateTime();
                carArray[i].entreTime = c->entreTime;
                carArray[i].spot = c->spot;
                carArray[i].license = c->license;
                parkingSpaces[i] = true;
                ui->listWidget->addItem(c->license + "进入停车场\n");
                break;
            }
            i++;
        }

    }
}
void MainWindow::deleteCar() {
    QString s = ui->lineEdit->text();
    bool ok;
    int i = s.toInt(&ok);
    ui->lineEdit->clear();

    if (ok && i > 0 && i <= MAXSIZE) {
        i--;

        if (parkingSpaces[i]) {
            // 直接使用 std::vector 的 erase 函数删除车辆
            spot = i;

            parkingSpaces[i] = false;
            QDateTime currentTime = QDateTime::currentDateTime();
            QDateTime entryTime = carArray[i].entreTime;
            int secondsParked = entryTime.secsTo(currentTime);
            double cost = secondsParked*0.01; // 一秒钟一分钱的费率

            ui->listWidget->addItem("第" + QString::number(i + 1) + "辆车    " + "车牌号:" + carArray[i].license + "    离开"
              "\n停：" + QString::number(secondsParked) + "秒,缴费：" + QString::number(cost) + "元\n");
            // 标记车辆为无效或已删除，而不是从 std::vector 中删除
            carArray[i].license = ""; // 或者其他方式标记为无效
            carArray[i].spot = -1; // 或者其他方式标记为无效
            if(!carQueuel.isEmpty()) {
                lineNum--;
            }
            finishButton->click();

        } else {
            ui->listWidget->addItem("此处无车");
        }
    } else {
        ui->listWidget->addItem("无效的整数字符串");
    }
}
void MainWindow::showCar() {
    int num = 0;
    carQueuel.show();
    ui->listWidget->addItem("停车场:\n");
    for (int i = 0; i < MAXSIZE; i++) {
        if (parkingSpaces[i]) {
            num++;
            ui->listWidget->addItem("第" + QString::number(i + 1) + "个停车位    " + "车牌号:" + carArray[i].license + "     入库时间:"  + carArray[i].entreTime.toString("yyyy-MM-dd HH:mm:ss") + "\n");
        }
    }
    if(num == 0) {
        ui->listWidget->addItem("无车\n");
    }

    ui->listWidget->addItem("便道上:\n");
    Car* tem = carQueuel.getHead();
    int i = 1;
    if(tem != NULL) {
        while (tem != NULL) {
            if (!tem->license.isEmpty() && tem->entreTime.isValid()) {
                ui->listWidget->addItem("第" + QString::number(i) + "辆等候车辆    " + "车牌号:" + tem->license  + "\n");
                i++;
            }
            tem = tem->next;
        }
    } else {
        ui->listWidget->addItem("无车\n");
    }

    ui->listWidget->addItem("停车场内有:" + QString::number(num)+ "辆车");
    ui->listWidget->addItem("剩余位置:" + QString::number(MAXSIZE - num)+ "个");
    ui->listWidget->addItem("----------------------------------------------");
}
void MainWindow::findCar() {
    QString s = ui->lineEdit->text();
    ui->lineEdit->clear();
    bool ok = false;

    // Check if the input is a valid integer (parking space number)
    bool isNumber;
    int parkingSpace = s.toInt(&isNumber);

    if (isNumber && parkingSpace > 0 && parkingSpace <= MAXSIZE) {
        // Query by parking space number
        int index = parkingSpace - 1;
        if (parkingSpaces[index]) {
            ok = true;
            ui->listWidget->addItem("第" + QString::number(parkingSpace) + "个停车位    " + "车牌号:" + carArray[index].license + "     入库时间:"  + carArray[index].entreTime.toString("yyyy-MM-dd HH:mm:ss") + "\n");
        } else {
            ui->listWidget->addItem("----------------------------------------------------------\n");
            ui->listWidget->addItem("此处无车\n");
        }
    } else {
        // Query by license plate number
        for (int i = 0; i < MAXSIZE; i++) {
            if (parkingSpaces[i] && s == carArray[i].license) {
                ok = true;
                ui->listWidget->addItem("第" + QString::number(i + 1) + "个停车位    " + "车牌号:" + carArray[i].license + "     入库时间:"  + carArray[i].entreTime.toString("yyyy-MM-dd HH:mm:ss") + "\n");
                break;
            }
        }

        // If not found in parking spaces, check the queue
        if (!ok) {
            Car* tem = carQueuel.getHead();
            int i = 1;
            while (tem != NULL) {
                if (s == tem->license) {
                    ok = true;
                    ui->listWidget->addItem("第" + QString::number(i) + "辆等候车辆    " + "车牌号:" + tem->license  + "\n");
                    break;
                }
                tem = tem->next;
                i++;
            }
        }
    }

    if (!ok) {
        ui->listWidget->addItem("无效的车牌号或车位号\n");
    }
}
int MainWindow::getCarNum() {
    int i = 0;
    int sum = 0;
    while(sum < MAXSIZE) {
        if(parkingSpaces[sum]) {
            i++;
        }
        sum++;
    }
    std::cout << i;
    return i;
}
void MainWindow::onButtonClickedP() {
    QPushButton *clickedButton = qobject_cast<QPushButton*>(sender());
    if (clickedButton) {
        QString buttonText = clickedButton->text();
        // 提取数字
        QString numberString = buttonText.mid(1);
        bool ok;
        int number = numberString.toInt(&ok);

        if (ok && parkingSpaces[number - 1]) {
            // Remove the car from the parking space
            int index = number - 1;
            parkingSpaces[index] = false;
            QDateTime currentTime = QDateTime::currentDateTime();
            QDateTime entryTime = carArray[index].entreTime;
            int secondsParked = entryTime.secsTo(currentTime);
            double cost = secondsParked * 0.01; // 一秒钟一分钱的费率

            ui->listWidget->addItem("第" + QString::number(number) + "个停车位    " + "车牌号:" + carArray[index].license + "    离开"
              "\n停：" + QString::number(secondsParked) + "秒,缴费：" + QString::number(cost) + "元\n");

            // Clear the car data
            carArray[index].license = "";
            carArray[index].spot = -1;

            // Trigger the leave animation
            spot = index;
            finishButton->click();
        } else {
            ui->listWidget->addItem("----------------------------------------------------------\n");
            ui->listWidget->addItem("此处无车\n");
        }
    }
}//点击停车场停车位按钮出库
void MainWindow::onButtonClickedW() {
    QPushButton *clickedButton = qobject_cast<QPushButton*>(sender());
    if (clickedButton) {
        ui->listWidget->addItem("----------------------------------------------------------\n");
        ui->listWidget->addItem("便道车辆信息:\n");

        Car* tem = carQueuel.getHead();
        int i = 1;

        while (tem != nullptr) {
            ui->listWidget->addItem("便道第" + QString::number(i) + "辆等候车辆    " + "车牌号:" + tem->license + "\n");
            tem = tem->next;
            i++;
        }

        if (i == 1) {
            ui->listWidget->addItem("便道上无车\n");
        }

        ui->listWidget->addItem("----------------------------------------------------------\n");
    }
}
