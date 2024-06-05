#ifndef WORKER_H
#define WORKER_H
#include <QDebug>
#include <alsa/asoundlib.h>
#include <mpg123.h>

class Worker : public QObject {
    Q_OBJECT

public:
    void playSong(mpg123_handle* mh, unsigned char* buffer, size_t buffer_size, size_t done, snd_pcm_t *pcm_handle, snd_pcm_uframes_t frames);
    void updateTime();
};

#endif // WORKER_H
