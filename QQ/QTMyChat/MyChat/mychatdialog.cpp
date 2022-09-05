#include "mychatdialog.h"
#include "ui_mychatdialog.h"


MyChatDialog::MyChatDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MyChatDialog)
{
    ui->setupUi(this);
    setWindowTitle("通信软件v1.0");
    //初始化QVBoxLayout
    m_layout = new QVBoxLayout;
    //设置与外边框的距离
    m_layout->setContentsMargins(0,0,0,0);
    //设置每个控件间的距离
    m_layout->setSpacing(3);
    //设置控件的层
    ui->wdg_list->setLayout(m_layout);


    //创建菜单对象，指定父控件，父控件负责子控件的回收
    m_menu = new QMenu(this);
    //添加菜单项
    m_menu->addAction("添加好友");
    m_menu->addAction("系统设置");
    //绑定菜单项点击事件的和处理函数
    connect(m_menu,SIGNAL(triggered(QAction*)),
            this,SLOT(slot_dealMenu(QAction*)));


}

MyChatDialog::~MyChatDialog()
{
    delete ui;
}

//添加好友
void MyChatDialog::slot_addFriend(UserItem* userItem)
{
    //往层上添加好友
    m_layout->addWidget(userItem);

}

//设置UI控件属性
void MyChatDialog::slot_setInfo(QString name, int iconId, QString feeling)
{
    ui->lb_name->setText(name);
    ui->le_feeling->setText(feeling);
    ui->pb_icon->setIcon(QIcon(QString(":/tx/%1.png").arg(iconId)));
}

//重写关闭窗口事件
void MyChatDialog::closeEvent(QCloseEvent *event)
{
    //1.忽略关闭事件
    event->ignore();
    //2.弹出询问窗口
    if(QMessageBox::question(this,"退出提示","是否要退出？") == QMessageBox::Yes)
    {
        //发送关闭信号
        Q_EMIT SIG_Close();
    }
}

//菜单项点击事件的处理函数
void MyChatDialog::slot_dealMenu(QAction* action)
{
    qDebug()<<__func__;
    //判断用户点击的是哪一个菜单项
    if("添加好友" == action->text())
    {
        //发送添加好友的信号
        qDebug()<<__func__;
        Q_EMIT SIG_addFriend();
    }
    else if("系统设置" == action->text())
    {
        //处理系统设置
    }
}

//点击菜单处理函数
void MyChatDialog::on_pb_menu_clicked()
{
    //显示菜单
    //按照鼠标位置显示菜单
    QPoint p = QCursor::pos();
    //获取菜单的绝对高度，就是所有的菜单先都添加完以后的高度
    QSize size = m_menu->sizeHint();

    m_menu->exec(QPoint(p.x(),p.y() - size.height()));

}
