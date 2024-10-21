#ifndef CARQUEUE_H
#define CARQUEUE_H

#include "car.h"
#include <QDebug>

#include <QTextBrowser>
#include <iostream>
#include <qdebug.h>



class CarQueue {
  public:

    int Maxqueue;
    CarQueue();

    void insert(Car* c) {
        if(size == 0) {
            head = c;
            last = c;
            c->next = NULL;

        } else {
            last->next = c;
            last = c;
            last->next = NULL;
        }
        size++;
    }
    Car* get() {
        Car* tem = head;
        head = head->next;
        return tem;
    }
    void show() {
        Car* tem = head; // Assuming 'head' is the pointer to the first node of the linked list

        while (tem != nullptr) {
            qDebug() << tem->license + " " + QString::number(tem->spot) + " " + tem->entreTime.toString("yyyy-MM-dd HH:mm:ss") + "\n";
            tem = tem->next;
        }
    }
    Car* getHead() {
        return head;
    }

    bool isFull() {

        if(size < Maxqueue) {
            return false;
        } else return true;
    }
    bool isEmpty() {
        if(size == 0) return true;
        else return false;
    }
    int getsize() {
        return size;
    }

  private:
    Car* head = NULL;
    Car* last = NULL;
    int size = 0;

};

#endif // CARQUEUE_H
