#include "useritem.h"
#include "ui_useritem.h"
#include<QIcon>
#include<QBitmap>

UserItem::UserItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UserItem)
{
    ui->setupUi(this);
}

UserItem::~UserItem()
{
    delete ui;
}

//设置用户属性
void UserItem::slot_setInfo(int id, QString name, int state, int icon, QString feeling)
{
    //1.保存属性
    m_id = id;
    m_name = name;
    m_status = state;
    m_iconId = icon;
    m_feeling = feeling;

    //2.设置昵称和签名
    ui->lb_name->setText(m_name);
    ui->lb_feeling->setText(m_feeling);
    //3设置头像，头像路径
    QString pathIcon = QString(":/tx/%1.png").arg(m_iconId);
    //4.判断用户状态
    if(m_status)
    {
        //5.用户在线，显示为彩色的头像
        ui->pb_icon->setIcon(QIcon(pathIcon));
    }
    else
    {
        //6.用户不在线，显示为灰色的头像
        //QBitmap以二进制位图的形式加载的图片是黑白的
        QBitmap bmp;
        bmp.load(pathIcon);
        ui->pb_icon->setIcon(bmp);
    }
    //7.重绘
    this->repaint();

}

void UserItem::mouseDoubleClickEvent(QMouseEvent *event)
{
    Q_EMIT SIG_UserItemClicked(m_id);
}

//设置用户下线
void UserItem::slot_setOffline()
{
    //1.设置用户状态
    m_status = 0;
    //2.绘制灰色头像
    QString pathIcon = QString(":/tx/%1.png").arg(m_iconId);
    QBitmap bmp;
    bmp.load(pathIcon);
    ui->pb_icon->setIcon(bmp);
    //3.重绘
    this->repaint();
}


void UserItem::on_pb_icon_clicked()
{
    //发送用户点击头像的信号
    Q_EMIT SIG_UserItemClicked(m_id);
}
