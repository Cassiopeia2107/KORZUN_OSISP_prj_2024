#ifndef AUDIOPLAYER_H
#define AUDIOPLAYER_H

#include <QWidget>
#include <mpg123.h>
#include <alsa/asoundlib.h>
#include <QTimer>
#include <QPushButton>
#include <QSlider>
#include <QLabel>
#include "volumepanel.h"

class AudioPlayer : public QWidget {
    Q_OBJECT

public:
    explicit AudioPlayer(QWidget *parent = nullptr);
    ~AudioPlayer();

    bool loadFile(const QString &filePath);
    void play();
    void pause();
    void stop();
    void setVolume(int volumeLeft, int volumeRight);
    void setEqualizer(int band, float gain);
    QString formatTime(int seconds);
    void onUpdateTimer();

signals:
    void updateProgress(int currentTime, int duration);

public slots:
    void onSoundChanged()
    {
        setVolume(volumePanel1->bar->value(), volumePanel2->bar->value());
    }

private:
    void initializeAudio();
    void cleanupAudio();

    VolumePanel* volumePanel1;
    VolumePanel* volumePanel2;

    mpg123_handle *mh;
    snd_pcm_t *pcm_handle;
    QPushButton *loadButton;
    QPushButton *playButton;
    QPushButton *pauseButton;
    QPushButton *stopButton;
    QSlider *volumeSlider;
    QSlider *eqSlider;
    QSlider *progressSlider;
    QLabel *timeLabel;

    int volume;
    int currentChannel;
};

#endif // AUDIOPLAYER_H
