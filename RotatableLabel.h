// rotatablelabel.h
#ifndef ROTATABLELABEL_H
#define ROTATABLELABEL_H

#include <QLabel>
#include <QPainter>

class RotatableLabel : public QLabel {
    Q_OBJECT
    Q_PROPERTY(int rotation READ rotation WRITE setRotation)

public:
    RotatableLabel(QWidget *parent = nullptr) : QLabel(parent), m_rotation(0) {}

    int rotation() const { return m_rotation; }
    void setRotation(int angle) {
        m_rotation = angle;
        update();
    }

protected:
    void paintEvent(QPaintEvent *event) override {
        QPainter painter(this);
        painter.translate(width() / 2, height() / 2);
        painter.rotate(m_rotation);
        painter.translate(-width() / 2, -height() / 2);
        QLabel::paintEvent(event);
    }

private:
    int m_rotation;
};

#endif // ROTATABLELABEL_H