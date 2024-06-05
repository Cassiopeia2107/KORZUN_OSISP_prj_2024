#ifndef CUSTOMBUTTON_H
#define CUSTOMBUTTON_H
#include <qtimer.h>
#include <QPushButton>
#include <qprogressbar.h>


class CustomButton: public QPushButton
{
private:
    QProgressBar* progress_bar;
    QTimer *timer;
    int step;
public:
    CustomButton(QWidget *parent = nullptr);
    CustomButton(QString name, int step, QWidget *parent = nullptr);

    void setProgressBar(QProgressBar* progress_bar);
    void setStep(int step) { this->step = step; };
    int getStep() { return this->step; };
protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
};

#endif // CUSTOMBUTTON_H
