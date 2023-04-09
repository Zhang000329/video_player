#include "videoslider.h"
#include <QMouseEvent>
#include <QStyle>
VideoSlider::VideoSlider(QWidget* parent)
    : QSlider(parent)
{}

void VideoSlider::mousePressEvent(QMouseEvent* ev)
{
    /**
     * @brief value
     * 这里不能直接简单的计算，因为随着对话窗口的改变，进度条的尺寸会随之变换
     * 所以需要计算每个的比例。
     */
    //    int value = (ev->pos().x() * 1.0 / width()) * (maximum() - minimum()) + minimum();
    //    setValue(value);
    int value = QStyle::sliderValueFromPosition(minimum(), maximum(), ev->pos().x(), width());
    setValue(value);
    QSlider::mousePressEvent(ev);
}
