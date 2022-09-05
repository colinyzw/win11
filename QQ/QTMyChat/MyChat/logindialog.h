#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include<QString>
#include<QCloseEvent>

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = 0);
    ~LoginDialog();
    //重写关闭窗口事件
    void closeEvent(QCloseEvent* event);

signals:
    //登录信息信号
    void SIG_LoginCommit(QString tel,QString password);
    //注册信息信号
    void SIG_LoginCommitRegister(QString tel,QString password,QString name);
    //关闭登录窗口信号
    void SIG_Close();

private slots:
    //清空提交按钮处理槽函数
    void on_pb_clear_clicked();

    void on_pb_commit_clicked();

    void on_pb_clear_register_clicked();

    void on_pb_commit_register_clicked();

private:
    Ui::LoginDialog *ui;
};

#endif // LOGINDIALOG_H
