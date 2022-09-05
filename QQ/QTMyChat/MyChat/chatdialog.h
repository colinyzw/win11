#ifndef CHATDIALOG_H
#define CHATDIALOG_H

#include <QWidget>
#include<QString>
namespace Ui {
class chatDialog;
}

class chatDialog : public QWidget
{
    Q_OBJECT

public:
    explicit chatDialog(QWidget *parent = 0);
    ~chatDialog();
    //设置窗口信息
    void setInfo(int id,QString name);
    //设置聊天对象不在线
    void slot_setUserOffline();

private slots:
    //发送按钮处理槽函数
    void on_pb_send_clicked();

public slots:
    void slot_SetChatMsg(QString content);

signals:
    void SIG_SendChatMsg(int id,QString content);

private:
    Ui::chatDialog *ui;
    int m_id;
    QString m_name;
};

#endif // CHATDIALOG_H
