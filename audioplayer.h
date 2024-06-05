#ifndef AUDIOPLAYER_H
#define AUDIOPLAYER_H

#include <QWidget>
#include <mpg123.h>
#include <alsa/asoundlib.h>
#include <QTimer>
#include <QPushButton>
#include <QSlider>
#include <QLabel>

class AudioPlayer : public QWidget {
    Q_OBJECT

public:
    explicit AudioPlayer(QWidget *parent = nullptr);
    ~AudioPlayer();

    bool loadFile(const QString &filePath);
    void play();
    void pause();
    void stop();
    void setVolume(int volume);
    void setEqualizer(int band, float gain);
    QString formatTime(int seconds);

signals:
    void updateProgress(int currentTime, int duration);

private slots:
    void onUpdateTimer();

private:
    void initializeAudio();
    void cleanupAudio();

    mpg123_handle *mh;
    snd_pcm_t *pcm_handle;
    QTimer *timer;
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
