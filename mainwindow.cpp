#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    qRegisterMetaType<VideoPlayer::VideoSwsSpec>("VideoSwsSpec&");
    _player = new VideoPlayer();
    connect(_player, &VideoPlayer::stateChanged, this, &MainWindow::onPlayerStateChanged);
    connect(_player, &VideoPlayer::playFailed, this, &MainWindow::onPlayerPlayFailed);
    connect(_player, &VideoPlayer::timeChanged, this, &MainWindow::onPlayerTimeChanged);
    connect(_player, &VideoPlayer::initFinished, this, &MainWindow::onPlayerInitFinished);

    // 监听时间滑块的点击
    connect(ui->currentlSlider, &VideoSlider::clicked, this, &MainWindow::onSliderClicked);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete _player;
}

void MainWindow::onPlayerPlayFailed(VideoPlayer* player)
{
    QMessageBox::critical(nullptr, "提示", "播放失败");
}

void MainWindow::onSliderClicked(VideoSlider* slider)
{
    //_player->setTime(slider->value());
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
    _player->setFilename(filename);
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


void MainWindow::onPlayerInitFinished(VideoPlayer* player)
{
    //    int duration = player->getDuration();

    //    // 设置slider的范围
    //    ui->currentlSlider->setRange(0, duration);

    //    // 设置label的文字
    //    ui->durationLabel->setText(getTimeText(duration));
}

void MainWindow::onPlayerTimeChanged(VideoPlayer* player)
{
    // ui->currentlSlider->setValue(player->getTime());
}

QString MainWindow::getTimeText(int value)
{
    QLatin1Char fill = QLatin1Char('0');
    return QString("%1:%2:%3")
        .arg(value / 3600, 2, 10, fill)
        .arg((value / 60) % 60, 2, 10, fill)
        .arg(value % 60, 2, 10, fill);
}
