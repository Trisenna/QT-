#ifndef CARQUEUE_H
#define CARQUEUE_H

#include "car.h"
#include <QDebug>

#include <QTextBrowser>
#include <iostream>
#include <qdebug.h>



class CarQueue {
  public:

    int Max;
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
        size--;
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
        if(size < Max) {
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

    void remove(Car* car) {
        if (head == nullptr) return; // Queue is empty

        if (head == car) {
            head = head->next;
            size--;
            return;
        }

        Car* prev = head;
        Car* curr = head->next;

        while (curr != nullptr) {
            if (curr == car) {
                prev->next = curr->next;
                if (curr == last) {
                    last = prev;
                }
                size--;
                return;
            }
            prev = curr;
            curr = curr->next;
        }
    }

  private:
    Car* head = NULL;
    Car* last = NULL;
    int size = 0;

};

#endif // CARQUEUE_H
