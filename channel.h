#ifndef CHANNEL_H
#define CHANNEL_H

#include <QObject>
#include <mpg123.h>
#include <alsa/asoundlib.h>

class AudioPlayer : public QObject {
    Q_OBJECT

public:
    explicit AudioPlayer(QObject *parent = nullptr);
    ~AudioPlayer();

    bool loadFile(const QString &filePath, int channel);
    void play(int channel);
    void stop(int channel);
    void setVolume(int channel, int volume);
    void setEqualizer(int channel, int band, float gain);

private:
    struct Channel {
        mpg123_handle *mh;
        snd_pcm_t *pcm_handle;
        int volume;
    };

    Channel channels[2];
    void initializeChannel(Channel &channel);
    void cleanupChannel(Channel &channel);
};

#endif // CHANNEL_H
