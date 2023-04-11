#ifndef VIDEOPLAYER_H
#define VIDEOPLAYER_H

#include <QObject>
#include <condmutex.h>
#include <list>
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
}

#define ERROR_BUF      \
    char errbuf[1024]; \
    av_strerror(ret, errbuf, sizeof(errbuf));

// #func， 将传递的函数名，转化成字符串
#define CODE(func, code)                        \
    if (ret < 0) {                              \
        ERROR_BUF;                              \
        qDebug() << #func << "error" << errbuf; \
        code;                                   \
    }

#define END(func) CODE(func, fataError(); return;)
#define RET(func) CODE(func, return ret;)
#define CONTINUE(func) CODE(func, continue;)
#define BREAK(func) CODE(func, break;)

/**
 * 预处理视频数据（不负责显示、渲染视频）
 */
class VideoPlayer : public QObject
{
    Q_OBJECT
public:
    // 状态
    typedef enum
    {
        Stopped = 0,
        Playing,
        Paused
    } State;

    // 音量
    typedef enum
    {
        Min = 0,
        Max = 100
    } Volumn;

    // 视频frame参数
    typedef struct
    {
        int           width;
        int           height;
        AVPixelFormat pixFmt;
        int           size;
    } VideoSwsSpec;

    explicit VideoPlayer(QObject* parent = nullptr);
    ~VideoPlayer();

    /** 播放*/
    void play();
    /** 暂停*/
    void pause();
    /** 停止*/
    void stop();
    /** 是否正在播放*/
    bool isPlaying();
    /** 获取当前状态*/
    State getState();
    /** 设置文件名称*/
    void setFilename(const char* filename);


signals:
    void stateChanged(VideoPlayer* videoplayer);

private:
    State       _state = Stopped;
    void        setState(State state);
    const char* _filename;
    void        readFile();
    /** 解封装需要的参数*/
    AVFormatContext* _fmtCtx     = nullptr;
    AVCodecContext * _aDecodeCtx = nullptr, *_vDecodeCtx = nullptr;
    AVStream *       _aStream = nullptr, *_vStream = nullptr;
    AVFrame *        _aFrame = nullptr, *_vFrame = nullptr;

    int initVideoInfo();
    int initAudioInfo();
    int initDecoder(AVCodecContext** decodeCtx, AVStream** stream, AVMediaType type);
};

#endif   // VIDEOPLAYER_H
