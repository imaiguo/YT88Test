
#pragma once

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>

#include "SoftkeyYT88.h"

class WindowReadWrite : public QWidget{
Q_OBJECT;
public:
    WindowReadWrite(QWidget* parent=nullptr):QWidget(parent){};
    void initUI();

private:
    QPushButton m_buttonDetectKey;
    QPushButton m_buttonReadString;
    QPushButton m_buttonWriteString;
    QPushButton m_buttonInit;
    QPushButton m_buttonReset;

    QLineEdit  m_EditSerialNumber;
    QLineEdit m_EditStringReadData;
    QLineEdit m_EditStringWriteData;

    SoftkeyYT88 m_keyYT88;
    std::string m_devicePath;

private:
    void addConnection();

    //用于返回锁的出厂编码。
    std::string getSerialNumber();
    bool writeDataToKey(std::string data);

public slots:
    void onbuttonDetectKey();
    void onbuttonReadString();
    void onbuttonWriteString();
    void onbuttonInit();
    void onbuttonReset();
};