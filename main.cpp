#include "mainwindow.h"

#include <QApplication>

// 会和SDL中的main发生冲突
#undef main

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    MainWindow   w;
    w.show();
    return a.exec();
}
