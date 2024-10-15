#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtCore>


//0为停车位进车，2为队列进车，1为只有停车位出车，3为停车位出车，队列进车
static int start = 0;
//变化的停车位
static int position = 0;
//变化的队列位置
static int lineNum = 0;
//static int ip = 1;

MainWindow::MainWindow(int MAXSIZE1, int Maxqueue1, QWidget *parent) : QMainWindow(parent)  ,
     ui(new Ui::MainWindow)
{

    ui->setupUi(this);

    this->MAXSIZE = MAXSIZE1;
    this->Maxqueue = Maxqueue1;
    setWindowTitle("停车场管理系统");

    //按钮的数组
    //车库
    QPushButton* locate[MAXSIZE];
    //便道
    QPushButton* locate2[Maxqueue];
    //动画使用的数组及其长宽高,停车场总长度为LONG,最大停车数MAX


    //定义每一个停车位的长度
    parkingLong = LONG / MAXSIZE;
    parkingWid = 30 * parkingLong / 54;


    // 初始化停车位数组，所有位置都设置为未占用（false）
    parkingSpaces.resize(MAXSIZE, false);


// 创建按钮布局
    ButtonLayout = new QHBoxLayout();


    // 初始化停车位按钮
    for(int i = 1; i <= MAXSIZE; i++) {
        QString s = "停车位";
        s.append(QString::number(i));
        locate[i - 1] = new QPushButton(this);
        locate[i - 1]->setText(s);
        locate[i - 1]->resize(parkingLong, parkingWid);
        locate[i - 1]->setGeometry((i - 1)*parkingLong + 200, 150, parkingLong, parkingWid);

        // 设置按钮遮罩
        QRect rect(0, 0,  parkingLong, parkingWid);
        QRegion region(rect, QRegion::Rectangle);
        locate[i - 1]->setMask(region);
        connect(locate[i - 1], &QPushButton::clicked, this, &MainWindow::onButtonClickedP);
    }
    for(int i = 1; i <= Maxqueue; i++) {
        QString s = "候车位";
        s.append(QString::number(i));
        locate2[i - 1] = new QPushButton(this);
        locate2[i - 1]->setText(s);
        locate2[i - 1]->resize(parkingWid, parkingLong);
        locate2[i - 1]->setGeometry(0, parkingLong * i, parkingWid, parkingLong);

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

    //隐藏起的按钮
    startButton = new QPushButton("Start Animation", this);
    startButton->setGeometry(-10, 10, 150, 30);
    finishButton = new QPushButton("Finish", this);
    finishButton->setGeometry(-200, 10, 150, 30);


    startButton->setVisible(false);
    finishButton->setVisible(false);

    //开始位置的小车图片
    tem = new QLabel(this);
    //用于动画的图片
    imageLabel = new QLabel(this);

    tem->setGeometry(10, 50, parkingLong, parkingWid);
    QPixmap newPixmap(":car1.png");
    QPixmap newPixmap2(":car2.png");
    QPixmap newPixmap3(":P.jpg");
    newPixmap = newPixmap.scaled(parkingLong, parkingWid);
    newPixmap2 = newPixmap2.scaled(parkingWid, parkingLong);
    temLable = new QLabel(this);
    temLable->setGeometry(-10, 250, parkingWid, parkingLong);
    imageLabel->setGeometry(-100, 50, parkingLong, parkingWid);

    temLable->setPixmap(newPixmap2);

    imageLabel->setPixmap(newPixmap);
    imageLabel->setScaledContents(true);
    temLable->setScaledContents(true);

    tem->setPixmap(newPixmap3);
    imageLabel->setVisible(false);
    temLable->setVisible(false);

    tem->move(100, 200);

    //图片集合
    previousImagesList = new QList<QLabel *>();
    previousImagesList2 = new QList<QLabel *>();
    previousImagesList4 = new QList<QLabel *>();
    previousImagesList3 = new QList<QLabel *>();
    for(int i = 0; i < MAXSIZE; i++) {
        previousImage = new QLabel(this);
        previousImage->setPixmap(imageLabel->pixmap());
        previousImage->setGeometry(200 + parkingLong * i, 150, parkingLong, parkingWid);
        previousImage->setVisible(false);

        previousImagesList->append(previousImage);
    }
    //隐藏的图片
    for(int i = 0; i < Maxqueue; i++) {
        previousImage = new QLabel(this);
        previousImage->setPixmap(temLable->pixmap());
        previousImage->setGeometry(0, parkingLong * (i + 1), parkingWid, parkingLong);
        previousImage->setVisible(false);
        previousImagesList2->append(previousImage);
    }

    //0为停车位进车，2为队列进车，1为只有停车位出车，3为停车位出车，队列进车

    connect(startButton, &QPushButton::clicked, [this]() {

        if(previousImagesList3->size() < MAXSIZE) {
            //将动画的图片显示
            imageLabel->setVisible(true);

            start = 0;
            // 停止并清空之前的动画组
            animationGroup.stop();
            animationGroup.clear();

            // 创建新的动画
            animation = new QPropertyAnimation(imageLabel, "geometry");
            animation->setDuration(1000);
            animation->setStartValue(QRect(-100, 50, parkingLong, parkingWid));
            animation2 = new QPropertyAnimation(imageLabel, "geometry");
            animation2->setDuration(300);
            animation->setEndValue(QRect(200 + parkingLong * position, 50, parkingLong, parkingWid));
            animation2->setStartValue(QRect(200 + parkingLong * position, 50, parkingLong, parkingWid));
            animation2->setEndValue(QRect(200 + parkingLong * position, 150, parkingLong, parkingWid));
            // 添加新的动画到动画组

            animationGroup.addAnimation(animation);
            animationGroup.addAnimation(animation2);

            // 开始新的动画
            animationGroup.start();

        }

        else if(previousImagesList4->size() <= Maxqueue) {
            temLable->setVisible(true);

            //临时停车道进车
            start = 2;
            // 停止并清空之前的动画组
            animationGroup.stop();
            animationGroup.clear();

            // 创建新的动画
            animation = new QPropertyAnimation(temLable, "geometry");
            animation->setDuration(1000);
            animation->setStartValue(QRect(0, 1000, parkingWid, parkingLong));
            animation->setEndValue(QRect(0, parkingLong * lineNum, parkingWid, parkingLong));

            // 添加新的动画到动画组
            animationGroup.addAnimation(animation);

            // 开始新的动画
            animationGroup.start();
        }
    });

    //当动画结束后将当前图片固定在结束位置
    //0为停车位进车，2为队列进车，1为只有停车位出车，3为停车位出车，队列进车
    connect(&animationGroup, &QSequentialAnimationGroup::finished, [this]() {
        //利用start实现只有当开始按钮被点击后才有图片添加
        if(start == 0) {
            previousImage = new QLabel(this);
            previousImage->setPixmap(imageLabel->pixmap());
            previousImage->setGeometry(200 + parkingLong * position, 150, parkingLong, parkingWid);
            previousImage->setVisible(false);
            previousImagesList3->append(previousImage);
            previousImagesList->at(position)->setVisible(true);

            // 更新布局
            parkingWidget->update();


        } else if(start == 2) {
            previousImage = new QLabel(this);
            previousImage->setPixmap(temLable->pixmap());
            previousImage->setGeometry(0, parkingLong * lineNum, parkingWid, parkingLong);
            previousImage->setVisible(false);
            previousImagesList4->append(previousImage);
            previousImagesList2->at(lineNum - 1)->setVisible(true);

            // 更新布局
            VparkingWidget->update();
        } else if(start == 3) {
            previousImage = new QLabel(this);
            previousImage->setPixmap(imageLabel->pixmap());
            previousImage->setGeometry(200 + parkingLong * position, 150, parkingLong, parkingWid);
            previousImage->setVisible(false);
            previousImagesList3->append(previousImage);
            previousImagesList->at(position)->setVisible(true);
            previousImagesList2->at(lineNum)->setVisible(false);
            // 更新布局
            parkingWidget->update();

        }



        imageLabel->setVisible(false);
        temLable->setVisible(false);

    });




    //离开动画
    connect(finishButton, &QPushButton::clicked, [this]() {


        //用于判断车辆的数目

        if ( !previousImagesList3->isEmpty() && previousImagesList4->isEmpty()) {
            imageLabel->setVisible(true);
            start = 1;
            previousImagesList->at(position)->setVisible(false);
            previousImagesList3->pop_back();

            // 获取并移除最后一个图片
            animationGroup.stop();
            animationGroup.clear();
            // 创建新的动画
            animation = new QPropertyAnimation(imageLabel, "geometry");
            animation->setDuration(1000);
            animation2 = new QPropertyAnimation(imageLabel, "geometry");
            animation2->setDuration(300);


            animation->setStartValue(QRect(200 + parkingLong * position, 50, parkingLong, parkingWid));
            animation2->setStartValue(QRect(200 + parkingLong * position, 150, parkingLong, parkingWid));
            animation2->setEndValue(QRect(200 + parkingLong * position, 50, parkingLong, parkingWid));

            animation->setEndValue(QRect(1400, 50, parkingLong, parkingWid));

            // 添加新的动画到动画组
            animationGroup.addAnimation(animation2);
            animationGroup.addAnimation(animation);

            // 开始新的动画
            animationGroup.start();
            // 删除图片


            parkingWidget->update();
        } else if( !previousImagesList3->isEmpty() && !previousImagesList4->isEmpty()) {
            imageLabel->setVisible(true);
            start = 3;
            previousImagesList->at(position)->setVisible(false);
            previousImagesList2->at(lineNum)->setVisible(false);
            previousImagesList3->pop_back();
            previousImagesList4->pop_back();
            // 获取并移除最后一个图片


            animationGroup.stop();
            animationGroup.clear();
            // 创建新的动画
            animation = new QPropertyAnimation(imageLabel, "geometry", this);
            animation->setDuration(1000);
            animation2 = new QPropertyAnimation(imageLabel, "geometry", this);
            animation2->setDuration(300);



            animation->setStartValue(QRect(200 + parkingLong * position, 50, parkingLong, parkingWid));

            animation2->setStartValue(QRect(200 + parkingLong * position, 150, parkingLong, parkingWid));

            animation2->setEndValue(QRect(200 + parkingLong * position, 50, parkingLong, parkingWid));

            animation->setEndValue(QRect(1400, 50, parkingLong, parkingWid));

            // 添加新的动画到动画组
            animationGroup.addAnimation(animation2);
            animationGroup.addAnimation(animation);



            // 创建新的动画
            animation = new QPropertyAnimation(imageLabel, "geometry");
            animation->setDuration(1000);

            animation2 = new QPropertyAnimation(imageLabel, "geometry");
            animation2->setDuration(300);
            \


            animation->setStartValue(QRect(-100, 50, parkingLong, parkingWid));
            animation->setEndValue(QRect(200 + parkingLong * position, 50, parkingLong, parkingWid));
            animation2->setStartValue(QRect(200 + parkingLong * position, 50, parkingLong, parkingWid));
            animation2->setEndValue(QRect(200 + parkingLong * position, 150, parkingLong, parkingWid));

            \
            // 添加新的动画到动画组
            animationGroup.addAnimation(animation);
            animationGroup.addAnimation(animation2);
            // 开始新的动画
            animationGroup.start();

            // 删除图片



            parkingWidget->update();
            VparkingWidget->update();

        }
    });
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(onNameButtonClicked()));
    connect(ui->pushButton_2, SIGNAL(clicked()), this, SLOT(onFindButtonClicked()));
    connect(ui->pushButton_8, SIGNAL(clicked()), this, SLOT(onShowButtonClicked()));
    connect(ui->pushButton_4, SIGNAL(clicked()), this, SLOT(onLeaveButtonClicked()));
}



// 在类的实现文件中实现槽函数
void MainWindow::onNameButtonClicked() {
    // 在这里编写与 "进入" 按钮相关的操作
    createCar();
}

void MainWindow::onFindButtonClicked() {
    // 在这里编写与 "查找" 按钮相关的操作
    findCar();
}

void MainWindow::onShowButtonClicked() {
    // 在这里编写与 "展示" 按钮相关的操作
    showCar();
    //ip++;
   // tem->move(100 * ip, 500);
}

void MainWindow::onLeaveButtonClicked() {
    // 在这里编写与 "离开" 按钮相关的操作
    deleteCar();
    addCar();
}

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
                ui->listWidget->addItem("停车场已满," + st + "进入等候队列\n");
                startButton->click();
            } else {
               ui->listWidget->addItem("停车场已满，队列已满，禁止进入\n");
            }

        } else {

            i = 0;
            // 将新的 Car 对象加入数组
            while(i < MAXSIZE) {
                if(parkingSpaces[i] == false) {
                    newCar->location = i;
                    position = i;

                    carArray[i].entreTime = newCar->entreTime;
                    carArray[i].location = newCar->location;
                    carArray[i].license = newCar->license;
                    parkingSpaces[i] = true;
                    ui->listWidget->addItem(carArray[i].license + "加入成功\n");
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

                position = i;
                c->location = i;
                c->entreTime = QDateTime::currentDateTime();
                carArray[i].entreTime = c->entreTime;
                carArray[i].location = c->location;
                carArray[i].license = c->license;
                parkingSpaces[i] = true;
                ui->listWidget->addItem(c->license + "加入成功\n");
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
            position = i;

            parkingSpaces[i] = false;
            QDateTime currentTime = QDateTime::currentDateTime();
            QDateTime entryTime = carArray[i].entreTime;
            int secondsParked = entryTime.secsTo(currentTime);
            int cost = secondsParked; // 一秒钟一块钱的费率

            ui->listWidget->addItem("第" + QString::number(i + 1) + "辆车    " + "车牌号:" + carArray[i].license + "    已出库，停车时长：" + QString::number(secondsParked) + "秒，费用：" + QString::number(cost) + "元\n");
            // 标记车辆为无效或已删除，而不是从 std::vector 中删除
            carArray[i].license = ""; // 或者其他方式标记为无效
            carArray[i].location = -1; // 或者其他方式标记为无效
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
    ui->listWidget->addItem("----------------------------------------------");
    ui->listWidget->addItem("停车位:\n");
    for (int i = 0; i < MAXSIZE; i++) {
        if (parkingSpaces[i]) {
            num++;
            ui->listWidget->addItem("第" + QString::number(i + 1) + "个停车位    " + "车牌号:" + carArray[i].license + "     入库时间:"  + carArray[i].entreTime.toString("yyyy-MM-dd HH:mm:ss") + "\n");
        }
    }
    if(num == 0) {
        ui->listWidget->addItem("无车\n");
    }

    ui->listWidget->addItem("等候位:\n");
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

    ui->listWidget->addItem("停车场内有:" + QString::number(num));
    ui->listWidget->addItem("剩余位置:" + QString::number(MAXSIZE - num));
    ui->listWidget->addItem("----------------------------------------------");
}
void MainWindow::findCar() {
    QString s = ui->lineEdit->text();
    ui->lineEdit->clear();
    // 用于判断转换是否成功
    bool ok = false;


    for (int i = 0; i < MAXSIZE; i++) {
        if (parkingSpaces[i]) {
            if(s == carArray[i].license) {
                ok = true;
                ui->listWidget->addItem("第" + QString::number(i + 1) + "个停车位    " + "车牌号:" + carArray[i].license + "     入库时间:"  + carArray[i].entreTime.toString("yyyy-MM-dd HH:mm:ss") + "\n");
            }
        }
    }
    if(!ok) {
        ui->listWidget->addItem("无效车牌号\n");
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
        QString numberString = buttonText.mid(3);
        bool ok;
        int number = numberString.toInt(&ok);

        if (ok && parkingSpaces[number - 1]) {
            ui->listWidget->addItem("----------------------------------------------------------\n");
            ui->listWidget->addItem("第" + QString::number(number) + "个停车位    " + "车牌号:" + carArray[number - 1].license + "     入库时间:"  + carArray[number - 1].entreTime.toString("yyyy-MM-dd HH:mm:ss") + "\n");
        } else {
            ui->listWidget->addItem("----------------------------------------------------------\n");
            ui->listWidget->addItem("此处无车\n");
        }
    }
}
void MainWindow::onButtonClickedW() {
    QPushButton *clickedButton = qobject_cast<QPushButton*>(sender());
    if (clickedButton) {
        QString buttonText = clickedButton->text();
        // 提取数字
        QString numberString = buttonText.mid(3);
        bool ok;
        int number = numberString.toInt(&ok);
        int temNumber = number;
        Car* tem = carQueuel.getHead();
        int i = 1;
        if(tem != NULL) {
            if (ok && number <= carQueuel.getsize()) {
                while(temNumber < 1) {
                    tem = tem->next;
                    temNumber--;
                }
                ui->listWidget->addItem("----------------------------------------------------------\n");
                ui->listWidget->addItem("第" + numberString + "辆等候车辆    " + "车牌号:" + tem->license  + "\n");

            } else {
                ui->listWidget->addItem("----------------------------------------------------------\n");
                ui->listWidget->addItem("此处无车\n");
            }
        } else {
            ui->listWidget->addItem("----------------------------------------------------------\n");
            ui->listWidget->addItem("此处无车\n");
        }
    }

}
