#include "miksher.h"

Miksher::Miksher(QObject *parent)
    : QObject{parent}
{
    this->isActive = false;
}
