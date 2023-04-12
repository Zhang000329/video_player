#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "videoplayer.h"
#include "videoslider.h"
#include <QMainWindow>
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void on_playBtn_clicked();

    void on_stopBtn_clicked();

    void on_openFileBtn_clicked();

    void on_currentlSlider_valueChanged(int value);

    void on_volumnSlider_valueChanged(int value);

    void onPlayerStateChanged(VideoPlayer* player);

    void onPlayerPlayFailed(VideoPlayer* player);

    void onPlayerTimeChanged(VideoPlayer* player);

    void onPlayerInitFinished(VideoPlayer* player);

    void onSliderClicked(VideoSlider* slider);



private:
    Ui::MainWindow* ui;
    VideoPlayer*    _player;
    QString         getTimeText(int value);
};
#endif   // MAINWINDOW_H
