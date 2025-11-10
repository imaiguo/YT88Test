
#pragma once

#include <QDialog>
#include "SoftkeyYT88.h"

namespace Ui {
class Dialog;
}

class DialogFunction : public QDialog
{
    Q_OBJECT

public:
    explicit DialogFunction(QWidget *parent = 0);
    ~DialogFunction();

private slots:
    // 返回加密狗的ID号
    void on_GetIDButton_clicked();

    void on_GetVerButton_clicked();

    void on_FindPort_2Button_clicked();

    void on_WriteEx_Button_clicked();

    void on_sWrite_2Ex_Button_clicked();

    void on_WriteStr_Button_clicked();

    void on_WriteStr_Len_Button_clicked();

    void on_ReadStr_Len_Button_clicked();

    void on_WriteByte_Button_clicked();

    void on_ReadByte_Button_clicked();

    void on_SetWritePwd_Button_clicked();

    void on_SetReadPwd_Button_clicked();

    void on_SetCal_2_Button_clicked();

    void on_EncString_Button_clicked();

    void on_Cal_Button_clicked();

    void on_SetCal_New_Button_clicked();

    void on_EncString_New_Button_clicked();

    void on_Cal_New_Button_clicked();

    void on_Reset_Button_clicked();

    void on_SetUKey_Button_clicked();

    void on_ReadStr_Button_clicked();

    void on_GetProdeceDateButton_clicked();

    void on_SetIDButton_clicked();

    void on_pushButton_clicked();

private:
    Ui::Dialog *ui;
    char DevicePath[260];
    SoftkeyYT88 *ytsoftkey;
};
