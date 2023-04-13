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



增加标识位**QFileDialog::DontUseNativeDialog**，作用是不使用系统自带的弹窗。
弹窗不显示的原因：shell对话框陷入僵局并不罕见。当显示它的线程不是STA线程时，它将发生。或者，如果您在计算机上安装了外壳扩展，则表现不正常。问题根本没有提供线索，挂起的UI线程的堆栈跟踪是最低要求。

## Day2 完成音频的输出

重采样：将音频A转换成音频B，并且音频A、B的参数（采样率、采样格式、声道数）并不完全相同。其主要用于音视频的解码和压缩。

![image-20230413112615878](/Users/zhangyujun/Library/Application Support/typora-user-images/image-20230413112615878.png)

1.获取输入输出格式

2.创建重采样上下文**`swr_alloc_set_opts()`**

3.初始化重采样上下文**`swr_init()`**

4.创建缓冲区

5.读取数据到缓冲区

6.回收资源

### 遇到的问题

1.当使用**`filename.toStdString().c_str()` ** 解析出来的字符串出现乱码的问题：

解决方案：将**toStdString()**和**c_str()**分开来写

> 原因：s.c_str()返回的是垃圾指针，s已经被析构，所以会乱码

```c++
std::string tmp = filename.toStdString(); 
const char* name = tmp.c_str();
qDebug() << name;
memcpy(_filename, name, strlen(name) + 1);
```





