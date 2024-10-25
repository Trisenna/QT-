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

    // 插入车辆到队列
    void insert(Car* c) {
        if (size == 0) {
            head = c;
            last = c;
            c->next = nullptr;
        } else {
            last->next = c;
            last = c;
            last->next = nullptr;
        }
        size++;
    }

    // 从队列中取出车辆
    Car* get() {
        // 检查队列是否为空
        if (isEmpty()) {
            qDebug() << "Queue is empty. No car to get.";
            return nullptr;
        }

        Car* tem = head;
        head = head->next;

        // 如果队列中只有一个元素，取出后需要将 last 置为空
        if (head == nullptr) {
            last = nullptr;
        }

        size--; // 减少队列的大小
        return tem;
    }

    // 显示队列中的车辆
    void show() {
        Car* tem = head;
        while (tem != nullptr) {
            qDebug() << tem->license + " " + QString::number(tem->spot) + " " + tem->entreTime.toString("yyyy-MM-dd HH:mm:ss") + "\n";
            tem = tem->next;
        }
    }

    Car* at(int index) {
        if (index < 0 || index >= size) {
            qDebug() << "Index out of bounds.";
            return nullptr; // 或者抛出异常
        }

        Car* tem = head;
        for (int i = 0; i < index; ++i) {
            tem = tem->next;
        }
        return tem;
    }
    // 获取队列的第一个车辆
    Car* getHead() {
        return head;
    }

    // 判断队列是否已满
    bool isFull() {
        return size >= Maxqueue;
    }

    // 判断队列是否为空
    bool isEmpty() {
        return size == 0;
    }

    // 获取队列中车辆数量
    int getsize() {
        return size;
    }

private:
    Car* head = nullptr;  // 队列头
    Car* last = nullptr;  // 队列尾
    int size = 0;         // 队列大小
};

#endif // CARQUEUE_H
