#ifndef VIDEOPLAYER_H
#define VIDEOPLAYER_H

#include <QObject>

class VideoPlayer : public QObject
{
    Q_OBJECT
public:
    typedef enum
    {
        Stopped = 0,
        Playing,
        Paused
    } State;
    explicit VideoPlayer(QObject* parent = nullptr);
    ~VideoPlayer();
    void  play();
    void  pause();
    void  stop();
    bool  isPlaying();
    State getState();
    void  setFilename(const char* filename);
signals:
    void stateChanged(VideoPlayer* videoplayer);

private:
    State       _state = Stopped;
    void        setState(State state);
    const char* _filename;
};

#endif   // VIDEOPLAYER_H
