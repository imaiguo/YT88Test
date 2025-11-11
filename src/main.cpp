
#include "DialogFunction.h"
#include "WindowKeyReadWrite.h"
#include "USBDeviceEventFilter.h"

#include <QApplication>

int main(int argc, char *argv[]){

    qputenv("QT_QPA_PLATFORM", "windows:darkmode=1");
    // qputenv("QT_ENABLE_HIGHDPI_SCALING", "0");
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QApplication app(argc, argv);
    app.setApplicationName("加密狗测试");

    USBDeviceEventFilter filter;
    app.installNativeEventFilter(&filter);


    // // 1. 厂家提供的示例程序
    // DialogFunction dialog;
    // dialog.show();

    // 2. 简单常用的方法调用
    WindowKeyReadWrite window;
    window.initUI();
    QObject::connect(&filter, &USBDeviceEventFilter::DeviceEvent, &window, &WindowKeyReadWrite::onbuttonDetectKey);
    window.show();

    return app.exec();
}
