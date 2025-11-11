
#include "USBDeviceEventFilter.h"

bool USBDeviceEventFilter::nativeEventFilter(const QByteArray &eventType, void *message, qintptr *data){
    MSG* msg = reinterpret_cast<MSG*>(message);
    if (msg->message == WM_DEVICECHANGE) {
        // if (msg->wParam == DBT_DEVICEARRIVAL) {
        //     // USB插入信号
        //     // emit DevicePlugIn();
        //     qDebug() << "DevicePlugIn";
        // } else if (msg->wParam == DBT_DEVICEREMOVECOMPLETE) {
        //     // USB拔出信号
        //     // emit DevicePlugOut();
        //     qDebug() << "DevicePlugOut";
        // }
        qDebug() << "DeviceEvent";
        emit DeviceEvent();
    }

    return false;
}
