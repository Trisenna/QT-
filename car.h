#ifndef CAR_H
#define CAR_H

#include <QDateTime>
#include <QString>



class Car
{
public:
    Car();
    Car(QString license, int spot, QDateTime entreTime)
    {  this->license=license;
        this->spot=spot;
        this->entreTime=entreTime;


    }
    QString license;
    int spot;
    QDateTime  entreTime;
    Car* next=NULL;
};

#endif // CAR_H
