#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QtCore>


//0为停车位进车，1为只有停车位出车，2为队列进车，3为停车位出车，队列进车
int start = 0;
//停车位
static int spot = 0;
//队列位置
static int lineNum = 0;

//构造函数
MainWindow::MainWindow(int MAXSIZE1, int Maxqueue1,int T,  QWidget *parent) : QMainWindow(parent)  ,
     ui(new Ui::MainWindow) {
    ui->setupUi(this);

    this->MAXSIZE = MAXSIZE1;
    this->MAXQUEUE = Maxqueue1;
    this->T = T;
    setWindowTitle("停车场管理系统");
    //车库
    QPushButton* parkingLocate[MAXSIZE];
    //便道
    QPushButton* waitingLocate[MAXQUEUE];

    queueAdvanceGroup = new QSequentialAnimationGroup(this);

    //动画使用的数组及其长宽高,停车场总长度为LONG,最大停车数MAX。定义每一个停车位的长度
    parkingLong = LONG / MAXSIZE;
    parkingWid = 30 * parkingLong / 54;

    // 创建按钮布局
    ButtonLayout = new QHBoxLayout();

    // 初始化停车位按钮
    for(int i = 1; i <= MAXSIZE; i++) {
        QString s = "停\n";
        s.append(QString::number(i));
        parkingLocate[i - 1] = new QPushButton(this);
        // 设置按钮的文本为 "停车位" 加上编号
        parkingLocate[i - 1]->setText(s);
        // 调整按钮的大小
        parkingLocate[i - 1]->resize( parkingWid,parkingLong);
        // 设置按钮的位置
        if (i <= MAXSIZE / 2) {
            // 上排
            parkingLocate[i - 1]->setGeometry((i - 1) *1.5* parkingWid + 120, 10, parkingWid, parkingLong);
        } else {
            // 下排
            parkingLocate[i - 1]->setGeometry((i - 1 - MAXSIZE / 2) * 1.5*parkingWid + 120, 1.8*parkingLong, parkingWid, parkingLong);
        }
        // 设置按钮遮罩（使得只有定义的区域可点击）
        QRect rect(0, 0, parkingWid,  parkingLong);
        QRegion region(rect, QRegion::Rectangle);
        parkingLocate[i - 1]->setMask(region);
        // 连接按钮的点击信号到槽函数 onButtonClickedP
        connect(parkingLocate[i - 1], &QPushButton::clicked, this, &MainWindow::onButtonClickedP);
    }
    //便道按钮
    for(int i = 1; i <= MAXQUEUE; i++) {
        QString s = "候\n";
        s.append(QString::number(i));
        waitingLocate[i - 1] = new QPushButton(this);
        waitingLocate[i - 1]->setText(s);
        waitingLocate[i - 1]->resize(parkingWid, parkingLong);
        // 设置按钮的位置
        if (i == 1) {
            // 第一辆车的位置在停车库上下两排的中间
            waitingLocate[i - 1]->setGeometry(40, 0.9*parkingLong, parkingWid, parkingLong);
        } else {
            // 其他车的位置
            waitingLocate[i - 1]->setGeometry(40, 0.9*parkingLong + parkingLong * (i - 1), parkingWid, parkingLong);
        }
        // 设置按钮遮罩
        QRect rect(0, 0, parkingWid, parkingLong);
        QRegion region(rect, QRegion::Rectangle);
        waitingLocate[i - 1]->setMask(region);
        connect(waitingLocate[i - 1], &QPushButton::clicked, this, &MainWindow::onButtonClickedW);
    }

    //初始化汽车容器
    for(int i = 0; i < MAXSIZE; i++) {
        Car* newCar = new Car("", -1, QDateTime::currentDateTime());
        carArray.insert(carArray.begin() + i, *newCar);
    }
    // 初始化停车位数组，所有位置都设置为未占用（false）
    parkingSpaces.resize(MAXSIZE, false);
   /* carArray.resize(MAXSIZE);
    for(int i = 0; i < MAXSIZE; i++) {
        carArray[i] = Car("", -1, QDateTime::currentDateTime());
        parkingSpaces[i] = false;

    }*/


    // 设置布局
    ButtonWeight = new QWidget(this);
    ButtonWeight->setLayout(ButtonLayout);
    ButtonWeight->setGeometry(200, 10, 700, 50);

    // 创建水平布局管理器和承载 QWidget
    parkingLayout = new QHBoxLayout();
    parkingWidget = new QWidget(this);
    parkingWidget->setLayout(parkingLayout);
    parkingWidget->setGeometry(100, 100, 1000, 50);

    waitinglayout = new QVBoxLayout();
    waitingWidget = new QWidget(this);
    waitingWidget->setLayout(waitinglayout);
    waitingWidget->setGeometry(50, 100, 100, 500);

    //用于动画的图片
    rightLabel = new QLabel(this);
    upLable = new QLabel(this);
    downLable = new QLabel(this);

    //设置图片
    QPixmap newPixmap(":carRight.png");//向右
    QPixmap newPixmap2(":carUp.png");//向上
    QPixmap newPixmap3(":carDown.png");//向下
    newPixmap = newPixmap.scaled(parkingLong, parkingWid);
    newPixmap2 = newPixmap2.scaled(parkingWid, parkingLong);
    newPixmap3 = newPixmap3.scaled(parkingWid,parkingLong);

    upLable->setPixmap(newPixmap2);//向上
    rightLabel->setPixmap(newPixmap);//向右
    downLable->setPixmap(newPixmap3);//向下

    rightLabel->setScaledContents(true);
    upLable->setScaledContents(true);
    downLable->setScaledContents(true);

    rightLabel->setVisible(false);
    upLable->setVisible(false);
    downLable->setVisible(false);

    //图片集合
    spotImagesList = new QList<QLabel *>();
    waitImagesList = new QList<QLabel *>();
    waitImagesList_1 = new QList<QLabel *>();
    spotImagesList_1 = new QList<QLabel *>();

    //停车位的车辆图片
    for(int i = 0; i < MAXSIZE; i++) {
        previousImage = new QLabel(this);
        if (i < MAXSIZE / 2) {
            // First row
            previousImage->setPixmap(downLable->pixmap());
            previousImage->setGeometry(120+1.5*parkingWid * i, 10, parkingWid, parkingLong);
        } else {
            // Second row
            previousImage->setPixmap(upLable->pixmap());
            previousImage->setGeometry(120+1.5*parkingWid* (i - MAXSIZE / 2), 1.8*parkingLong, parkingWid, parkingLong);
        }
        previousImage->setVisible(false);
        spotImagesList->append(previousImage);
    }
    //便道的车辆图片
    for(int i = 0; i < MAXQUEUE; i++) {
        previousImage = new QLabel(this);
        previousImage->setPixmap(upLable->pixmap());

        if (i == 0) {
            // 第一辆车的位置在停车库上下两排的中间
            previousImage->setGeometry(40, 0.9*parkingLong, parkingWid, parkingLong);
        } else {
            // 其他车的位置
            previousImage->setGeometry(40, 0.9*parkingLong + parkingLong * i, parkingWid, parkingLong);
        }
        previousImage->setVisible(false);
        waitImagesList->append(previousImage);
    }

    //停车动画
    connect(ui->enterButton, &QPushButton::clicked, [this, newPixmap2, newPixmap3, T]() {
        //0为停车位进车
        if(start==0) {
            start == 0;
            //将动画的图片显示
            rightLabel->setVisible(true);
            upLable->setVisible(true);
            downLable->setVisible(true);
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
            animation0->setEndValue(QRect(40, 0.9*parkingLong, parkingWid, parkingLong));

            animation = new QPropertyAnimation(rightLabel, "geometry");
            animation->setDuration(T);
            animation->setStartValue(QRect(20, 1.18*parkingLong, parkingLong, parkingWid));
            animation->setEndValue(QRect(120-0.5*parkingWid + 1.5*parkingWid * (spot % (MAXSIZE / 2)), 1.18*parkingLong, parkingLong, parkingWid));

            //倒车入库
            int xCoordinate,yCoordinate;
            if (spot < MAXSIZE / 2) {
           // 第一排使用 downLable
                downLable->setPixmap(newPixmap3);
                xCoordinate=0.75*parkingLong;
            yCoordinate = 10;
             } else {
             // 第二排使用 upLable
             downLable->setPixmap(newPixmap2);
                 xCoordinate=1.25*parkingLong;
             yCoordinate = 1.8*parkingLong;
            }
            animation2 = new QPropertyAnimation(downLable, "geometry");
            animation2->setDuration(T);
            animation2->setStartValue(QRect(120 +  1.5*parkingWid * (spot % (MAXSIZE / 2)), xCoordinate, parkingWid, parkingLong));
            animation2->setEndValue(QRect(120 +  1.5*parkingWid * (spot % (MAXSIZE / 2)), yCoordinate, parkingWid, parkingLong));

            // 添加新的动画到动画组
            animationGroup.addAnimation(animation0);
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
        else if(start==2) {
            start == 2;
            rightLabel->setVisible(true);
            upLable->setVisible(true);
            downLable->setVisible(true);
            //设置位置
            rightLabel->setGeometry(-400, -100, parkingLong, parkingWid);
            upLable->setGeometry(-400, -100, parkingWid, parkingLong);
            downLable->setGeometry(-400, -100, parkingWid, parkingLong);

            // 停止并清空之前的动画组
            animationGroup.stop();
            animationGroup.clear();
            // 创建新的动画
            animation = new QPropertyAnimation(upLable, "geometry");
            animation->setDuration(T);
            animation->setStartValue(QRect(40, 1000, parkingWid, parkingLong));
            animation->setEndValue(QRect(40, 0.9*parkingLong+parkingLong * carQueuel.getsize(), parkingWid, parkingLong));
            // 添加新的动画到动画组
            animationGroup.addAnimation(animation);
            // 开始新的动画
            animationGroup.start();
        }
    });
     //离开动画
    connect(ui->leaveButton, &QPushButton::clicked, [this,T, newPixmap2, newPixmap3]() {
        // start = 1，只有停车位出车
        if (start == 1) {
            start == 1;
            rightLabel->setVisible(true);
            upLable->setVisible(true);
            downLable->setVisible(true);
            //设置位置
            rightLabel->setGeometry(-400, -100, parkingLong, parkingWid);
            upLable->setGeometry(-400, -100, parkingWid, parkingLong);
            downLable->setGeometry(-400, -100, parkingWid, parkingLong);
            // 更新停车场车辆状态
        for (int i = 0; i < MAXSIZE; ++i) {
            if (parkingSpaces[i]) {
                spotImagesList->at(i)->setVisible(true);
            }
        }

            spotImagesList->at(spot)->setVisible(false);
            spotImagesList_1->removeAt(spot);

            animationGroup.stop();
            animationGroup.clear();

            // 进入过道的动画
            animation0 = new QPropertyAnimation(downLable, "geometry");
            animation0->setDuration(T);
            int yCoordinate_1,yCoordinate;
            if (spot < MAXSIZE / 2) {
             // First row
                downLable->setPixmap(newPixmap3);
                yCoordinate = 10;
                yCoordinate_1=0.75*parkingLong;
                } else {
                // Second row
                    downLable->setPixmap(newPixmap2);
                  yCoordinate =  1.8*parkingLong;
                    yCoordinate_1=1.25*parkingLong;
                    }
            animation0->setStartValue(QRect(120 + 1.5*parkingWid * (spot % (MAXSIZE / 2)), yCoordinate, parkingWid, parkingLong));
            animation0->setEndValue(QRect(120 +1.5*parkingWid * (spot % (MAXSIZE / 2)), yCoordinate_1, parkingWid, parkingLong));


            // 由过道出车库的动画
           animation1 = new QPropertyAnimation(rightLabel, "geometry");
           animation1->setDuration(T);
           animation1->setStartValue(QRect(120 + 1.5*parkingWid * (spot % (MAXSIZE / 2)), 1.18*parkingLong, parkingLong, parkingWid));
            animation1->setEndValue(QRect(500, 1.18*parkingLong, parkingLong, parkingWid));

            //向下出车库的动画
            animation2 = new QPropertyAnimation(downLable, "geometry");
            animation2->setDuration(T);
            animation2->setStartValue(QRect(500+0.5*parkingLong, 1.18*parkingLong, parkingWid, parkingLong));
            animation2->setEndValue(QRect(500+0.5*parkingLong, 1000, parkingWid, parkingLong));

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
        //start = 3，便道有车时，停车位出车
        else if(start ==3) {
            start == 3;
            rightLabel->setVisible(true);
            upLable->setVisible(true);
            downLable->setVisible(true);
            rightLabel->setGeometry(-400, -100, parkingLong, parkingWid);
            upLable->setGeometry(-400, -100, parkingWid, parkingLong);
            downLable->setGeometry(-400, -100, parkingWid, parkingLong);

            // 更新停车场车辆状态
           for (int i = 0; i < MAXSIZE; ++i) {
               if (parkingSpaces[i]) {
                   spotImagesList->at(i)->setVisible(true);
               }
           }
            spotImagesList->at(spot)->setVisible(false);
            spotImagesList_1->removeAt(spot);

            animationGroup.stop();
            animationGroup.clear();

            // 停车场车辆出库，进入过道
            animation0 = new QPropertyAnimation(downLable, "geometry");
            animation0->setDuration(T);
            int yCoordinate_1,yCoordinate;
            if (spot < MAXSIZE / 2) {
             // First row
                downLable->setPixmap(newPixmap3);
                yCoordinate = 10;
                yCoordinate_1=0.75*parkingLong;
                } else {
                // Second row
                    downLable->setPixmap(newPixmap2);
                  yCoordinate =  1.8*parkingLong;
                    yCoordinate_1=1.25*parkingLong;
                    }
            animation0->setStartValue(QRect(120 + 1.5*parkingWid * (spot % (MAXSIZE / 2)), yCoordinate, parkingWid, parkingLong));
            animation0->setEndValue(QRect(120 +1.5*parkingWid * (spot % (MAXSIZE / 2)), yCoordinate_1, parkingWid, parkingLong));
            connect(animation0, &QPropertyAnimation::finished, [this]() {
                           downLable->hide();
                           rightLabel->show();
                           animation1->start();
                       });

            // 由过道出车库的动画
           animation1 = new QPropertyAnimation(rightLabel, "geometry");
           animation1->setDuration(T);
           animation1->setStartValue(QRect(120 + 1.5*parkingWid * (spot % (MAXSIZE / 2)), 1.18*parkingLong, parkingLong, parkingWid));
            animation1->setEndValue(QRect(500, 1.18*parkingLong, parkingLong, parkingWid));
            connect(animation1, &QPropertyAnimation::finished, [this]() {
     rightLabel->hide();
     downLable->show();
     animation2->start();
 });
            //向下出车库的动画
            animation2 = new QPropertyAnimation(downLable, "geometry");
            animation2->setDuration(T);
            animation2->setStartValue(QRect(500+0.5*parkingLong, 1.18*parkingLong, parkingWid, parkingLong));
            animation2->setEndValue(QRect(500+0.5*parkingLong, 1000, parkingWid, parkingLong));
            connect(animation2, &QPropertyAnimation::finished, [this, T]() {
                         downLable->hide();
                         rightLabel->show();
                waitImagesList->at(MAXQUEUE-lineNum-1)->setVisible(false);
                animation->start();

                     });
            //便道车辆入库
          animation = new QPropertyAnimation(rightLabel, "geometry");
          animation->setDuration(T);
          animation->setStartValue(QRect(35, 1.18*parkingLong, parkingLong, parkingWid));
          animation->setEndValue(QRect(120-0.5*parkingWid + 1.5*parkingWid * (spot % (MAXSIZE / 2)), 1.18*parkingLong, parkingLong, parkingWid));
          connect(animation, &QPropertyAnimation::finished, [this]() {
              rightLabel->hide();
              downLable->show();
              animation4->start();
          });
          //倒车入库
          int xCoordinate;
          if (spot < MAXSIZE / 2) {
         // 第一排使用 downLable
              xCoordinate=0.75*parkingLong;
          yCoordinate = 10;
           } else {
           // 第二排使用 upLable
           downLable->setPixmap(newPixmap2);
               xCoordinate=1.25*parkingLong;
           yCoordinate = 1.8*parkingLong;
          }
          animation4 = new QPropertyAnimation(downLable, "geometry");
          animation4->setDuration(T);
          animation4->setStartValue(QRect(120 +  1.5*parkingWid * (spot % (MAXSIZE / 2)), xCoordinate, parkingWid, parkingLong));
          animation4->setEndValue(QRect(120 +  1.5*parkingWid * (spot % (MAXSIZE / 2)), yCoordinate, parkingWid, parkingLong));
            connect(animation4, &QPropertyAnimation::finished, [this, T]() {
             downLable->hide();
             queueAdvanceGroup->stop();
             queueAdvanceGroup->clear();
             for (int i = MAXQUEUE - lineNum; i <MAXQUEUE; i++) {
                 auto advanceAnimation = new QPropertyAnimation(waitImagesList->at(i), "geometry");
                 advanceAnimation->setDuration(T);
                 advanceAnimation->setStartValue(waitImagesList->at(i)->geometry());
                advanceAnimation->setEndValue(waitImagesList->at(i-1)->geometry());
                queueAdvanceGroup->addAnimation(advanceAnimation);
             }
                connect(queueAdvanceGroup, &QSequentialAnimationGroup::finished, [this]() {
        for (int i = MAXQUEUE-lineNum; i <MAXQUEUE; i++) {
            waitImagesList->at(i)->setVisible(true);
        }
    });
             queueAdvanceGroup->start();
             parkingWidget->update();
             waitingWidget->update();
         });

            // Add the animations to the animation group
            animationGroup.addAnimation(animation0);
            animationGroup.addAnimation(animation1);
            animationGroup.addAnimation(animation2);
            animationGroup.addAnimation(animation);
            animationGroup.addAnimation(animation4);

      // Start the animation group
      animationGroup.start();

      // Update the layout
      parkingWidget->update();
            waitingWidget->update();
        }
    });
    // 连接动画完成信号到槽函数，更新车辆的图片
    connect(&animationGroup, &QSequentialAnimationGroup::finished, [this, newPixmap3]() {
        if(start == 0) {
            downLable->setPixmap(newPixmap3);
            previousImage = new QLabel(this);
   if (spot < MAXSIZE / 2) {
// First row
previousImage->setPixmap(downLable->pixmap());
} else {
// Second row
previousImage->setPixmap(upLable->pixmap());
}
   previousImage->setVisible(false);
   spotImagesList_1->append(previousImage);
   //显示停车位按钮上的图片
   spotImagesList->at(spot)->setVisible(true);

   // 更新布局
   parkingWidget->update();

        }
        else if (start == 1) {
            parkingSpaces[spot] = false;

      // Hide the animation labels
      rightLabel->setVisible(false);
      upLable->setVisible(false);
      downLable->setVisible(false);

      // Update the layout
      parkingWidget->update();
  }
        else if(start == 2) {
            previousImage = new QLabel(this);
previousImage->setPixmap(upLable->pixmap());
previousImage->setVisible(false);
waitImagesList_1->append(previousImage);
//显示便道按钮上的图片
waitImagesList->at(carQueuel.getsize() - 1)->setVisible(true);

// 更新布局
waitingWidget->update();
        }
        else if(start == 3) {
           // 更新布局
          parkingWidget->update();
          waitingWidget->update();
           previousImage = new QLabel(this);
           if (spot < MAXSIZE / 2) {
                // 第一排：设置图片朝下
                previousImage->setPixmap(downLable->pixmap());
            } else {
                // 第二排：设置图片朝上
                previousImage->setPixmap(upLable->pixmap());
            }
            previousImage->setVisible(false);


            spotImagesList_1->append(previousImage);
            spotImagesList->at(spot)->setVisible(true);
            parkingSpaces[spot] = true;


       }
        //隐藏动画的图片
        rightLabel->setVisible(false);
        upLable->setVisible(false);
        downLable->setVisible(false);
    });

    connect(ui->enterButton, SIGNAL(clicked()), this, SLOT(onNameButtonClicked()));
    connect(ui->leaveButton, SIGNAL(clicked()), this, SLOT(onLeaveButtonClicked()));
    connect(ui->queryButton_1, SIGNAL(clicked()), this, SLOT(onFindButtonClicked()));
    connect(ui->queryButton_2, SIGNAL(clicked()), this, SLOT(onShowButtonClicked()));
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
}

void MainWindow::onLeaveButtonClicked() {
    deleteCar();
}
//析构函数
MainWindow::~MainWindow() {
    delete ui;
}

//车辆进入停车位或便道
void MainWindow::createCar() {
    QString st = ui->lineEdit->text();
    if (st.isEmpty()) {
       return;
    }

    Car* newCar = new Car(st, -1, QDateTime::currentDateTime());
    ui->lineEdit->clear();

    // Check if the parking lot is full
    int occupiedSpots = std::count(parkingSpaces.begin(), parkingSpaces.end(), true);
    if (occupiedSpots >= MAXSIZE) {
        // Parking lot is full, check if the queue has space
        if (carQueuel.getsize() < MAXQUEUE) {
            lineNum++;
            carQueuel.insert(newCar);
            ui->listWidget->addItem("停车场已满," + st + "进入侯车位"+QString::number(lineNum)+"\n");
            ui->listWidget->scrollToBottom();
            start = 2;
            ui->enterButton->click();
        } else {
            ui->listWidget->addItem("停车场已满，便道已满，禁止进入\n");
            ui->listWidget->scrollToBottom();
        }
        return;
    }

    //随机分配一个车位号
    int randomSpot;
    do {
        randomSpot = QRandomGenerator::global()->bounded(MAXSIZE);
    } while (parkingSpaces[randomSpot]);

    newCar->spot = randomSpot+1;
    spot = randomSpot;
    carArray[randomSpot] = *newCar;
    parkingSpaces[randomSpot] = true;
    ui->listWidget->addItem(carArray[randomSpot].license + "进入停车位"+QString::number(carArray[randomSpot].spot)+"\n");
    ui->listWidget->scrollToBottom();
    start = 0; // Parking lot entry animation
    ui->enterButton->click();
}
//便道车辆入库
void MainWindow::addCar(int vacatedSpot) {
    if (!carQueuel.isEmpty()) {
        // 使用腾出的停车位
        int availableSpot = vacatedSpot;

        // 将车从队列移动到停车场
        Car* c = carQueuel.get();
        spot = availableSpot;
        c->spot = availableSpot;
        c->entreTime = QDateTime::currentDateTime();
        carArray[availableSpot-1] = *c;
        ui->listWidget->addItem(c->license + "进入停车位" + QString::number(c->spot )+"\n");
        ui->listWidget->scrollToBottom();
        parkingSpaces[availableSpot-1] = true;
        start = 3;
        ui->leaveButton->click();
        ui->enterButton->click();
        lineNum--;
        waitImagesList_1->pop_front();

    }
}
//车辆离开停车位
void MainWindow::deleteCar() {
   /* QString licensePlate = ui->lineEdit->text();
    bool isNumber;
    int spotIndex = licensePlate.toInt(&isNumber);
    spotIndex--;
    ui->lineEdit->clear();
    */
    int spotIndex = ui->lineEdit->text().toInt() ;
    ui->lineEdit->clear();
        if (spotIndex >0 && spotIndex <= MAXSIZE && parkingSpaces[spotIndex-1]) {

                spot = spotIndex-1;
                parkingSpaces[spot] = false;
                QDateTime currentTime = QDateTime::currentDateTime();
                QDateTime entryTime = carArray[spot].entreTime;
                int secondsParked = entryTime.secsTo(currentTime);
                double cost = secondsParked * 0.01; // 一秒钟一分钱的费率

                ui->listWidget_3->addItem("第" + QString::number(spotIndex) + "辆车    " + "车牌号:" + carArray[spot].license + "    离开"
                  "\n停：" + QString::number(secondsParked) + "秒,缴费：" + QString::number(cost) + "元\n");
                ui->listWidget_3->scrollToBottom();
                carArray[spot].license = "";
                carArray[spot].spot = -1;

                // 检查队列是否为空
                if(carQueuel.isEmpty()) {
                    start = 1; // 只有停车位腾出
                    ui->leaveButton->click();
                } else {
                    start = 3; // 停车位腾出，队列中的车进入
                    addCar(spotIndex); // 传递腾出的停车位索引
                }
            } else {
                ui->listWidget->addItem("此处无车");
                ui->listWidget->scrollToBottom();
            }


}
void MainWindow::showCar() {
    int num = 0;
    carQueuel.show();
    ui->listWidget->addItem("停车场:\n");
     ui->listWidget->scrollToBottom();
    for (int i = 0; i < MAXSIZE; i++) {
        if (parkingSpaces[i]) {
            num++;
            ui->listWidget->addItem("第" + QString::number(i + 1) + "个停车位    " + "车牌号:" + carArray[i].license + "     入库时间:"  + carArray[i].entreTime.toString("yyyy-MM-dd HH:mm:ss") + "\n");
         ui->listWidget->scrollToBottom();
        }
    }
    if(num == 0) {
        ui->listWidget->addItem("无车\n");
     ui->listWidget->scrollToBottom();
    }

    ui->listWidget->addItem("便道上:\n");
     ui->listWidget->scrollToBottom();
    Car* tem = carQueuel.getHead();
    int i = 1;
    if(tem != NULL) {
        while (tem != NULL) {
            if (!tem->license.isEmpty() && tem->entreTime.isValid()) {
                ui->listWidget->addItem("第" + QString::number(i) + "辆等候车辆    " + "车牌号:" + tem->license  + "\n");
                ui->listWidget->scrollToBottom();
                i++;
            }
            tem = tem->next;
        }
    } else {
        ui->listWidget->addItem("无车\n");
        ui->listWidget->scrollToBottom();
    }

    ui->listWidget->addItem("停车场内有:" + QString::number(num)+ "辆车");
    ui->listWidget->addItem("剩余位置:" + QString::number(MAXSIZE - num)+ "个");
    ui->listWidget->scrollToBottom();
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
        ui->listWidget->scrollToBottom();
        } else {
            ui->listWidget->addItem("此处无车\n");
            ui->listWidget->scrollToBottom();
        }
    } else {
        // Query by license plate number
        for (int i = 0; i < MAXSIZE; i++) {
            if (parkingSpaces[i] && s == carArray[i].license) {
                ok = true;
                ui->listWidget->addItem("第" + QString::number(i + 1) + "个停车位    " + "车牌号:" + carArray[i].license + "\n入库时间:"  + carArray[i].entreTime.toString("yyyy-MM-dd HH:mm:ss") + "\n");
                ui->listWidget->scrollToBottom();
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
                    ui->listWidget->scrollToBottom();
                    break;
                }
                tem = tem->next;
                i++;
            }
        }
    }

    if (!ok) {
        ui->listWidget->addItem("无效的车牌号或车位号\n");
        ui->listWidget->scrollToBottom();
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
        // Extract the number from the button text
        QString numberString = buttonText.mid(1);
        bool ok;
        int number = numberString.toInt(&ok);

        if (ok && number > 0 && number <= MAXSIZE && parkingSpaces[number - 1]) {
            // Get the car information from the parking space
            int index = number - 1;
            Car car = carArray[index];
            QDateTime entryTime = car.entreTime;
            QString license = car.license;

            // Display the car information
            ui->listWidget->addItem("第" + QString::number(number) + "个停车位    " + "车牌号:" + license +
                                    "\n入库时间：" + entryTime.toString("yyyy-MM-dd HH:mm:ss") + "\n");
        ui->listWidget->scrollToBottom();
        } else {
            ui->listWidget->addItem("此处无车\n");
            ui->listWidget->scrollToBottom();
        }
    }
}
void MainWindow::onButtonClickedW() {
    QPushButton *clickedButton = qobject_cast<QPushButton*>(sender());
    if (clickedButton) {
        ui->listWidget->addItem("便道车辆信息:\n");
        ui->listWidget->scrollToBottom();
        Car* tem = carQueuel.getHead();
        int i = 1;

        while (tem != nullptr) {
            ui->listWidget->addItem("便道第" + QString::number(i) + "辆等候车辆    " + "车牌号:" + tem->license + "\n");
            ui->listWidget->scrollToBottom();
            tem = tem->next;
            i++;
        }

        if (i == 1) {
            ui->listWidget->addItem("便道上无车\n");
            ui->listWidget->scrollToBottom();
        }
  }
}/*
void MainWindow::onButtonClickedP() {
    QPushButton *clickedButton = qobject_cast<QPushButton*>(sender());
    if (clickedButton) {
        QString buttonText = clickedButton->text();
        // Extract the number from the button text
        QString numberString = buttonText.mid(1);
        bool ok;
        int number = numberString.toInt(&ok);

        if (ok && number > 0 && number <= MAXSIZE && parkingSpaces[number - 1]) {
            // Remove the car from the parking space
            int index = number - 1;
            parkingSpaces[index] = false;
            QDateTime currentTime = QDateTime::currentDateTime();
            QDateTime entryTime = carArray[index].entreTime;
            int secondsParked = entryTime.secsTo(currentTime);
            double cost = secondsParked * 0.01; // Fee rate: 0.01 per second

            ui->listWidget->addItem("第" + QString::number(number) + "个停车位    " + "车牌号:" + carArray[index].license + "    离开"
              "\n停：" + QString::number(secondsParked) + "秒,缴费：" + QString::number(cost) + "元\n");

            // Clear the car data
            carArray[index].license = "";
            carArray[index].spot = -1;

            if(carQueuel.isEmpty()) {
                start = 1; // 只有停车位腾出
                ui->leaveButton->click();
            } else {
                start = 3; // 停车位腾出，队列中的车进入
                addCar(index+1); // 传递腾出的停车位索引
            }

            // Trigger the leave animation
            spot = index;
            ui->leaveButton->click();
        } else {
            ui->listWidget->addItem("此处无车\n");
        }
    }
}//点击停车场停车位按钮出库*/
