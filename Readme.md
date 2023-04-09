# Video player

1. 操作系统：mac
2. QT+FFmpeg+h264+RTMP

## Day1 完成简单的页面布局，并实现简单的功能

完成简单的界面搭建和简单的案件逻辑的实现。

### 遇到的问题

1.使用mac，当使用**`QFileDialog::getOpenFileName`**出现闪退的情况时，解决方法如下

```c++
QString filename =
        QFileDialog::getOpenFileName(nullptr,
                                     "选择多媒体文件",
                                     "/Users/zhangyujun/CLionProjects/video_player/",
                                     tr("多媒体文件 (*.mp4 *.avi *.mkv *.mp3 *.aac)"),
                                     0,
                                     QFileDialog::DontUseNativeDialog);
```



增加标识位QFileDialog::DontUseNativeDialog，作用是不使用系统自带的弹窗。
弹窗不显示的原因：shell对话框陷入僵局并不罕见。当显示它的线程不是STA线程时，它将发生。或者，如果您在计算机上安装了外壳扩展，则表现不正常。问题根本没有提供线索，挂起的UI线程的堆栈跟踪是最低要求。