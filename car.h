#ifndef CAR_H
#define CAR_H

#include <QDateTime>
#include <QString>



class Car
{
public:
    Car();
    Car(QString license, int location, QDateTime entreTime)
    {  this->license=license;
        this->location=location;
        this->entreTime=entreTime;


    }
    QString license;
    int location;
    QDateTime  entreTime;
    Car* next=NULL;
};

#endif // CAR_H
