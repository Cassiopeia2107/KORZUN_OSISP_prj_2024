#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <qdebug.h>
#include <QFileDialog>
#include "worker.h"

#include <alsa/asoundlib.h>
#include <mpg123.h>
#include <qthread.h>
#include <QList>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    miksher = new Miksher();
    ui->pushButton_3->setEnabled(true);

    QList<Equalizer*> equalizersList = QList<Equalizer*>() << new Equalizer(60, this) << new Equalizer(230, this) << new Equalizer(910, this) << new Equalizer(3600, this) << new Equalizer(14400, this);
    ui->widget->setEqualizersList(equalizersList);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_3_clicked()
{
    bool currState = miksher->getIsActive();
    if(currState) {
        ui->pushButton_3->setText("Off");
        ui->miksherWidget->setEnabled(false);
        ui->pushButton_3->setStyleSheet(R"(
            QPushButton {
                background-color: #FF6B6B;
                color: white;
                border-radius: 5px;
                padding: 5px 10px;
            }
        )");
    }
    else {
        ui->pushButton_3->setText("On");
        ui->miksherWidget->setEnabled(true);
        ui->pushButton_3->setStyleSheet(R"(
            QPushButton {
                background-color: #4caf50;
                color: white;
                border-radius: 5px;
                padding: 5px 10px;
            }
        )");
    }
    miksher->setIsActive(!currState);
}

// load mp3
void MainWindow::on_pushButton_clicked()
{

//               off_t length = mpg123_length(mh);
//               int song_duration = (int) (length * mpg123_tpf(mh) / 1000);
//               qDebug() << "Song duration: " << song_duration;

               time_t start_time = time(NULL);
               time_t current_time = start_time;
               time_t last_print_time = start_time;

//               QThread* thr1 = new QThread;

//               connect(thr1, &QThread::started, worker, [worker, mh, buffer, buffer_size, done, pcm_handle, frames](){
//                   worker->playSong(mh, buffer, buffer_size, done, pcm_handle, frames);} );
//               thr1->start();



               // Play the MP3 fille
//               while (mpg123_read(mh, buffer, buffer_size, &done) == MPG123_OK) {
//                    snd_pcm_writei(pcm_handle, buffer, frames);
//                    current_time = time(NULL);
//                    if (difftime(current_time, last_print_time) >= 1.0) {
//                        off_t position = mpg123_tell(mh);
//                        qDebug() << "Current position in samples: " << position * mpg123_tpf(mh) / 1000;
//                        last_print_time = current_time;
//                    }
//               }
}


void MainWindow::on_pushButton_8_clicked()
{
    QList<int> params  = QList<int>() << 0 << 0 << 0 << 0 << 0;
    ui->widget->setEqualizersSettings(params);
}


void MainWindow::on_pushButton_7_clicked()
{
    QList<int> params  = QList<int>() << -1 << 2 << 5 << 1 << -2;
    ui->widget->setEqualizersSettings(params);
}


void MainWindow::on_pushButton_9_clicked()
{
    QList<int> params  = QList<int>() << 5 << 3 << 1 << 3 << 5;
    ui->widget->setEqualizersSettings(params);
}


void MainWindow::on_pushButton_6_clicked()
{
    QList<int> params  = QList<int>() << 6 << 0 << 2 << 4 << 1;
    ui->widget->setEqualizersSettings(params);
}

