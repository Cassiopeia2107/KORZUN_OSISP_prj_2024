#include "audioplayer.h"
#include <QDebug>
#include <QFileDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <qthread.h>

AudioPlayer::AudioPlayer(QWidget *parent) : QWidget(parent), mh(nullptr), pcm_handle(nullptr), volume(100), currentChannel(0) {
    initializeAudio();

    loadButton = new QPushButton("Load", this);
    pauseButton = new QPushButton("Pause", this);
    progressSlider = new QSlider(Qt::Horizontal, this);
    progressSlider->setEnabled(false);
    timeLabel = new QLabel("00:00 / 00:00", this);

    connect(loadButton, &QPushButton::clicked, [this]() {
        QString filePath = QFileDialog::getOpenFileName(this, "Open MP3 File", "", "MP3 Files (*.mp3)");
        if (!filePath.isEmpty()) {
            QThread* thread = new QThread;
            connect(thread, &QThread::started, [&, filePath](){
                loadFile(filePath);
            });
            thread->start();
        }
    });

    connect(pauseButton, &QPushButton::clicked, this, &AudioPlayer::pause);

    QWidget *layout1_widget = new QWidget(this);
    QVBoxLayout *layout1 = new QVBoxLayout(layout1_widget);

    layout1->addWidget(loadButton);
    layout1->addWidget(pauseButton);

    QWidget *layout2_widget = new QWidget(this);
    QVBoxLayout *layout2 = new QVBoxLayout(layout2_widget);

    layout2->addWidget(progressSlider);
    layout2->addWidget(timeLabel);


    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->addWidget(layout1_widget);
    layout->addWidget(layout2_widget);

    setLayout(layout);
}

AudioPlayer::~AudioPlayer() {
    cleanupAudio();
}

void AudioPlayer::initializeAudio() {
    mpg123_init();
    mh = mpg123_new(nullptr, nullptr);
    snd_pcm_open(&pcm_handle, "default", SND_PCM_STREAM_PLAYBACK, 0);
    snd_pcm_set_params(pcm_handle, SND_PCM_FORMAT_S16_LE, SND_PCM_ACCESS_RW_INTERLEAVED, 2, 44100, 1, 500000);
}

void AudioPlayer::cleanupAudio() {
    if (mh) {
        mpg123_close(mh);
        mpg123_delete(mh);
        mh = nullptr;
    }
    if (pcm_handle) {
        snd_pcm_close(pcm_handle);
        pcm_handle = nullptr;
    }
    mpg123_exit();
}

bool AudioPlayer::loadFile(const QString &filePath) {
    if (!mh || !pcm_handle) return false;
    int err = mpg123_open(mh, filePath.toUtf8().constData());
    off_t totalSamples = mpg123_length(mh);
    int duration = static_cast<int>(totalSamples / 44100);
    progressSlider->setMaximum(duration);
    timeLabel->setText(QString("%1 / %2").arg(formatTime(0)).arg(formatTime(duration)));

    unsigned char buffer[4096];
    size_t done;
    while (mpg123_read(mh, buffer, sizeof(buffer), &done) == MPG123_OK) {
        snd_pcm_writei(pcm_handle, buffer, done / 4);
        onUpdateTimer();
    }
    return err == MPG123_OK;
}


void AudioPlayer::pause() {
    if (!mh || !pcm_handle) return;
    if(snd_pcm_state(pcm_handle) == SND_PCM_STATE_RUNNING){
        snd_pcm_pause(pcm_handle, 1);
        pauseButton->setText("Continue");
    }
    else{
        snd_pcm_pause(pcm_handle, 0);
        pauseButton->setText("Pause");
    }
}

void AudioPlayer::setVolume(int volumeLeft, int volumeRight) {
    if (!mh || !pcm_handle) return;

    long min, max;
    snd_mixer_t *mixer;
    snd_mixer_elem_t *elem;
    const char *card = "default";
    const char *selem_name = "Master";

    // Открываем микшер
    snd_mixer_open(&mixer, 0);

    // Инициализируем микшер
    snd_mixer_attach(mixer, card);
    snd_mixer_selem_register(mixer, nullptr, nullptr);
    snd_mixer_load(mixer);

    // Находим элемент управления громкостью
    snd_mixer_selem_id_t *sid;
    snd_mixer_selem_id_alloca(&sid);
    snd_mixer_selem_id_set_index(sid, 0);
    snd_mixer_selem_id_set_name(sid, selem_name);
    elem = snd_mixer_find_selem(mixer, sid);

    // Получаем минимальное и максимальное значение громкости
    snd_mixer_selem_get_playback_volume_range(elem, &min, &max);

    // Устанавливаем громкость для левого и правого каналов
    long volumeLeftAlsa = (volumeLeft * (max - min)) / 100 + min;
    long volumeRightAlsa = (volumeRight * (max - min)) / 100 + min;
    snd_mixer_selem_set_playback_volume(elem, SND_MIXER_SCHN_FRONT_LEFT, volumeLeftAlsa);
    snd_mixer_selem_set_playback_volume(elem, SND_MIXER_SCHN_FRONT_RIGHT, volumeRightAlsa);

    // Закрываем микшер
    snd_mixer_close(mixer);
}


void AudioPlayer::setEqualizer(int band, float gain) {
    if (!mh || !pcm_handle) return;
    // Implement equalizer adjustment logic
}

void AudioPlayer::onUpdateTimer() {
    if (!mh || !pcm_handle) return;
    off_t currentSample = mpg123_tell(mh);
    off_t totalSamples = mpg123_length(mh);
    int currentTime = static_cast<int>(currentSample / 44100);
    int duration = static_cast<int>(totalSamples / 44100);
    emit updateProgress(currentTime, duration);
    // Update progressSlider and timeLabel here
    progressSlider->setMaximum(duration);
    progressSlider->setValue(currentTime);
    timeLabel->setText(QString("%1 / %2").arg(formatTime(currentTime)).arg(formatTime(duration)));
}

QString AudioPlayer::formatTime(int seconds) {
    int minutes = seconds / 60;
    seconds %= 60;
    return QString::asprintf("%02d:%02d", minutes, seconds);
}
