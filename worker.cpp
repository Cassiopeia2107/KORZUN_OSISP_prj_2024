#include "worker.h"

void Worker::playSong(mpg123_handle* mh, unsigned char* buffer, size_t buffer_size, size_t done, snd_pcm_t *pcm_handle, snd_pcm_uframes_t frames) {
    qDebug() << "kek";
    while (mpg123_read(mh, buffer, buffer_size, &done) == MPG123_OK) {
         snd_pcm_writei(pcm_handle, buffer, frames);
    }
}

void Worker::updateTime() {
    qDebug() << "Doing 2";
}
