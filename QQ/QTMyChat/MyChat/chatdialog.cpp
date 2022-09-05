#include "chatdialog.h"
#include "ui_chatdialog.h"
#include<QTime>
chatDialog::chatDialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::chatDialog)
{
    ui->setupUi(this);
}

chatDialog::~chatDialog()
{
    delete ui;
}

void chatDialog::setInfo(int id, QString name)
{
    m_id = id;
    m_name = name;
    //聊天窗口的标题，与【XXXx】的聊天
    setWindowTitle(QString("与【%1】的聊天").arg(m_name));
}

void chatDialog::slot_setUserOffline()
{
    //【XXX】时间 用户不在线
    ui->tb_chat->append(QString("[%1] %2 用户不在线").arg(m_name).arg(QTime::currentTime().toString("hh::mm::ss")));
}

void chatDialog::on_pb_send_clicked()
{
    //1.把用户输入的数据取出来
    QString content = ui->te_chat->toPlainText();
    if(content.isEmpty())
    {
        return;
    }
    //2.清空编辑窗口
    ui->te_chat->clear();
    //3.添加数据到浏览窗口
    ui->tb_chat->append(QString("[我] %1").arg(QTime::currentTime().toString("hh::mm::ss")));
    ui->tb_chat->append(content);
    //4.把数据发送出去
    Q_EMIT SIG_SendChatMsg(m_id,content);
}

void chatDialog::slot_SetChatMsg(const QString content)
{
    //[XXX]时间 内容
    ui->tb_chat->append(QString("[%1] %2").arg(m_name).arg(QTime::currentTime().toString("hh::mm::ss")));
    ui->tb_chat->append(content);
}
