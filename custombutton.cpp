#include "custombutton.h"
#include <qdebug.h>
#include <QMouseEvent>

#include "custombutton.h"
#include <qdebug.h>
#include <QMouseEvent>

CustomButton::CustomButton(QWidget *parent): QPushButton(parent)
{
    timer = new QTimer(this);
    timer->setInterval(100);
}

CustomButton::CustomButton(QString name, int step, QWidget *parent): CustomButton(parent){
    this->setText(name);
    this->step = step;
}

void CustomButton::setProgressBar(QProgressBar* progress_bar)
{
    this->progress_bar = progress_bar;
    connect(timer, &QTimer::timeout, this, [=]() {
        this->progress_bar->setValue(this->progress_bar->value() + step);
    });
}

void CustomButton::mousePressEvent(QMouseEvent *event)
{
    timer->start();
}

void CustomButton::mouseReleaseEvent(QMouseEvent *event)
{
    timer->stop();
}
