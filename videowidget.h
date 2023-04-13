#ifndef VIDEOWIDGET_H
#define VIDEOWIDGET_H

#include "videoplayer.h"
#include <QImage>
#include <QWidget>
class VideoWidget : public QWidget
{
    Q_OBJECT
public:
    explicit VideoWidget(QWidget* parent = nullptr);
    ~VideoWidget();
    QImage* _image = nullptr;
    QRect   _rect;
    void    paintEvent(QPaintEvent* event) override;

    void freeImage();

signals:
public slots:
    void onPlayerFrameDecoded(VideoPlayer* player, uint8_t* data, VideoPlayer::VideoSwsSpec& spec);
    void onPlayerStateChanged(VideoPlayer* player);
};

#endif   // VIDEOWIDGET_H
