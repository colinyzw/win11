#ifndef USERITEM_H
#define USERITEM_H

#include <QWidget>

namespace Ui {
class UserItem;
}

class UserItem : public QWidget
{
    Q_OBJECT

signals:
    //点击好友头像，弹出聊天窗口
    void SIG_UserItemClicked(int id);

public:
    explicit UserItem(QWidget *parent = 0);
    ~UserItem();

    //设置用户属性
    void slot_setInfo(int id,QString name,int state,int icon,QString feeling);
    //鼠标双击用户，弹出聊天窗口
    void mouseDoubleClickEvent(QMouseEvent* event);
    //设置用户下线
    void slot_setOffline();

public:
    int m_status;//用以保存用户状态
    QString m_name;

private slots:
    void on_pb_icon_clicked();

private:
    Ui::UserItem *ui;
    int m_id;
    int m_iconId;
    QString m_feeling;
};

#endif // USERITEM_H
