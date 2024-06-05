#include "equalizer.h"
#include <QString>
#include <QVBoxLayout>

Equalizer::Equalizer(QObject *parent)
{
    QVBoxLayout* layout = new QVBoxLayout(this);
    slider = new QSlider();
    slider->setMinimum(-10);
    slider->setMaximum(10);
    connect(slider, &QSlider::sliderMoved, this, [=](int position)
        {setValue(position);}
    );

    valueLabel = new QLabel();
    freqLabel = new QLabel();
    layout->addWidget(slider);
    layout->addWidget(valueLabel);
    layout->addWidget(freqLabel);

    this->setValue(0);
    this->setFrequensy(0);
    this->setParent((QWidget*)parent);
}

Equalizer::Equalizer(int frequensy, QObject *parent): Equalizer(parent){
    this->setFrequensy(frequensy);
}

void Equalizer::setValue(int value)
{
    this->value = value;
    QString res = "";
    if(value > 0)
        res += "+";
    else if(value < 0)
        res += "-";
    res += QString::number(abs(value));
    slider->setValue(value);
    this->valueLabel->setText(res);
}

void Equalizer::setFrequensy(int frequensy)
{
    this->frequensy = frequensy;
    QString res = QString::number(this->frequensy) + "Hz";
    this->freqLabel->setText(res);
}
