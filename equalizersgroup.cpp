#include "equalizersgroup.h"
#include <QHBoxLayout>

EqualizersGroup::EqualizersGroup(QObject *parent)
{
    this->setParent((QWidget*)parent);
}

EqualizersGroup::EqualizersGroup(QList<Equalizer*> equalizersList, QObject *parent): EqualizersGroup(parent) {
    setEqualizersList(equalizersList);
}

void EqualizersGroup::setEqualizersList(QList<Equalizer *> equalizersList){
    this->equalizersList = equalizersList;
    delete this->findChild<QHBoxLayout*>("layout");
    QHBoxLayout* layout = new QHBoxLayout(this);
    for(Equalizer* el: equalizersList){
        layout->addWidget(el);
    }
}

void EqualizersGroup::setEqualizersSettings(QList<int> settings){
    for(int i = 0; i < settings.length(); i++){
        equalizersList[i]->setValue(settings[i]);
    }
}
