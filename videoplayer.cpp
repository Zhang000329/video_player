#include "videoplayer.h"
#include <QDebug>
#include <thread>

#define AUDIO_MAX_PKT_SIZE 1000
#define VIDEO_MAX_PKT_SIZE 500
VideoPlayer::VideoPlayer(QObject* parent)
    : QObject(parent)
{
    // 初始化Audio子系统
    if (SDL_Init(SDL_INIT_AUDIO)) {
        qDebug() << "SDL_Init error" << SDL_GetError();
        emit playFailed(this);
        return;
    }
}
VideoPlayer::~VideoPlayer()
{
    disconnect();
    stop();
    SDL_Quit();
}

void VideoPlayer::play()
{
    if (_state == Playing) {
        return;
    }

    if (_state == Stopped) {
        // 开启线程读取文件
        std::thread([this]() { readFile(); }).detach();
    }
    else {
        setState(Playing);
    }
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
    _state = Stopped;
    free();

    emit stateChanged(this);
}

bool VideoPlayer::isPlaying()
{
    return _state = Playing;
}

VideoPlayer::State VideoPlayer::getState()
{
    return _state;
}

void VideoPlayer::setState(State state)
{
    if (state == _state) return;
    _state = state;
    emit stateChanged(this);
}

void VideoPlayer::setFilename(QString& filename)
{
    const char* name = filename.toStdString().c_str();
    memcpy(_filename, name, strlen(name) + 1);
}

// TODO: 音频的读取
void VideoPlayer::readFile()
{
    int ret = 0;
    ret     = avformat_open_input(&_fmtCtx, _filename, nullptr, nullptr);
    END(avformat_open_input);

    ret = avformat_find_stream_info(_fmtCtx, nullptr);
    END(avformat_find_stream_info);

    av_dump_format(_fmtCtx, 0, _filename, 0);
    fflush(stderr);

    _hasAudio = initAudioInfo() >= 0;

    if (!_hasAudio) {
        fataError();
        return;
    }

    emit initFinished(this);
    setState(Playing);
    SDL_PauseAudio(0);


    AVPacket pkt;
    while (_state != Stopped) {
        if (_seekTime >= 0) {
            int streamIdx;
            if (_hasAudio) {
                streamIdx = _aStream->index;
            }
            else {}

            AVRational timeBase = _fmtCtx->streams[streamIdx]->time_base;
            int64_t    ts       = _seekTime / av_q2d(timeBase);
            ret                 = av_seek_frame(_fmtCtx, streamIdx, ts, AVSEEK_FLAG_BACKWARD);

            if (ret < 0) {
                qDebug() << "seek失败" << _seekTime << ts << streamIdx;
            }
            else {
                qDebug() << "seek成功" << _seekTime << ts << streamIdx;
                _aSeekTime = _seekTime;
                _seekTime  = -1;
                _aTime     = 0;
                clearAudioPktList();
            }
        }
        int aSize = _aPktList.size();

        ret = av_read_frame(_fmtCtx, &pkt);
        if (ret == 0) {
            if (pkt.stream_index == _aStream->index) {
                addAudioPkt(pkt);
            }
            else {
                av_packet_unref(&pkt);
            }
        }
        else if (ret == AVERROR_EOF) {
            if (aSize == 0) {
                _fmtCtxCanFree = true;
                break;
            }
        }
        else {
            ERROR_BUF;
            qDebug() << "av_read_frame error" << errbuf;
            continue;
        }
    }
    if (_fmtCtxCanFree) {
        stop();
    }
    else {
        _fmtCtxCanFree = true;
    }
}
int VideoPlayer::initDecoder(AVCodecContext** decodeCtx, AVStream** stream, AVMediaType type)
{
    int ret = av_find_best_stream(_fmtCtx, type, -1, -1, nullptr, 0);
    RET(av_find_best_stream);
    int streamIdx = ret;
    *stream       = _fmtCtx->streams[streamIdx];
    if (!*stream) {
        qDebug() << "stream is empty";
        return -1;
    }

    AVCodec* decoder = avcodec_find_decoder((*stream)->codecpar->codec_id);
    if (!decoder) {
        qDebug() << "decoder not find" << (*stream)->codecpar->codec_id;
        return -1;
    }
    *decodeCtx = avcodec_alloc_context3(decoder);
    if (!decodeCtx) {
        qDebug() << "avcodec_alloc_context3 error";
        return -1;
    }

    ret = avcodec_parameters_to_context(*decodeCtx, (*stream)->codecpar);
    RET(avcodec_parameters_to_context);
    ret = avcodec_open2(*decodeCtx, decoder, nullptr);
    RET(avcodec_open2);
    return 0;
}
void VideoPlayer::free()
{
    while (_hasAudio && !_aCanFree)
        ;

    while (!_fmtCtxCanFree)
        ;
    avformat_close_input(&_fmtCtx);
    _fmtCtxCanFree = false;
    _seekTime      = -1;
    freeAudio();
}

void VideoPlayer::fataError()
{
    _state = Playing;
    stop();
    emit playFailed(this);
}
