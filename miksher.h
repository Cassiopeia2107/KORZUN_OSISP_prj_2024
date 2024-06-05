#ifndef MIKSHER_H
#define MIKSHER_H

#include <QObject>

class Miksher : public QObject
{
    Q_OBJECT
private:
    bool isActive;
public:
    explicit Miksher(QObject *parent = nullptr);
    void on();
    void off();

    void setIsActive(bool isActive) { this->isActive = isActive; };
    bool getIsActive() { return this->isActive; };
signals:
};

#endif // MIKSHER_H
