#include "videoplayer.h"
#include <QDebug>
#include <QThread>
#include <thread>
VideoPlayer::VideoPlayer(QObject* parent)
    : QObject(parent)
{}
VideoPlayer::~VideoPlayer() {}

void VideoPlayer::play()
{
    if (_state == Playing) {
        return;
    }


    // 开启线程读取文件
    std::thread([this]() { readFile(); }).detach();


    setState(Playing);
}

void VideoPlayer::pause()
{
    if (_state != Playing) {
        return;
    }
    setState(Paused);
}


void VideoPlayer::stop()
{
    if (_state == Stopped) {
        return;
    }
    setState(Stopped);
}

bool VideoPlayer::isPlaying()
{
    return _state = Playing;
}

void VideoPlayer::setState(State state)
{
    if (state == _state) return;
    _state = state;
    emit stateChanged(this);
}

void VideoPlayer::setFilename(const char* filename)
{
    _filename = filename;
}

VideoPlayer::State VideoPlayer::getState()
{
    return _state;
}

void VideoPlayer::readFile() {}

int VideoPlayer::initVideoInfo() {}
int VideoPlayer::initAudioInfo() {}
int VideoPlayer::initDecoder(AVCodecContext** decodeCtx, AVStream** stream, AVMediaType type) {

}
