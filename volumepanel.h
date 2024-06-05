#ifndef VOLUMEPANEL_H
#define VOLUMEPANEL_H
#include <QLabel>
#include "custombutton.h"
#include <QProgressBar>


class VolumePanel: public QWidget
{
private:
    QLabel* title;
    QProgressBar* bar;
    CustomButton* minusButton;
    CustomButton* plusButton;
public:
    VolumePanel(QObject* parent = nullptr);
};

#endif // VOLUMEPANEL_H
