#ifndef EQUALIZER_H
#define EQUALIZER_H
#include <QLabel>
#include <QSlider>


class Equalizer: public QWidget
{
private:

    int frequensy;
    int value;
    QSlider* slider;
    QLabel* freqLabel;
    QLabel* valueLabel;

public:
    Equalizer(QObject *parent = nullptr);
    Equalizer(int frequensy, QObject *parent = nullptr);

    void setValue(int value);
    int getValue() { return this->value; };

    void setFrequensy(int frequensy);
    int getFrequensy() { return this->frequensy; };
};

#endif // EQUALIZER_H
