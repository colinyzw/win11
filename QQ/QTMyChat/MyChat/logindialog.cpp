#include "logindialog.h"
#include "ui_logindialog.h"
#include<QMessageBox>

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

//重写关闭窗口事件
void LoginDialog::closeEvent(QCloseEvent *event)
{
    //1.接受关闭事件
    event->accept();
    //2.发送信号通知kernel类
    Q_EMIT SIG_Close();
}

//登录页的清空按钮
void LoginDialog::on_pb_clear_clicked()
{
    //把对应设置为空
    ui->le_tel->setText("");
    ui->le_password->setText("");
}
//登录页的提交按钮
void LoginDialog::on_pb_commit_clicked()
{
    //1.取出控件中的内容
    QString tel = ui->le_tel->text();
    QString password = ui->le_password->text();
    //2.判断数据的合法性
    //2.1tel不能为空，长度为11，只能是纯数字（正则表达式还没学）
    //2.2password不为空，长度不能超过10
    QString telTemp = tel;
    if(tel.isEmpty() || password.isEmpty() || telTemp.remove(" ").isEmpty()/*去掉空格之后不能是空*/)
    {
        QMessageBox::about(this,"提示","手机号或密码不能为空");
        return;
    }
    if(tel.length()!=11 || password.length() > 10)
    {
        QMessageBox::about(this,"提示","手机号必须为11位，密码不能超过10位");
        return;
    }
    Q_EMIT SIG_LoginCommit(tel,password);
}
//注册页的清空按钮
void LoginDialog::on_pb_clear_register_clicked()
{
    ui->le_tel_register->setText("");
    ui->le_password_register->setText("");
    ui->le_name_register->setText("");
}
//注册页的提交按钮
void LoginDialog::on_pb_commit_register_clicked()
{
    //1.取出控件中的内容
    QString tel = ui->le_tel_register->text();
    QString password = ui->le_password_register->text();
    QString name = ui->le_name_register->text();
    //2.判断数据的合法性
    //2.1 tel不能为空，长度为11，只能是纯数字（正则表达式还没学）
    //2.2 password不为空，长度不能超过10
    //2.3 name不能为空，不能超过20
    QString telTemp = tel;
    QString nameTemp = name;
    if(tel.isEmpty() || password.isEmpty() || name.isEmpty() || telTemp.remove(" ").isEmpty()
            || nameTemp.remove(" ").isEmpty())
    {
        QMessageBox::about(this,"提示","手机号或密码或昵称不能为空");
        return;
    }
    if(tel.length()!=11 || password.length() > 10 || name.length()>20)
    {
        QMessageBox::about(this,"提示","手机号必须为11位，密码不能超过10位,昵称不能超过20位");
        return;
    }
    Q_EMIT SIG_LoginCommitRegister(tel,password,name);
}
