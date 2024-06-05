#ifndef CUSTOMBUTTON_H
#define CUSTOMBUTTON_H
#include <qtimer.h>
#include <QPushButton>
#include <qprogressbar.h>


class CustomButton : public QPushButton
{
    Q_OBJECT
private:
    QProgressBar* progress_bar;
    QTimer *timer;
    int step;
public:
    CustomButton(QWidget *parent = nullptr);
    CustomButton(QString name, int step, QWidget *parent);
    void setProgressBar(QProgressBar* progress_bar);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
signals:
    void soundChanged();
};

#endif // CUSTOMBUTTON_H
