#ifndef EQUALIZERSGROUP_H
#define EQUALIZERSGROUP_H
#include <qlist.h>
#include <equalizer.h>


class EqualizersGroup: public QWidget
{
private:
    QList<Equalizer*> equalizersList;
public:
    EqualizersGroup(QObject *parent = nullptr);
    EqualizersGroup(QList<Equalizer*> equalizersList, QObject *parent = nullptr);

    void setEqualizersList(QList<Equalizer*> equalizersList);
    void setEqualizersSettings(QList<int> settings);
};

#endif // EQUALIZERSGROUP_H
