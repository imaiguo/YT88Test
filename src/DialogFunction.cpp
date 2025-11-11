#include "DialogFunction.h"
#include "ui_dialog.h"
#include <Windows.h>
#include <tchar.h>
#include <QMessageBox>


DialogFunction::DialogFunction(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    ytsoftkey=new SoftkeyYT88();
    if(!ytsoftkey->IsLoad){
         QMessageBox::warning(NULL, QStringLiteral("错误"),QStringLiteral("加载类DLL时错误。"), QMessageBox::Close);
         exit(0);
    }
    //这个用于判断系统中是否存在着加密锁。不需要是指定的加密锁,
   if (ytsoftkey->FindPort(0, DevicePath) != 0){
       QMessageBox::warning(NULL, QStringLiteral("错误"),QStringLiteral("未找到加密锁,请插入加密锁后，再进行操作。"), QMessageBox::Close);
        exit(0);
   }
}

DialogFunction::~DialogFunction()
{
    delete ui;
}

void DialogFunction::on_GetIDButton_clicked()
{
    /*'用于返回加密狗的ID号，加密狗的ID号由两个长整型组成。
        '提示1：锁ID可以是开发商唯一或每一把都是唯一的，开发商唯一是指同一开发商相同，不同的开发商不相同，每一把是唯一的，是指每一把锁的ID都不相同
        '提示2、如果是每一把都是唯一的，需要在订货时告知我们)
        '提示3: ID唯一是指两个ID转化为16进制字符串后并连接起来后是唯一的*/
        DWORD ID1,ID2;
        if(ytsoftkey->GetID(&ID1,&ID2,DevicePath)!=0){ QMessageBox::warning(NULL, QStringLiteral("错误"), QStringLiteral("返回ID错误"), QMessageBox::Close);return ;}
        char sID[17];
        sprintf(sID,"%X%X",ID1,ID2);
        ui->IDText->setText(tr(sID));
}

void DialogFunction::on_GetVerButton_clicked()
{
    //用于返回加密狗的版本号
    int ver;
    if(ytsoftkey->NT_GetIDVersion(&ver,DevicePath) != 0){ QMessageBox::warning(NULL, QStringLiteral("错误"), QStringLiteral("返回版本号错误"), QMessageBox::Close); return;}
    ui->VerText->setText(tr("%1").arg(ver));
}

void DialogFunction::on_FindPort_2Button_clicked()
{
    //使用普通算法一来查找指定的加密锁
        /*查找是否存在指定的加密狗,如果找到，则返回0,DevicePath为锁所在的返回设备所在的路径。
        注意！！！！！！！！！这里的参数“1”及参数“134226688”，随每个软件开发商的不同而不同，因为每个开发商的加密锁的加密算法都不一样，
        1、运行我们的开发工具，
        2、在“算法设置及测试页”-》“加密”-》“请输入要加密的数据”那里随意输入一个数
        3、然后单击“加密数据(使用普通算法一)”
        4、然后就会返回对应的数据(即“加密后的数据”)，
        然后将输入的数和返回的数替换这里的参数“1”及参数“134226688”*/

      if (ytsoftkey->FindPort_2(0, 1, 134226688, DevicePath) != 0)
            QMessageBox::warning(NULL, QStringLiteral("错误"),QStringLiteral("未找到指定的加密锁"),QMessageBox::Close);
       else
            QMessageBox::information(NULL, QStringLiteral("成功"),QStringLiteral("找到指定的加密锁"),QMessageBox::Close);
}


void DialogFunction::on_WriteEx_Button_clicked()
{
    //对输入的数进行加密运算，然后读出加密运算后的结果，(使用普通算法一)
    DWORD m_in1=ui->mInText_1->toPlainText().toUInt();
    DWORD m_out1;QString c_out1;
     if( ytsoftkey->sWriteEx(m_in1,&m_out1, DevicePath)!= 0 ){QMessageBox::warning(NULL, QStringLiteral("错误"),QStringLiteral("加密错误"),QMessageBox::Close);return ;}
     c_out1.setNum(m_out1);
     ui->mOutText_1->setText(c_out1);
}

void DialogFunction::on_sWrite_2Ex_Button_clicked()
{
    //对输入的数进行解密运算，然后读出解密运算后的结果(使用普通算法一)
    DWORD m_in2=ui->mInText_2->toPlainText().toUInt();
    DWORD m_out2;QString c_out2;
    if( ytsoftkey->sWrite_2Ex(m_in2,&m_out2,DevicePath)!= 0 ){QMessageBox::warning(NULL, QStringLiteral("错误"),QStringLiteral("解密错误"),QMessageBox::Close);return ;}
    c_out2.setNum(m_out2);
    ui->mOutText_2->setText(c_out2);
}


void DialogFunction::on_WriteStr_Button_clicked()
{
    //注意，如果是普通单片机芯片，储存器的写次数是有限制的，写次数为1000次，读不限制，如果是智能芯片，写的次数为10万次
    //写入字符串到加密锁中,使用默认的写密码
    int ret ;
    QString c_len;
    QString InString= QString::fromLocal8Bit("加密锁");
    ret = ytsoftkey->YWriteString((InString.toLocal8Bit()).data(), 0, (char*)"ffffffff", (char*)"ffffffff", DevicePath);
    if (ret != 0 ){ QMessageBox::warning(NULL, QStringLiteral("错误"),QStringLiteral("写字符串失败"), QMessageBox::Close) ; return ;}
    c_len=tr("%1").arg((InString.toLocal8Bit()).length());
    QMessageBox::information(NULL, QStringLiteral("成功"),QStringLiteral("写入成功。写入的字符串的长度是：")+ c_len, QMessageBox::Close);
}

void DialogFunction::on_ReadStr_Button_clicked()
{
    //从加密锁中读取字符串,使用默认的读密码
    char *outstring ;
    short mylen ;
    mylen = 6;//注意这里的6是长度，长度要与写入的字符串的长度相同
    outstring = new char[mylen+1 ];//注意，这里要加1一个长度，用于储存结束学符串，
    memset(outstring,0,mylen+1);//outstring[mylen]=0;//将最后一个字符设置为0，即结束字符串
    if (ytsoftkey->YReadString(outstring, 0, mylen, (char*)"ffffffff", (char*)"ffffffff", DevicePath) != 0) { QMessageBox::warning(NULL, QStringLiteral("错误"),QStringLiteral("读字符串失败"), QMessageBox::Close); goto exit1; }
    QMessageBox::information(NULL, QStringLiteral("成功"),QStringLiteral("读字符串成功：")+QString::fromLocal8Bit(outstring), QMessageBox::Close);
exit1:
    delete [] outstring;
}

void DialogFunction::on_WriteStr_Len_Button_clicked()
{
    //注意，如果是普通单片机芯片，储存器的写次数是有限制的，写次数为1000次，读不限制，如果是智能芯片，写的次数为10万次
    // '这个例子与上面的不同之处是，可以写入非固定长度的字符串，它是先将字符串的长度写入到首地址，然后再写入相应的字符串
    int ret;
    int nlen;
    QString InString;
    byte  buf[1];
    int addr=0;//要写入的地址
    InString = QString::fromLocal8Bit("加密锁");
    nlen = (InString.toLocal8Bit()).length();
    buf[0] = nlen;

    //写入字符串到地址1
    ret = ytsoftkey->YWriteString((InString.toLocal8Bit()).data(), addr+1, (char*)"ffffffff", (char*)"ffffffff", DevicePath);
    if( ret != 0 )
    {
        QMessageBox::warning(NULL, QStringLiteral("错误"),QStringLiteral("写入字符串错误。"), QMessageBox::Close );return  ;
    }
   //写入字符串的长度到地址0
    ret = ytsoftkey->YWriteEx(buf, addr, 1, (char*)"ffffffff", (char*)"ffffffff", DevicePath);
    if( ret != 0 )
        QMessageBox::warning(NULL, QStringLiteral("错误"),QStringLiteral("写入字符串长度错误。"), QMessageBox::Close);
    else
        QMessageBox::information(NULL, QStringLiteral("成功"),QStringLiteral("写入字符串成功"), QMessageBox::Close);
}

void DialogFunction::on_ReadStr_Len_Button_clicked()
{
    //这个例子与上面的不同之处是，可以读取非固定长度的字符串，它是先从首地址读取字符串的长度，然后再读取相应的字符串
    int ret;
    short nlen;
    byte buf[1];
    char  *outstring;
    int addr=0;//要读取的地址
    //先从地址0读到以前写入的字符串的长度
    ret = ytsoftkey->YReadEx(buf, addr, 1, (char*)"ffffffff", (char*)"ffffffff", DevicePath);
    nlen = buf[0];
    if( ret != 0 ){
        QMessageBox::warning(NULL, QStringLiteral("错误"), QStringLiteral("读取字符串长度错误。"), QMessageBox::Close);return ;
        return;
    }

    //这里要加1一个长度，用于储存结束学符串，
    outstring = new char[nlen+1 ];

    //outstring[nlen]=0;//将最后一个字符设置为0，即结束字符串
    memset(outstring, 0, nlen+1);

    //再读取相应长度的字符串
    ret = ytsoftkey->YReadString(outstring, addr+1, nlen, (char*)"ffffffff", (char*)"ffffffff", DevicePath);
    if( ret != 0 )
        QMessageBox::warning(NULL, QStringLiteral("错误"),QStringLiteral("读取字符串错误。"), QMessageBox::Close );
    else
        QMessageBox::information(NULL, QStringLiteral("成功"),QStringLiteral("已成功读取字符串：") + QString::fromLocal8Bit(outstring), QMessageBox::Close);
    delete [] outstring;
}

void DialogFunction::on_WriteByte_Button_clicked()
{
    //向加密锁指定的地址中写入一批数据  ，开始地址为0  ，使用默认的写密码
    int n;
    char HKey[20]="FFFFFFFF",LKey[20]="FFFFFFFF";//写密码
    BYTE Indata[2048];
    short len = 300;//要写入300个字节的数据
    for( n = 0;n<len;n++)
    {
        Indata[n] =(byte) n;
    }
    if(ytsoftkey->YWriteEx(Indata, 0,len,HKey,LKey, DevicePath) != 0 ){QMessageBox::warning(NULL, QStringLiteral("错误"),QStringLiteral("写储存器错误"), QMessageBox::Close);return ;}
    QMessageBox::information(NULL, QStringLiteral("成功"),QStringLiteral("写入成功。"), QMessageBox::Close);
}

void DialogFunction::on_ReadByte_Button_clicked()
{
    //从加密锁的指定的地址中读取一批数据,使用默认的读密码,开始地址为0
    char HKey[20]="FFFFFFFF",LKey[20]="FFFFFFFF";    //读密码
    short len = 300;//要读取300个字节的数据
    BYTE *Outdata=new BYTE[len];
    if(ytsoftkey->YReadEx(Outdata,0,len, HKey,LKey,DevicePath)!=0){ QMessageBox::warning(NULL, QStringLiteral("错误"),QStringLiteral("读储存器错误"), QMessageBox::Close);return;}
    QMessageBox::information(NULL, QStringLiteral("成功"),QStringLiteral("读取成功。"), QMessageBox::Close);
}

void DialogFunction::on_SetWritePwd_Button_clicked()
{
    // 设置锁的写密码
    if(ytsoftkey->SetWritePassword((char *)ui->Old_WPWD_H_1->text().toStdString().c_str(),(char *)ui->Old_WPWD_L_1->text().toStdString().c_str(),
                                   (char *)ui->New_WPWD_H->text().toStdString().c_str(),(char *)ui->New_WPWD_L->text().toStdString().c_str(), DevicePath)!=0){ QMessageBox::warning(NULL, QStringLiteral("错误"),QStringLiteral("设置写密码失败"), QMessageBox::Close);return;}
    QMessageBox::information(NULL, QStringLiteral("成功"),QStringLiteral("设置写密码成功"), QMessageBox::Close);
}

void DialogFunction::on_SetReadPwd_Button_clicked()
{
    // 设置锁的读密码，注意设置锁的读密码，是输入原来的“写”密码，而不是原来的“读”密码
    if(ytsoftkey->SetReadPassword((char *)ui->Old_WPWD_H_2->text().toStdString().c_str(),(char *)ui->Old_WPWD_L_2->text().toStdString().c_str(),
                                  (char *)ui->New_RPWD_H->text().toStdString().c_str(),(char *)ui->New_RPWD_L->text().toStdString().c_str(), DevicePath)!=0){ QMessageBox::warning(NULL, QStringLiteral("错误"),QStringLiteral("设置读密码失败"), QMessageBox::Close);return;}
    QMessageBox::information(NULL, QStringLiteral("成功"),QStringLiteral("设置读密码成功"), QMessageBox::Close);
}

void DialogFunction::on_SetCal_2_Button_clicked()
{
    //设置增强算法密钥一
   //注意：密钥为不超过32个的0-F字符，例如：1234567890ABCDEF1234567890ABCDEF,不足32个字符的，系统会自动在后面补0
    int ret;
    char Key[33]="1234567890ABCDEF1234567890ABCDEF";
    ret = ytsoftkey->SetCal_2(Key, DevicePath);
    if (ret != 0) { QMessageBox::warning(NULL, QStringLiteral("错误"),QStringLiteral("设置增强算法密钥错误"), QMessageBox::Close); return; }
    QMessageBox::information(NULL, QStringLiteral("成功"),QStringLiteral("已成功设置了增强算法密钥"), QMessageBox::Close);
}

void DialogFunction::on_EncString_Button_clicked()
{
    //'使用增强算法一对字符串进行加密
        int ret,nlen;
        QString InString;
        char *outstring;
        InString = QString::fromLocal8Bit("加密锁");
        nlen = (InString.toLocal8Bit()).length() + 1;
        if( nlen < 8 ) nlen = 8;
        outstring = new char[nlen * 2+1];//注意，这里要加1一个长度，用于储存结束学符串
        ret = ytsoftkey->EncString((InString.toLocal8Bit()).data(), outstring, DevicePath);
        if (ret != 0) { QMessageBox::warning(NULL, QStringLiteral("错误"),QStringLiteral("加密字符串出现错误"), QMessageBox::Close); return; }
        QMessageBox::information(NULL, QStringLiteral("成功"),QStringLiteral("已成功对字符串进行加密，加密后的字符串为：") + QString::fromLocal8Bit(outstring), QMessageBox::Close);
        delete [] outstring;
        //推荐加密方案：生成随机数，让锁做加密运算，同时在程序中端使用代码做同样的加密运算，然后进行比较判断。
        //'以下是对应的加密代码，可以参考使用
        QString InString_2;
        char *outstring_2,*outstring_3;
        InString_2 = QString::fromLocal8Bit("加密锁");
        nlen = (InString.toLocal8Bit()).length()+ 1;
        if( nlen < 8 ) nlen = 8;
        outstring_2 = new char[nlen * 2+1];//注意，这里要加1一个长度，用于储存结束学符串
        ytsoftkey->StrEnc((InString.toLocal8Bit()).data(),outstring_2, (char*)"1234567890ABCDEF1234567890ABCDEF");
        outstring_3 = new char[nlen];
        ytsoftkey->StrDec(outstring_2,outstring_3, (char*)"1234567890ABCDEF1234567890ABCDEF");
        delete [] outstring_2;delete [] outstring_3;
}

void DialogFunction::on_Cal_Button_clicked()
{
    //使用增强算法一对二进制数据进行加密
   int ret,n;
   byte InBuf[8];
   byte OutBuf[8];

   for(n=0;n<8;n++)
   {
       InBuf[n]=byte(n);
   }
   ret = ytsoftkey->Cal(InBuf, OutBuf, DevicePath);
   if (ret != 0) { QMessageBox::warning(NULL, QStringLiteral("错误"),QStringLiteral("加密数据时失败"), QMessageBox::Close); return; }
   QMessageBox::information(NULL, QStringLiteral("成功"),QStringLiteral("已成功对二进制数据进行了加密"), QMessageBox::Close);
   //推荐加密方案：生成随机数，让锁做加密运算，同时在程序中端使用代码做同样的加密运算，然后进行比较判断。
   //以下是对应的加密代码，可以参考使用
   byte outbuf_2[8];
   ytsoftkey->EnCode(InBuf, OutBuf, (char*)"1234567890ABCDEF1234567890ABCDEF");
   ytsoftkey->DeCode(OutBuf, outbuf_2, (char*)"1234567890ABCDEF1234567890ABCDEF");
}

void DialogFunction::on_SetCal_New_Button_clicked()
{
    //设置增强算法密钥二
   //注意：密钥为不超过32个的0-F字符，例如：1234567890ABCDEF1234567890ABCDEF,不足32个字符的，系统会自动在后面补0
    int ret;
    char Key[33]="ABCDEF1234567890ABCDEF1234567890";
    ret = ytsoftkey->SetCal_New(Key, DevicePath);
    if (ret != 0) { QMessageBox::warning(NULL, QStringLiteral("错误"),QStringLiteral("设置增强算法密钥错误"), QMessageBox::Close); return; }
    QMessageBox::information(NULL, QStringLiteral("成功"),QStringLiteral("已成功设置了增强算法密钥"), QMessageBox::Close);
}

void DialogFunction::on_EncString_New_Button_clicked()
{
    //'使用增强算法二对字符串进行加密
    int ret,nlen;
    QString InString;
    char *outstring;
    InString = QString::fromLocal8Bit("加密锁");
    nlen =(InString.toLocal8Bit()).length() + 1;
    if( nlen < 8 ) nlen = 8;
    outstring = new char[nlen * 2+1];//注意，这里要加1一个长度，用于储存结束学符串
    ret = ytsoftkey->EncString_New((InString.toLocal8Bit()).data(), outstring, DevicePath);
    if (ret != 0) { QMessageBox::warning(NULL, QStringLiteral("错误"),QStringLiteral("加密字符串出现错误"), QMessageBox::Close); return; }
   QMessageBox::information(NULL, QStringLiteral("成功"),QStringLiteral("已成功对字符串进行加密，加密后的字符串为：" )+ QString::fromLocal8Bit(outstring), QMessageBox::Close);
    delete [] outstring;
}

void DialogFunction::on_Cal_New_Button_clicked()
{
    //使用增强算法二对二进制数据进行加密
   int ret,n;
   byte InBuf[8];
   byte OutBuf[8];

   for(n=0;n<8;n++)
   {
       InBuf[n]=byte(n);
   }
   ret = ytsoftkey->Cal_New(InBuf, OutBuf, DevicePath);
   if (ret != 0) { QMessageBox::warning(NULL, QStringLiteral("错误"),QStringLiteral("加密数据时失败"), QMessageBox::Close); return; }
   QMessageBox::information(NULL, QStringLiteral("成功"),QStringLiteral("已成功对二进制数据进行了加密"), QMessageBox::Close);
}

void DialogFunction::on_Reset_Button_clicked()
{
    //用于将加密锁数据全部初始化为0，只适用于版本号大于或等于9以上的锁
       int ret;
       ret = ytsoftkey->ReSet(DevicePath);
       if (ret != 0)
       {
           QMessageBox::warning(NULL, QStringLiteral("错误"),QStringLiteral("初始化失败"), QMessageBox::Close);
           return;
       }

       //初始化成功，所有数据将回复到0的状态，读密码及新密码都全部为0
       //以下代码再将它重新设置为原来出厂时的FFFFFFFF-FFFFFFF
       //先设置写密码
       ret = ytsoftkey->SetWritePassword((char*)"00000000", (char*)"00000000", (char*)"FFFFFFFF", (char*)"FFFFFFFF", DevicePath);
       if (ret != 0)
       {
           QMessageBox::warning(NULL, QStringLiteral("错误"),QStringLiteral("设置写密码错误。"), QMessageBox::Close); return;
       }
       //再设置读密码,注意，设置读密码是用原"写"密码进行设置，而不是原“读”密码
       ret =ytsoftkey->SetReadPassword((char*)"FFFFFFFF", (char*)"FFFFFFFF", (char*)"FFFFFFFF", (char*)"FFFFFFFF", DevicePath);
       if (ret != 0)
       {
           QMessageBox::warning(NULL, QStringLiteral("错误"),QStringLiteral("设置读密码错误。"), QMessageBox::Close); return;
       }
       QMessageBox::information(NULL, QStringLiteral("成功"),QStringLiteral("初始化成功。"), QMessageBox::Close);
}

void DialogFunction::on_SetUKey_Button_clicked()
{
    //以下代码只支持iKey系列
    /*if (FindU(0,DevicePath) != 0)
    {
        QMessageBox::warning(NULL, QStringLiteral("错误"),QStringLiteral("未找到iKey,请插入加密锁后，再进行操作。", QMessageBox::Close);
        return;
    }*/
    int ver;
    if(ytsoftkey->NT_GetVersionEx(&ver,DevicePath) != 0){ QMessageBox::warning(NULL, QStringLiteral("错误"),QStringLiteral("返回加密锁扩展版本号错误"),QMessageBox::Close); return;}
    if(ver<38) {QMessageBox::warning(NULL, QStringLiteral("错误"),QStringLiteral("锁的扩展版本少于38,不支持带U盘功能"),QMessageBox::Close); return;}
    if( ytsoftkey->SetUReadOnly(DevicePath)!= 0 ){QMessageBox::warning(NULL, QStringLiteral("错误"),QStringLiteral("设置iKey为只读模式时错误"),QMessageBox::Close);return ;}
    if( ytsoftkey->SetHidOnly(TRUE,DevicePath)!= 0 ){QMessageBox::warning(NULL, QStringLiteral("错误"),QStringLiteral("设置iKey不显示盘符时错误"),QMessageBox::Close);return ;}
    QMessageBox::information(NULL, QStringLiteral("成功"),QStringLiteral("设置成功，需要重新插入iKey才生效。"),QMessageBox::Close);
}



void DialogFunction::on_GetProdeceDateButton_clicked()
{
    //用于返回锁的出厂编码。
    //提示：开发商可以根据这个编码查询是否是我们公司的原厂锁，该编码是全球唯一的，将这个编码发给我们，我们就可以识别到是否是我们公司的原厂锁了
    //开发商可以凭这个出厂编码来维护自己的正当权益，也可以将这个唯一编码用于加密
    //出厂编码含义：YY--MM--DD--hh--mm--ss--NNNN  均以16进制的形式表示
    //              年--月--日--时--分--秒--4位随机序号
    char sn[17]=""; char ProduceDate[100]="";

    //只有11版本的才支持这个功能
    int ver;
    if( ytsoftkey->NT_GetIDVersion(&ver, DevicePath) != 0){ QMessageBox::warning(NULL, QStringLiteral("错误"), QStringLiteral("返回加密锁版本号错误"),QMessageBox::Close);return ;}
    if (ver < 11){QMessageBox::warning(NULL, QStringLiteral("错误"), QStringLiteral("锁的版本少于11,不支持返回锁的出厂编码功能")+  QStringLiteral("\n") +  QStringLiteral("2013年9月15号以后出厂的锁都支持这个功能。"),QMessageBox::Close);return ;}

    if(ytsoftkey->GetProduceDate(sn, DevicePath)!= 0){ QMessageBox::warning(NULL, QStringLiteral("错误"),"返回出厂编码错误",QMessageBox::Close);return ;}
    ytsoftkey->SnToProduceDate(sn,ProduceDate);
    QMessageBox::information(NULL,  QStringLiteral("成功"),
                             QStringLiteral("已成功返回锁的出厂编码：") + QString::fromLocal8Bit(ProduceDate) + QStringLiteral("\n") + QStringLiteral("表示如下：") + QString::fromLocal8Bit(ProduceDate),QMessageBox::Close);
}

void DialogFunction::on_SetIDButton_clicked()
{
    //1、设置锁的ID,设置之前建议设置好锁的增强算法二
    //2、对于同一增强算法密钥二，不同的ID种子，生成的ID不相同
    //3、对于同一ID种了，不同的增强算法密钥二，生成的ID也不相同
    //4、根据已知ID，无法推算出ID种子及增强算法密钥二
    //5、即你生成的ID，只要ID种子及增强算法密钥二不让别人知道，别人就没有办法生成同一ID
    int ver;
    if(ytsoftkey->NT_GetVersionEx(&ver,DevicePath) != 0){QMessageBox::warning(NULL, QStringLiteral("错误"), QStringLiteral("返回加密锁扩展版本号错误"),QMessageBox::Close); return;}
    if(ver<32) {QMessageBox::warning(NULL, QStringLiteral("错误"),QStringLiteral("锁的扩展版本少于32,不支持设置锁的ID"),QMessageBox::Close); return;}
    char seed[17]="1234567890ABCDEF";//ID种子
    if(ytsoftkey->SetID(seed, DevicePath)==0)
       QMessageBox::information(NULL,  QStringLiteral("成功"), QStringLiteral("设置ID成功。"),QMessageBox::Close);
    else
       QMessageBox::warning(NULL, QStringLiteral("错误"), QStringLiteral("设置ID错误。"),QMessageBox::Close);
}

void DialogFunction::on_pushButton_clicked()
{
    //设置锁的普通算法密钥 ，要成功设置锁的普通算法密钥，必须要提供原算法密钥
    //对于同一数据，设置不同的算法密钥，加密或解密后的结果不一样
    if(ytsoftkey->SetCal((char*)"FFFFFFFF", (char*)"FFFFFFFF", (char*)"FFFFFFFF", (char*)"FFFFFFFF", DevicePath)==0)
    {
        QMessageBox::information(NULL,  QStringLiteral("成功"), QStringLiteral("设置普通算法密钥成功。"),QMessageBox::Close);
    }
    else
    {
       QMessageBox::warning(NULL, QStringLiteral("错误"),QStringLiteral("设置普通算法密钥错误。"),QMessageBox::Close);
    }
}
