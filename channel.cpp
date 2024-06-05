#include "channel.h"
#include <QPushButton>
#include <QSlider>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QThread>
#include <QDebug>

Channel::Channel(QWidget* parent)
{
    QPushButton* load = new QPushButton("Канал", this);
    QPushButton* play = new QPushButton("Play", this);

    QWidget* buttonsLayoutWidget = new QWidget(this);
    QVBoxLayout* buttonsLayout = new QVBoxLayout(buttonsLayoutWidget);
    buttonsLayout->addWidget(load);
    buttonsLayout->addWidget(play);

    QSlider* slider = new QSlider(this);
    QLabel* time_label = new QLabel("00:00/00:00");

    QWidget* scrollLayoutWidget = new QWidget(this);
    QVBoxLayout* scrollLayout = new QVBoxLayout(scrollLayoutWidget);
    slider->setOrientation(Qt::Horizontal);
    scrollLayout->addWidget(slider);
    scrollLayout->addWidget(time_label);

    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->addWidget(buttonsLayoutWidget);
    layout->addWidget(scrollLayoutWidget);

    this->setParent((QWidget*)parent);

    connect(load, &QPushButton::pressed, this, &Channel::loadMp3);

    mpg123_init();
    mh = mpg123_new(NULL, &err);

    snd_pcm_open(&pcm_handle, PCM_DEVICE, SND_PCM_STREAM_PLAYBACK, 0);
    snd_pcm_hw_params_malloc(&params);
    snd_pcm_hw_params_any(pcm_handle, params);
    snd_pcm_hw_params_set_access(pcm_handle, params, SND_PCM_ACCESS_RW_INTERLEAVED);
    snd_pcm_hw_params_set_format(pcm_handle, params, SND_PCM_FORMAT_S16_LE);

    setVolume(50);
}

Channel::~Channel(){
    free(buffer);
    snd_pcm_drain(pcm_handle);
    snd_pcm_close(pcm_handle);
    mpg123_close(mh);
    mpg123_delete(mh);
    mpg123_exit();
}

void Channel::loadMp3(){
    audio_path = QFileDialog::getOpenFileName(this, tr("Open MP3 File"), "/home", tr("MP3 Files (*.mp3)"));
    if (audio_path.isEmpty()) return;

    mpg123_open(mh, audio_path.toUtf8().data());
    mpg123_getformat(mh, &rate, &channels, &encoding);

    snd_pcm_hw_params_set_channels(pcm_handle, params, channels);
    snd_pcm_hw_params_set_rate_near(pcm_handle, params, (unsigned int*)&rate, NULL);
    snd_pcm_hw_params(pcm_handle, params);
    snd_pcm_hw_params_get_period_size(params, &frames, 0);
    snd_pcm_prepare(pcm_handle);

    buffer_size = frames * channels * 2; // 2 bytes per sample
    buffer = (unsigned char *) malloc(buffer_size);

    audioThread = new QThread;
    connect(audioThread, &QThread::started, this, &Channel::play);
    audioThread->start();
}

void Channel::play(){
    while (mpg123_read(mh, buffer, buffer_size, &done) == MPG123_OK) {
         snd_pcm_writei(pcm_handle, buffer, frames);
    }
}

void Channel::updateTime(){
    int song_duration = (int) (mpg123_length(mh) * mpg123_tpf(mh) / 1000);
    qDebug() << "Song duration: " << song_duration;

    while (true) {
        off_t position = mpg123_tell(mh);
        qDebug() << "Current position in samples: " << position * mpg123_tpf(mh) / 1000;
        usleep(100000);
    }
}

void Channel::setVolume(int volume) {
    long min, max;
    snd_mixer_t *handle;
    snd_mixer_selem_id_t *sid;
    static const char *mix_name = "Master";
    static const char *card = "default";
    static int mix_index = 0;

    snd_mixer_open(&handle, 0);
    snd_mixer_attach(handle, card);
    snd_mixer_selem_register(handle, NULL, NULL);
    snd_mixer_load(handle);

    snd_mixer_selem_id_alloca(&sid);
    snd_mixer_selem_id_set_index(sid, mix_index);
    snd_mixer_selem_id_set_name(sid, mix_name);
    snd_mixer_elem_t* elem = snd_mixer_find_selem(handle, sid);

    if (!elem) {
        printf("Unable to find simple control.\n");
        return;
    }

    snd_mixer_selem_get_playback_volume_range(elem, &min, &max);
    qDebug() << min << max;
    snd_mixer_selem_set_playback_volume_all(elem, volume * max / 100);

    snd_mixer_close(handle);
}
