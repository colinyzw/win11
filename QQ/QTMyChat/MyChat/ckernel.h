#ifndef CKERNEL_H
#define CKERNEL_H

#include <QObject>
#include"TcpClientMediator.h"
#include"mychatdialog.h"
#include<QDebug>
#include"Package.h"
#include"logindialog.h"
#include<iostream>
#include"useritem.h"
#include<QMap>
#include"chatdialog.h"
using namespace std;

class CKernel;
//定义函数指针
typedef void (CKernel::*FUN)(char* buf,int nLen);

class CKernel : public QObject
{
    Q_OBJECT
public:
    explicit CKernel(QObject *parent = 0);
    ~CKernel();

signals:

public slots:
    //登录信息处理的槽函数
    void slot_loginCommit(QString tel,QString password);
    //注册信息处理的槽函数
    void slot_loginCommitRegister(QString tel,QString password,QString name);
    //处理发送聊天信息的槽函数
    void slot_sendChatMsg(int id,QString content);
    //处理点击好友头像的槽函数
    void slot_UserItemClicked(int id);
    //处理添加好友的槽函数
    void slot_addFriend();

    //处理聊天回复请求
    void slot_chatRs(char* buf,int nLen);
    //处理聊天请求
    void slot_chatRq(char* buf,int nLen);

    //处理接收到的数据
    void slot_dealData(long lSendIp,char* buf,int nLen);
    //处理注册回复
    void slot_register_RS(char* buf,int nLen);
    //处理登录回复
    void slot_login_RS(char* buf,int nLen);
    //处理好友信息请求
    void slot_friendInfoRq(char* buf, int nLen);
    //处理添加好友请求
    void slot_addFriendRq(char* buf, int nLen);
    //处理添加好友回复
    void slot_addFriendRs(char* buf, int nLen);
    //处理关闭登录窗口的槽函数
    void slot_destroyInstance();
    //处理关闭好友列表窗口的槽函数
    void slot_offline();
    //处理好友下线请求
    void slot_offlineRq(char* buf, int nLen);
    //处理xxx回复请求
private:
    //初始化协议映射表
    void setProtocolMap();

    //QString -> char* GB2312
    void Utf8ToGB2312(QString &utf8,char* gbbuf,int nLen);
    //char* GB2312 -> QString
    QString GB2312ToUtf8(char* gbbuf);

private:
    TcpClientMediator* m_tcpClient;
    MyChatDialog* m_mainWnd;
    LoginDialog* m_loginDialog;
    int m_id;
    QString m_name;

    //协议映射表（函数指针数组）【协议头】-函数指针
    FUN m_netProtocolMap[_DEF_PROTOCOL_COUNT];
    //保存好友id和useritem的映射关系
    QMap<int,UserItem*> m_mapIdToUserItem;
    //保存好友id和聊天窗口的映射关系
    QMap<int,chatDialog*> m_mapIdToChatDialog;

};

#endif // CKERNEL_H
