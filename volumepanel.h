#ifndef VOLUMEPANEL_H
#define VOLUMEPANEL_H
#include <QLabel>
#include "custombutton.h"
#include <QProgressBar>


class VolumePanel: public QWidget
{
public:
    QLabel* title;
    QProgressBar* bar;
    CustomButton* minusButton;
    CustomButton* plusButton;
    VolumePanel(QObject* parent = nullptr);
};

#endif // VOLUMEPANEL_H
