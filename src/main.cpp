
#include "DialogFunction.h"
#include "WindowReadWrite.h"

#include <QApplication>

int main(int argc, char *argv[]){

    qputenv("QT_QPA_PLATFORM", "windows:darkmode=1");
    // qputenv("QT_ENABLE_HIGHDPI_SCALING", "0");
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QApplication a(argc, argv);
    a.setApplicationName("加密狗测试");

    // // 1. 厂家提供的示例程序
    // DialogFunction dialog;
    // dialog.show();

    // 2. 简单常用的方法调用
    WindowReadWrite window;
    window.initUI();
    window.show();

    return a.exec();
}
