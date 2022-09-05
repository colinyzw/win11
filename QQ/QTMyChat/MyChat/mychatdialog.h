#ifndef MYCHATDIALOG_H
#define MYCHATDIALOG_H

#include <QDialog>
#include<QVBoxLayout>
#include"useritem.h"
#include<QMenu>
#include<qDebug>
#include<QCloseEvent>
#include<QAction>
#include<QMessageBox>

namespace Ui {
class MyChatDialog;
}

class MyChatDialog : public QDialog
{
    Q_OBJECT

signals:
    //添加好友信号
    void SIG_addFriend();
    //关闭好友列表窗口信号
    void SIG_Close();

public:
    explicit MyChatDialog(QWidget *parent = 0);
    ~MyChatDialog();
    //添加好友
    void slot_addFriend(UserItem* userItem);
    //设置UI控件属性
    void slot_setInfo(QString name,int iconId,QString feeling);
    //重写关闭窗口事件
    void closeEvent(QCloseEvent* event);



private slots:
    void on_pb_menu_clicked();
    //菜单项点击事件的处理函数
    void slot_dealMenu(QAction* action);

private:
    Ui::MyChatDialog *ui;
    //层
    QVBoxLayout* m_layout;
    //菜单
    QMenu* m_menu;
};

#endif // MYCHATDIALOG_H
