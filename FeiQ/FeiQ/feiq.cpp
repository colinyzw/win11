#include "feiq.h"
#include "ui_feiq.h"
#include<QMessageBox>
FeiQ::FeiQ(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FeiQ)
{
    ui->setupUi(this);
}

FeiQ::~FeiQ()
{
    delete ui;
}
#include<QDebug>
void FeiQ::on_tw_friend_cellDoubleClicked(int row, int column)
{
    qDebug()<<QString("(%1, %2)").arg(row).arg(column);
    //取出IP地址
    QString ip = ui->tw_friend->item(row,0)->text();

    //发送IP地址
    Q_EMIT SIG_userClicked(ip);
}

void FeiQ::closeEvent(QCloseEvent* event)
{
    //弹出提示窗口，确认是否关闭
    if(QMessageBox::question(this,"提示","确认是否关闭") == QMessageBox::Yes)
    {
        Q_EMIT SIG_close();
    }
    event->ignore();
}
void FeiQ::slot_addFriend(QString ip, QString host)
{
    //1.获取总行数，添加新的一行
    ui->tw_friend->setRowCount(ui->tw_friend->rowCount() + 1);
    //2.在最后一行加入数据
    QTableWidgetItem* item1 = new QTableWidgetItem(ip);
    ui->tw_friend->setItem(ui->tw_friend->rowCount()-1,0,item1);

    QTableWidgetItem* item2 = new QTableWidgetItem(host);
    ui->tw_friend->setItem(ui->tw_friend->rowCount()-1,1,item2);

}

void FeiQ::slot_deleteFriend(QString ip)
{
    for(int i = 0;i < ui->tw_friend->rowCount();)
    {
        if(ip == ui->tw_friend->item(i,0)->text())
        {
            ui->tw_friend->removeRow(i);
        }
        else
        {
            i++;
        }
    }
    //刷新控件
    this->update();

}
