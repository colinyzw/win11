#include "ckernel.h"
#include<QMessageBox>
#include<QDebug>
#include<QTextCodec>
#include"useritem.h"
#include<QInputDialog>

//设置协议映射关系
#define NetPackFunMap(a) m_netProtocolMap[a - _DEF_PROTOCAL_BASE - 10]

//初始化协议映射表
void CKernel::setProtocolMap()
{
    //1.初始化数组
    memset(m_netProtocolMap,0,sizeof(m_netProtocolMap));
    //2.设置数组
    NetPackFunMap(_DEF_PACK_REGISTER_RS) = &slot_register_RS;
    NetPackFunMap(_DEF_PACK_LOGIN_RS)= &slot_login_RS;
    NetPackFunMap(_DEF_PACK_FRIEND_INFO)= &slot_friendInfoRq;
    NetPackFunMap(_DEF_PACK_CHAT_RQ)= &slot_chatRq;
    NetPackFunMap(_DEF_PACK_CHAT_RS)= &slot_chatRs;
    NetPackFunMap(_DEF_PACK_ADDFRIEND_RQ)= &slot_addFriendRq;
    NetPackFunMap(_DEF_PACK_ADDFRIEND_RS)= &slot_addFriendRs;
    NetPackFunMap(_DEF_PACK_OFFINE_RQ)= &slot_offlineRq;

}

//QString -> char* GB2312
void CKernel::Utf8ToGB2312(QString &utf8, char *gbbuf, int nLen)
{
    QTextCodec* gb2312 = QTextCodec::codecForName("gb2312");
    QByteArray ba = gb2312->fromUnicode(utf8);
    strcpy_s(gbbuf,nLen,ba.data());
}

//char* GB2312 -> QString
QString CKernel::GB2312ToUtf8(char *gbbuf)
{
    QTextCodec* gb2312 = QTextCodec::codecForName("gb2312");
    return gb2312->toUnicode(gbbuf);
}

CKernel::CKernel(QObject *parent) : QObject(parent)
{
    //初始化登录&注册窗口并显示
    m_loginDialog = new LoginDialog;
    m_loginDialog->showNormal();

    //绑定登录信息提交的信号和槽函数
    connect(m_loginDialog,SIGNAL(SIG_LoginCommit(QString,QString)),
            this,SLOT(slot_loginCommit(QString,QString)));
    //绑定注册信息提交的信号和槽函数
    connect(m_loginDialog,SIGNAL(SIG_LoginCommitRegister(QString,QString,QString)),
            this,SLOT(slot_loginCommitRegister(QString,QString,QString)));
    //绑定关闭登录窗口的信号和槽函数
    connect(m_loginDialog,SIGNAL(SIG_Close()),
            this,SLOT(slot_destroyInstance()));

    //初始化主对话框并显示
    m_mainWnd = new MyChatDialog;
    //m_mainWnd->show();
    //绑定添加好友的信号和槽函数
    connect(m_mainWnd,SIGNAL(SIG_addFriend()),
            this,SLOT(slot_addFriend()));
    //绑定关闭好友列表窗口的信号和槽函数
    connect(m_mainWnd,SIGNAL(SIG_Close()),
            this,SLOT(slot_offline()));


    //初始化中介者类
    m_tcpClient = new TcpClientMediator;

    //绑定接收数据的信号和槽函数
    connect(m_tcpClient,SIGNAL(SIG_ReadyData(long,char*,int)),
             this,SLOT(slot_dealData(long,char*,int)));

    //打开网络
    if(!m_tcpClient->OpenNet())
    {
        QMessageBox::about(m_mainWnd,"提示","打开网络失败");
        exit(0);
    }

    //初始化协议映射表
    setProtocolMap();

}


CKernel::~CKernel()
{
    //回收登录&注册对话窗口
    if(m_loginDialog)
    {
        m_loginDialog->hide();
        delete m_loginDialog;
        m_loginDialog = NULL;
    }
    //回收主窗口对象
    if(m_mainWnd)
    {
        m_mainWnd->hide();
        delete m_mainWnd;
        m_mainWnd = 0;
    }

    //回收中介者类对象
    if(m_tcpClient)
    {
        m_tcpClient->CloseNet();
        delete m_tcpClient;
        m_tcpClient = 0;
    }

    //回收map中的空间,父控件负责回收子控件不用这个步骤
//    for(auto ite = m_mapIdToUserItem.begin();ite != m_mapIdToUserItem.end();)
//    {
//        UserItem* item = *ite;
//        item->close();
//        ite = m_mapIdToUserItem.erase(ite);
//        delete item;
//        item = NULL;
//    }
    for(auto ite = m_mapIdToChatDialog.begin();ite != m_mapIdToChatDialog.end();)
    {
        chatDialog* chat = *ite;
        chat->hide();
        ite = m_mapIdToChatDialog.erase(ite);
        delete chat;
        chat = NULL;
    }

}
//登录信息处理的槽函数
void CKernel::slot_loginCommit(QString tel, QString password)
{
    //1.QString转换成std::string
    //QString通过toStdString转换成std::string
    std::string strTel = tel.toStdString();
    std::string strPassword = password.toStdString();
    //std::string通过c_str()转换成char[];
    //2.封包//strcpy遇到'/0'-0结束，与memcpy拷贝任何东西不同
    STRU_LOGIN_RQ rq;
    strcpy(rq.tel,strTel.c_str());
    strcpy(rq.password,strPassword.c_str());

    //3.发送给服务器
    m_tcpClient->SendData(0,(char*)&rq,sizeof(rq));
}
//注册信息处理的槽函数
void CKernel::slot_loginCommitRegister(QString tel, QString password, QString name)
{
    //1.QString转换成std::string
    //QString通过toStdString转换成std::string
    std::string strTel = tel.toStdString();
    std::string strPassword = password.toStdString();
    //std::string strName = name.toStdString();
    //std::string通过c_str()转换成char[];
    //2.封包
    STRU_REGISTER_RQ rq;
    strcpy(rq.tel,strTel.c_str());
    strcpy(rq.password,strPassword.c_str());
    //strcpy(rq.name,strName.c_str());
    Utf8ToGB2312(name,rq.name,sizeof(rq.name));
    //3.发送给服务器
    m_tcpClient->SendData(0,(char*)&rq,sizeof(rq));
}

//处理发送聊天信息的槽函数
void CKernel::slot_sendChatMsg(int id, QString content)
{
    qDebug()<< __func__ << id << content;
    //1.封包
    STRU_CHAT_RQ rq;
    rq.userId = m_id;
    rq.friendId = id;
    strcpy(rq.content,content.toStdString().c_str());
    //2.发送聊天内容
    m_tcpClient->SendData(0,(char*)&rq,sizeof(rq));
}


//处理点击好友头像的槽函数
void CKernel::slot_UserItemClicked(int id)
{
    qDebug()<<__func__;
    if(m_mapIdToChatDialog.find(id) != m_mapIdToChatDialog.end())
    {
        chatDialog* chat = m_mapIdToChatDialog[id];
        chat->showNormal();
    }
}

//处理添加好友的槽函数
void CKernel::slot_addFriend()
{
    qDebug()<<__func__;
    //1.弹窗，获取用户输入的昵称
    QString strName = QInputDialog::getText(m_mainWnd,"添加好友","输入好友名字");
    //2.校验昵称不超过20位
    if(strName.length() > 20  || strName.length() <= 0)
    {
        QMessageBox::about(m_mainWnd,"提示","输入名字过长");
        return;
    }
    //3.判断是不是自己
    if(strName == m_name)
    {
        QMessageBox::about(m_mainWnd,"提示","不能添加自己为好友");
        return;
    }
    //4.判断是不是已经是不是好友了
    for(auto ite = m_mapIdToUserItem.begin();ite != m_mapIdToUserItem.end();ite++)
    {
        UserItem* item = *ite;
        if(item->m_name == strName)
        {
            QMessageBox::about(m_mainWnd,"提示","已经是好友了，不需要添加");
            return;
        }
    }
    //5.发送数据包给服务器
    STRU_ADD_FRIEND_RQ rq;
    rq.userId = m_id;
    strcpy(rq.userName,m_name.toStdString().c_str());
    Utf8ToGB2312(strName,rq.friendName,sizeof(rq.friendName));
    m_tcpClient->SendData(0,(char*)&rq,sizeof(rq));
}

//处理聊天回复请求
void CKernel::slot_chatRs(char *buf, int nLen)
{
    //1.拆包
    STRU_CHAT_RS* rs = (STRU_CHAT_RS*)buf;
    //2.查看与该用户的聊天窗口是否存在
    if(m_mapIdToChatDialog.find(rs->friendId) !=m_mapIdToChatDialog.end())
    {
        //3.如果存在，通过用户id获取到聊天窗口
        chatDialog* chat = m_mapIdToChatDialog[rs->friendId];
        //4.显示该好友不在线
        chat->slot_setUserOffline();
    }
}

//处理聊天请求
void CKernel::slot_chatRq(char *buf, int nLen)
{
    //1.拆包
    STRU_CHAT_RQ* rq = (STRU_CHAT_RQ*)buf;
    //2.查看与该用户的聊天窗口是否存在
    if(m_mapIdToChatDialog.find(rq->userId) !=m_mapIdToChatDialog.end())
    {
        //3.如果存在，通过用户id获取到聊天窗口
        chatDialog* chat = m_mapIdToChatDialog[rq->userId];
        chat->showNormal();
        //4.设置聊天信息
        chat->slot_SetChatMsg(rq->content);
    }
}


void CKernel::slot_dealData(long lSendIp,char* buf,int nLen)
{
    qDebug()<<__func__;
    //1.拆包
    int nType = *(int*)buf;
    //2.根据协议头处理不同的数据
    //2.1判断协议头是不是在数组下标范围内
    if(nType >= (_DEF_PROTOCAL_BASE + 10)
            && nType < (_DEF_PROTOCAL_BASE + 10 + _DEF_PROTOCOL_COUNT))
    {
        //2.2计算出数组下标对应的函数
        FUN pf = NetPackFunMap(nType);
        //2.3执行函数
        if(pf)
        {
            cout<< "(this->*pf)(buf,nLen);" <<endl;
            (this->*pf)(buf,nLen);
        }
    }
    //3.回收接收函数创建的空间
    delete[] buf;

}

//处理注册回复
void CKernel::slot_register_RS(char* buf,int nLen)
{
    qDebug()<<__func__;
    //1.拆包
    STRU_REGISTER_RS* rs = (STRU_REGISTER_RS*)buf;
    //2.判断注册结果
    switch(rs->result)
    {
        case user_is_exist:
            QMessageBox::about(this->m_loginDialog,"提示","注册失败，该用户已经存在");
            break;
        case register_success:
            QMessageBox::about(this->m_loginDialog,"提示","注册成功");
            break;
        case register_fail:
            QMessageBox::about(this->m_loginDialog,"提示","注册失败，网络错误或者数据库异常");
            break;
        default:
            break;
    }
}

//处理登录回复
void CKernel::slot_login_RS(char* buf,int nLen)
{
    qDebug()<<__func__;
    //1.拆包
    STRU_LOGIN_RS* rs = (STRU_LOGIN_RS*)buf;
    //2.根据不同的登录结果提示
    switch(rs->result)
    {
        case user_not_exist:
            QMessageBox::about(this->m_loginDialog,"提示","登录失败，该用户不存在");
            break;
        case password_error:
            QMessageBox::about(this->m_loginDialog,"提示","登录失败，密码错误");
            break;
        case login_success:
            //QMessageBox::about(this->m_loginDialog,"提示","登陆成功");
            {
                //1.隐藏登录界面
                m_loginDialog->hide();
                //2.跳转到登陆后的界面
                m_mainWnd->showNormal();
                //3.保存登录用户id
                m_id = rs->userId;

            }
            break;
        default:
            break;
    }
}


//处理好友信息请求
void CKernel::slot_friendInfoRq(char* buf, int nLen)
{
    //1.拆包
    STRU_FRIEND_INFO* info = (STRU_FRIEND_INFO*)buf;

    //1.1、中文转码
    QString name = GB2312ToUtf8(info->name);
    QString feeling = GB2312ToUtf8(info->feeling);

    //2.判断是不是自己
    if(m_id == info->userId)
    {
        //2.1如果是自己，更新到界面上
        m_mainWnd->slot_setInfo(name,info->iconId,feeling);

        //2.2保存用户自己的信息
        m_name = name;

        //2.3返回
        return;
    }

    //3.不是自己是好友，先查看这个好友id有没有对应的useritem
    if(m_mapIdToUserItem.find(info->userId)==m_mapIdToUserItem.end())
    {
        //4.如果没有useritem，new一个
        UserItem* item = new UserItem;
        //4.1设置控件属性
        item->slot_setInfo(info->userId,name,info->status,info->iconId,feeling);
        //4.2、给控件绑定点击的信号和槽
        connect(item,SIGNAL(SIG_UserItemClicked(int)),this,SLOT(slot_UserItemClicked(int)));
        //4.3、创建聊天窗口，但是不显示
        chatDialog* chat = new chatDialog;
        chat->setInfo(info->userId,name);
        //4.4.给聊天窗口绑定发送数据的信号和槽函数
        connect(chat,SIGNAL(SIG_SendChatMsg(int,QString)),this,SLOT(slot_sendChatMsg(int,QString)));
        //4.5、添加到map中管理聊天窗口
        m_mapIdToChatDialog[info->userId] = chat;
        //4.6、把useritem添加到好友列表上
        m_mainWnd->slot_addFriend(item);
        //4.7添加到map中管理
        m_mapIdToUserItem[info->userId] = item;

    }
    else
    {
        //5.如果有这个useritem，先取出useritem，判断好友状态
        UserItem* item = m_mapIdToUserItem[info->userId];
        if(item && item->m_status == 0 && info->status == 1)
        {
            //5.1如果好友以前是下线状态，现在是上线状态，提示XX已上线
            qDebug()<<QString("用户【%1】上线了").arg(name);
        }
        //5.2更新useritem属性
        item->slot_setInfo(info->userId,name,info->status,info->iconId,feeling);
    }
}


//处理添加好友请求
void CKernel::slot_addFriendRq(char *buf, int nLen)
{
    qDebug()<<__func__;
    //1.拆包
    STRU_ADD_FRIEND_RQ* rq = (STRU_ADD_FRIEND_RQ*)buf;
    STRU_ADD_FRIEND_RS rs;

    //2.弹出窗口，提示用户选择
    QString str = QString("用户【%1】请求添加为好友，是否同意？").arg(rq->userName);
    if(QMessageBox::question(m_mainWnd,"添加好友",str) == QMessageBox::Yes)
    {
        //2.1同意添加好友
        rs.result = add_success;
    }
    else
    {
        //2.2不同意添加好友
        rs.result = user_refuse;
    }
    //3.返回请求结果
    rs.friendId = m_id;
    rs.userId = rq->userId;
    m_tcpClient->SendData(0,(char*)&rs,sizeof(rs));
}

//处理添加好友回复
void CKernel::slot_addFriendRs(char *buf, int nLen)
{
    qDebug()<<__func__;
    //1.拆包
    STRU_ADD_FRIEND_RS* rs = (STRU_ADD_FRIEND_RS*)buf;
    //2.获取好友昵称，转换成QString
    //3.弹出提示信息
    switch(rs->result)
    {
    case no_this_user:
        QMessageBox::about(m_mainWnd,"提示","用户不存在，添加好友失败");
        break;
    case user_refuse:
        QMessageBox::about(m_mainWnd,"提示","用户拒绝添加好友");
        break;
    case user_offline:
        QMessageBox::about(m_mainWnd,"提示","用户不在线，添加好友失败");
        break;
    case add_success:
        QMessageBox::about(m_mainWnd,"提示","添加用户成功");
        break;
    }
}


//处理关闭登录窗口的槽函数
void CKernel::slot_destroyInstance()
{
    qDebug()<<__func__;
    //回收登录&注册对话窗口
    if(m_loginDialog)
    {
        m_loginDialog->hide();
        delete m_loginDialog;
        m_loginDialog = NULL;
    }
    //回收主窗口对象
    if(m_mainWnd)
    {
        m_mainWnd->hide();
        delete m_mainWnd;
        m_mainWnd = 0;
    }

    //回收中介者类对象
    if(m_tcpClient)
    {
        m_tcpClient->CloseNet();
        delete m_tcpClient;
        m_tcpClient = 0;
    }

    //回收map中的空间,父控件负责回收子控件不用这个步骤
//    for(auto ite = m_mapIdToUserItem.begin();ite != m_mapIdToUserItem.end();)
//    {
//        UserItem* item = *ite;
//        item->close();
//        ite = m_mapIdToUserItem.erase(ite);
//        delete item;
//        item = NULL;
//    }
    for(auto ite = m_mapIdToChatDialog.begin();ite != m_mapIdToChatDialog.end();)
    {
        chatDialog* chat = *ite;
        chat->hide();
        ite = m_mapIdToChatDialog.erase(ite);
        delete chat;
        chat = NULL;
    }
    exit(0);
}

//处理关闭好友列表窗口的槽函数
void CKernel::slot_offline()
{
    //1.发送离线请求
    STRU_OFFLINE_RQ rq;
    rq.userId = m_id;
    m_tcpClient->SendData(0,(char*)&rq,sizeof(rq));
    //2.销毁对象
    slot_destroyInstance();
}

//处理好友下线请求
void CKernel::slot_offlineRq(char* buf, int nLen)
{
    //1.拆包
    STRU_OFFLINE_RQ* rq = (STRU_OFFLINE_RQ*)buf;
    //2.取出id，设置好友为离线状态
    if(m_mapIdToUserItem.count(rq->userId) > 0)
    {
        UserItem* item = m_mapIdToUserItem[rq->userId];
        item->slot_setOffline();
    }
}


















