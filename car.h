#ifndef CAR_H
#define CAR_H

#include <QDateTime>
#include <QString>

class Car {
public:
    // 默认构造函数
    Car() : license(""), spot(-1), entreTime(QDateTime::currentDateTime()), next(nullptr) {}

    // 参数化构造函数，使用初始化列表
    Car(QString license, int spot, QDateTime entreTime)
        : license(license), spot(spot), entreTime(entreTime), next(nullptr) {}

    // 成员变量
    QString license;         // 车牌号
    int spot;                // 停车位
    QDateTime entreTime;      // 入库时间
    Car* next = nullptr;      // 指向下一个车的指针
};

#endif // CAR_H
