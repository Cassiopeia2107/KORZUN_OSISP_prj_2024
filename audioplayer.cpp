#include "audioplayer.h"
#include <QDebug>
#include <QFileDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>

AudioPlayer::AudioPlayer(QWidget *parent) : QWidget(parent), mh(nullptr), pcm_handle(nullptr), timer(nullptr), volume(100), currentChannel(0) {
    initializeAudio();

    loadButton = new QPushButton("Load", this);
    pauseButton = new QPushButton("Pause", this);
    progressSlider = new QSlider(Qt::Horizontal, this);
    timeLabel = new QLabel("00:00 / 00:00", this);

    connect(loadButton, &QPushButton::clicked, [this]() {
        QString filePath = QFileDialog::getOpenFileName(this, "Open MP3 File", "", "MP3 Files (*.mp3)");
        qDebug() << filePath;
        if (!filePath.isEmpty()) {
            qDebug() << "da";
            loadFile(filePath);
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
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &AudioPlayer::onUpdateTimer);
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
    delete timer;
    timer = nullptr;
    mpg123_exit();
}

bool AudioPlayer::loadFile(const QString &filePath) {
    if (!mh || !pcm_handle) return false;
    int err = mpg123_open(mh, filePath.toUtf8().constData());
    off_t totalSamples = mpg123_length(mh);
    int duration = static_cast<int>(totalSamples / 44100);
    progressSlider->setMaximum(duration);
    timeLabel->setText(QString("%1 / %2").arg(formatTime(0)).arg(formatTime(duration)));

    timer->setInterval(1000);
    unsigned char buffer[4096];
    size_t done;
    while (mpg123_read(mh, buffer, sizeof(buffer), &done) == MPG123_OK) {
        snd_pcm_writei(pcm_handle, buffer, done / 4);
    }
    return err == MPG123_OK;
}


void AudioPlayer::pause() {
    if (!mh || !pcm_handle) return;
    if(timer->isActive())
        timer->setInterval(0);
    else
        timer->setInterval(1000);
    // implement pause logic
}

void AudioPlayer::stop() {
    if (!mh || !pcm_handle) return;
    timer->stop();
    snd_pcm_drop(pcm_handle);
}

void AudioPlayer::setVolume(int volume) {
    if (!mh || !pcm_handle) return;
    this->volume = volume;
    // Implement volume adjustment logic using ALSA's mixer API
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
