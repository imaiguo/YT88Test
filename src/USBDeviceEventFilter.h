
#pragma once

#include <Windows.h>
#include <dbt.h>
#include <QWidget>
#include <QAbstractNativeEventFilter>

class USBDeviceEventFilter : public QWidget, public QAbstractNativeEventFilter{
    Q_OBJECT
public:
    bool nativeEventFilter(const QByteArray &eventType, void *message, qintptr *data) override;

signals:
    void DeviceEvent();
};
