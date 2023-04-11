#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QFileDialog>
MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    _player = new VideoPlayer();
    connect(_player, &VideoPlayer::stateChanged, this, &MainWindow::onPlayerStateChanged);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_playBtn_clicked()
{
    VideoPlayer::State state = _player->getState();
    if (state == VideoPlayer::Playing) {
        _player->pause();
    }
    else {
        _player->play();
    }
}


void MainWindow::on_stopBtn_clicked()
{
    _player->stop();
}


void MainWindow::on_openFileBtn_clicked()
{
    QString filename =
        QFileDialog::getOpenFileName(nullptr,
                                     "选择多媒体文件",
                                     "/Users/zhangyujun/CLionProjects/video_player/",
                                     tr("多媒体文件 (*.mp4 *.avi *.mkv *.mp3 *.aac)"),
                                     0,
                                     QFileDialog::DontUseNativeDialog);
    if (filename.isEmpty()) return;

    qDebug() << filename;
    _player->setFilename(filename.toUtf8().data());
    _player->play();
}
void MainWindow::onPlayerStateChanged(VideoPlayer* player)
{
    VideoPlayer::State state = player->getState();
    if (state == VideoPlayer::Playing) {
        ui->playBtn->setText("暂停");
    }
    else {
        ui->playBtn->setText("播放");
    }

    if (state == VideoPlayer::Stopped) {
        ui->playBtn->setEnabled(false);
        ui->stopBtn->setEnabled(false);
        ui->currentlSlider->setEnabled(false);
        ui->volumnSlider->setEnabled(false);
        ui->silenceBtn->setEnabled(false);

        ui->durationLabel->setText("00:00:00");
        ui->currentlSlider->setValue(0);


        ui->playWidget->setCurrentWidget(ui->openFilePage);
    }
    else {
        ui->playBtn->setEnabled(true);
        ui->stopBtn->setEnabled(true);
        ui->currentlSlider->setEnabled(true);
        ui->volumnSlider->setEnabled(true);
        ui->silenceBtn->setEnabled(true);

        ui->playWidget->setCurrentWidget(ui->videoPage);
    }
};
void MainWindow::on_currentlSlider_valueChanged(int value)
{

    qDebug() << "on_currentlSlider_valueChanged" << value;
}


void MainWindow::on_volumnSlider_valueChanged(int value)
{
    ui->volumnLabel->setText(QString("%1").arg(value));
    qDebug() << "on_volumnSlider_valueChanged" << value;
}
