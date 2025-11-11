
#include "WindowReadWrite.h"
#include "Config.h"

#include <QDebug>
#include <QMessageBox>
#include <QDateTime>

const static std::string YT88DeviceReadPasswordHigh = "31415926";
const static std::string YT88DeviceReadPasswordLow =  "53589793";

const static std::string YT88DeviceWritePasswordHigh ="23846264";
const static std::string YT88DeviceWritePasswordLow = "33832795";

void WindowReadWrite::initUI(){
    qDebug() << "WindowReadWrite::initUI";
    setWindowTitle("加密狗测试");

    int x = 40, y = 20;
    m_buttonDetectKey.setParent(this);
    m_buttonDetectKey.setText("检测加密狗[未插入]");
    m_buttonDetectKey.setStyleSheet(Config::styleButton);
    m_buttonDetectKey.move(x, y);

    x+=45, y+= 5;
    m_EditSerialNumber.setParent(this);
    m_EditSerialNumber.setText("未插入加密狗");
    m_EditSerialNumber.setFixedWidth(200);
    m_EditSerialNumber.move(x + 100, y);
    m_EditSerialNumber.setReadOnly(true);

    y += 35;
    m_buttonReadString.setParent(this);
    m_buttonReadString.setText("读取字符串");
    m_buttonReadString.setStyleSheet(Config::styleButton);
    m_buttonReadString.move(x, y);

    m_EditStringReadData.setParent(this);
    m_EditStringReadData.move(x + 100, y);
    m_EditStringReadData.setFixedWidth(200);
    m_EditStringReadData.setReadOnly(true);

    y += 40;
    m_buttonWriteString.setParent(this);
    m_buttonWriteString.setText("写入字符串");
    m_buttonWriteString.setStyleSheet(Config::styleButton);
    m_buttonWriteString.move(x, y);

    m_EditStringWriteData.setParent(this);
    m_EditStringWriteData.move(x + 100, y);
    m_EditStringWriteData.setFixedWidth(200);
    m_EditStringWriteData.setText("测试加密锁写入的字符串");

    y += 40;
    m_buttonInit.setParent(this);
    m_buttonInit.setText("发布加密狗");
    m_buttonInit.setStyleSheet(Config::styleButton);
    m_buttonInit.move(x, y);

    y += 60;
    m_buttonReset.setParent(this);
    m_buttonReset.setText("重置加密狗");
    m_buttonReset.setStyleSheet(Config::styleButton);
    m_buttonReset.move(x, y);

    setFixedSize(420, 400);
    addConnection();
}

void WindowReadWrite::addConnection(){
    qDebug() << "WindowReadWrite::addConnection";
    connect(&m_buttonDetectKey, &QPushButton::clicked, this, &WindowReadWrite::onbuttonDetectKey);
    connect(&m_buttonReadString, &QPushButton::clicked, this, &WindowReadWrite::onbuttonReadString);
    connect(&m_buttonWriteString, &QPushButton::clicked, this, &WindowReadWrite::onbuttonWriteString);
    connect(&m_buttonReset, &QPushButton::clicked, this, &WindowReadWrite::onbuttonReset);
    connect(&m_buttonInit, &QPushButton::clicked, this, &WindowReadWrite::onbuttonInit);
    ;
}

void WindowReadWrite::onbuttonDetectKey(){
    qDebug() << "WindowReadWrite::onbuttonDetectKey";
    char DevicePath[260];
    if (m_keyYT88.FindPort(0, DevicePath) != 0){
        QMessageBox::warning(this, QStringLiteral("错误"), QStringLiteral("未找到加密锁,请插入加密锁后，再进行操作。"), QMessageBox::Close);
    }

    m_devicePath = DevicePath;
    qDebug() << "device->[" << m_devicePath << "].";
    std::string num = getSerialNumber();
    qDebug() << "num->[" << num << "].";
    m_buttonDetectKey.setText("检测加密狗[已插入]");
    m_EditSerialNumber.setText(num.c_str());
    update();
}

//这个例子与上面的不同之处是，可以读取非固定长度的字符串，它是先从首地址读取字符串的长度，然后再读取相应的字符串
void WindowReadWrite::onbuttonReadString(){
    qDebug() << "WindowReadWrite::onbuttonReadString";
    int ret;
    short nlen;
    byte lengthBuffer[1];
    char bzStr[512];
    int addr=0;

    memset(bzStr, 0, 512);

    //先从地址0读到以前写入的字符串的长度
    ret = m_keyYT88.YReadEx(lengthBuffer, addr, 1, (char*)YT88DeviceReadPasswordHigh.c_str(), (char*)YT88DeviceReadPasswordLow.c_str(), (char*)m_devicePath.c_str());
    nlen = lengthBuffer[0];
    if( ret != 0 ){
        QMessageBox::warning(this, QStringLiteral("错误"), QStringLiteral("读取字符串长度错误。"), QMessageBox::Close);return ;
    }

    //outstring[nlen]=0;//将最后一个字符设置为0，即结束字符串
    memset(bzStr, 0, 512);

    //再读取相应长度的字符串
    ret = m_keyYT88.YReadString(bzStr, addr+1, nlen, (char*)YT88DeviceReadPasswordHigh.c_str(), (char*)YT88DeviceReadPasswordLow.c_str(), (char*)m_devicePath.c_str());
    if( ret != 0 ){
        QMessageBox::warning(this, QStringLiteral("错误"), QStringLiteral("读取字符串错误。"), QMessageBox::Close );
    }else{
        m_EditStringReadData.setText(QString::fromLocal8Bit(bzStr));
        qDebug() << "bzStr readed -> [" << bzStr <<"].";
    }
}

void WindowReadWrite::onbuttonWriteString(){
    qDebug() << "WindowReadWrite::onbuttonWriteString";
    if(writeDataToKey(m_EditStringWriteData.text().toStdString()))
        QMessageBox::information(NULL, QStringLiteral("成功"),QStringLiteral("写入字符串成功"), QMessageBox::Close);
    else
        QMessageBox::warning(NULL, QStringLiteral("错误"),QStringLiteral("写入字符串长度错误。"), QMessageBox::Close);
        
}

//用于将加密锁数据全部初始化为0，只适用于版本号大于或等于9以上的锁
void WindowReadWrite::onbuttonReset(){
    qDebug() << "WindowReadWrite::onbuttonReset";
    int ret = -1;
    // 1. 重新初始化加密狗
    ret = m_keyYT88.ReSet((char*)m_devicePath.c_str());
    if (ret != 0){
        QMessageBox::warning(NULL, QStringLiteral("错误"),QStringLiteral("初始化失败"), QMessageBox::Close);
        return;
    }

    //初始化成功，所有数据将回复到0的状态，读密码及新密码都全部为0
    //以下代码再将它重新设置为原来出厂时的FFFFFFFF-FFFFFFF
    // 2.先设置写密码
    ret = m_keyYT88.SetWritePassword((char*)"00000000", (char*)"00000000", (char*)"FFFFFFFF", (char*)"FFFFFFFF", (char*)m_devicePath.c_str());
    if (ret != 0){
        QMessageBox::warning(NULL, QStringLiteral("错误"),QStringLiteral("设置写密码错误。"), QMessageBox::Close);
        return;
    }

    //2. 再设置读密码
    // 注意，设置读密码是用原"写"密码进行设置，而不是原“读”密码
    ret =m_keyYT88.SetReadPassword((char*)"FFFFFFFF", (char*)"FFFFFFFF", (char*)"FFFFFFFF", (char*)"FFFFFFFF", (char*)m_devicePath.c_str());
    if (ret != 0){
        QMessageBox::warning(NULL, QStringLiteral("错误"),QStringLiteral("设置读密码错误。"), QMessageBox::Close);
        return;
    }

    QMessageBox::information(NULL, QStringLiteral("成功"), QStringLiteral("初始化成功!"), QMessageBox::Close);
}

//提示：开发商可以根据这个编码查询是否是我们公司的原厂锁，该编码是全球唯一的，将这个编码发给我们，我们就可以识别到是否是我们公司的原厂锁了
//开发商可以凭这个出厂编码来维护自己的正当权益，也可以将这个唯一编码用于加密
//出厂编码含义：YY--MM--DD--hh--mm--ss--NNNN  均以16进制的形式表示, 年--月--日--时--分--秒--4位随机序号
std::string WindowReadWrite::getSerialNumber(){
    char sn[17];
    memset(sn, 0, 17);
    std::string strNum = "";
    int version;

    //只有11版本的才支持这个功能
    if(m_keyYT88.NT_GetIDVersion(&version, (char *)m_devicePath.c_str()) != 0){
        QMessageBox::warning(this, QStringLiteral("错误"), QStringLiteral("返回加密锁版本号错误"), QMessageBox::Close);
        return strNum;
    }

    if (version < 11){
        QString msg = QStringLiteral("锁的版本少于11,不支持返回锁的出厂编码功能")+  QStringLiteral("\n") +  QStringLiteral("2013年9月15号以后出厂的锁都支持这个功能。");
        QMessageBox::warning(NULL, QStringLiteral("错误"), msg, QMessageBox::Close);
        return strNum;
    }

    if(m_keyYT88.GetProduceDate(sn, (char *)m_devicePath.c_str())!= 0){
        QMessageBox::warning(NULL, QStringLiteral("错误"),"返回出厂编码错误",QMessageBox::Close);
        return strNum;
    }

    strNum = std::string(&sn[12], 4);
    return strNum;
}

// 发行加密狗 [修改读写密码为指定密码 + 写入指定的数据]
void WindowReadWrite::onbuttonInit(){
    qDebug() << "WindowReadWrite::onbuttonInit";
    // 1. 修改读密码和写密码
    // 2.先设置写密码
    int ret = m_keyYT88.SetWritePassword((char*)"FFFFFFFF",
                                         (char*)"FFFFFFFF",
                                         (char*)YT88DeviceWritePasswordHigh.c_str(),
                                         (char*)YT88DeviceWritePasswordLow.c_str(),
                                         (char*)m_devicePath.c_str());

    if (ret != 0){
        QMessageBox::warning(NULL, QStringLiteral("错误"),QStringLiteral("设置写密码错误。"), QMessageBox::Close);
        return;
    }

    //2. 再设置读密码
    // 注意，设置读密码是用原"写"密码进行设置，而不是原“读”密码
    ret =m_keyYT88.SetReadPassword((char*)YT88DeviceWritePasswordHigh.c_str(),
                                   (char*)YT88DeviceWritePasswordLow.c_str(),
                                   (char*)YT88DeviceReadPasswordHigh.c_str(),
                                   (char*)YT88DeviceReadPasswordLow.c_str(),
                                   (char*)m_devicePath.c_str());
    if (ret != 0){
        QMessageBox::warning(NULL, QStringLiteral("错误"),QStringLiteral("设置读密码错误。"), QMessageBox::Close);
        return;
    }

    QMessageBox::information(NULL, QStringLiteral("成功"), QStringLiteral("初始化成功!"), QMessageBox::Close);

    // 2. 把日期和序列号写入盘中
    std::string num = getSerialNumber();
    QString data = QDateTime::currentDateTime().toString("yyyyMMddhhmmsszzz");
    data += num;
    writeDataToKey(data.toStdString());
}

bool WindowReadWrite::writeDataToKey(std::string data){
    //注意，如果是普通单片机芯片，储存器的写次数是有限制的，写次数为1000次，读不限制，如果是智能芯片，写的次数为10万次
    // '这个例子与上面的不同之处是，可以写入非固定长度的字符串，它是先将字符串的长度写入到首地址，然后再写入相应的字符串
    int ret = -1;
    int nlen = -1;
    byte lengthBuffer[1];
    int addr=0;
    nlen = data.length();
    lengthBuffer[0] = nlen;

    //写入字符串到地址1
    ret = m_keyYT88.YWriteString(data.data(), addr+1, (char*)YT88DeviceWritePasswordHigh.c_str(), (char*)YT88DeviceWritePasswordLow.c_str(), (char*)m_devicePath.c_str());
    if( ret != 0 ){
        // QMessageBox::warning(NULL, QStringLiteral("错误"),QStringLiteral("写入字符串错误。"), QMessageBox::Close );
        return false;
    }

   //写入字符串的长度到地址0
    ret = m_keyYT88.YWriteEx(lengthBuffer, addr, 1, (char*)YT88DeviceWritePasswordHigh.c_str(), (char*)YT88DeviceWritePasswordLow.c_str(), (char*)m_devicePath.c_str());
    if(ret != 0)
        return false;

    return true;
}
