#include "volumepanel.h"
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <qdebug.h>

VolumePanel::VolumePanel(QObject* parent)
{
    QVBoxLayout* layout = new QVBoxLayout(this);
    title = new QLabel("Громкость");
    title->setAlignment(Qt::AlignCenter);
    layout->addWidget(title);

    bar = new QProgressBar;
    layout->addWidget(bar);

    minusButton = new CustomButton("-", -1, this);
    minusButton->setMaximumHeight(32);
    minusButton->setMaximumWidth(32);
    minusButton->setProgressBar(bar);

    plusButton = new CustomButton("+", 1, this);
    plusButton->setMaximumHeight(32);
    plusButton->setMaximumWidth(32);
    plusButton->setProgressBar(bar);

    QWidget *buttonsWidget = new QWidget(this);
    QHBoxLayout* buttonsLayout = new QHBoxLayout(buttonsWidget);
    buttonsLayout->addWidget(minusButton);
    buttonsLayout->addWidget(plusButton);
    layout->addWidget(buttonsWidget);

    this->setParent((QWidget*)parent);
}
