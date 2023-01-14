#include "ckernel.h"
#include<QHostAddress>
CKernel::CKernel(QObject *parent) : QObject(parent)
{
    m_pMainWnd = new FeiQ;
    m_pMainWnd->show();
    connect(m_pMainWnd,SIGNAL(SIG_userClicked(QString)),this,SLOT(slot_ShowChatDlg(QString)));

    connect(m_pMainWnd,SIGNAL(SIG_close()),this,SLOT(slot_destory()));


    m_pUdp = new UdpMediator;
    if(!m_pUdp->OpenNet())
    {
        QMessageBox::about(m_pMainWnd,"提示","网络错误");
        exit(0);//直接结束程序
    }
    connect(m_pUdp,SIGNAL(SIG_ReadyData(long,char*,int)),this,SLOT(slot_ReadyData(long,char*,int)));
    //测试


    //上线通知
    STRU_ONLINE rq;
    gethostname(rq.szName,sizeof(rq.szName));
    m_pUdp->SendData(INADDR_BROADCAST,(char*)&rq,sizeof(rq));

}


CKernel::~CKernel()
{
    //删除聊天窗口
    for(auto ite = m_pmapIPToDialog.begin();ite !=m_pmapIPToDialog.end();)
    {
        chatDialog* chattemp = ite->second;
        chattemp->hide();
        delete chattemp;
        //删除节点之前，需要给迭代器赋新值
        ite = m_pmapIPToDialog.erase(ite);
    }


    if(m_pMainWnd)
    {
        m_pMainWnd->hide();
        delete m_pMainWnd;
        m_pMainWnd = NULL;
    }
    if(m_pUdp)
    {
        m_pUdp->CloseNet();
        delete m_pUdp;
        m_pUdp = NULL;
    }
}

void CKernel::slot_ReadyData(long lSendIp,char *buf,int nLen)
{

    //1.取协议头
    int nType = *(int*)buf;
    //2.判断是什么协议类型
    switch(nType)
    {
    case _DEF_PROTOCAL_ONLINE_RQ:
        slot_dealOnlineRq(lSendIp,buf,nLen);
        break;
    case _DEF_PROTOCAL_ONLINE_RS:
        slot_dealOnlineRs(lSendIp,buf,nLen);
        break;
    case _DEF_PROTOCAL_OFFLINE:
        slot_dealOffline(lSendIp,buf,nLen);
        break;
    case _DEF_PROTOCAL_DATEINFO_RQ:
        slot_dealChatRq(lSendIp,buf,nLen);
        break;
    case _DEF_PROTOCAL_DATEINFO_RS:
        slot_dealChatRs(lSendIp,buf,nLen);
        break;
    default:break;
    }
    delete[] buf;
    //QMessageBox::about(m_pMainWnd,INet::GetipString(lSendIp),buf)

}


//处理上线请求
void CKernel::slot_dealOnlineRq(long lSendIp,char *buf,int nLen)
{
    //1.拆包
    STRU_ONLINE* rq = (STRU_ONLINE*)buf;
    //2.添加好友到聊天界面
    m_pMainWnd->slot_addFriend(INet::GetipString(lSendIp).c_str(),QString::fromStdString(rq->szName));
    //判断窗口是否已经存在
    if(0 == m_pmapIPToDialog.count(lSendIp))
    {
        //3.创建聊天窗口
        chatDialog* chatdialog= new chatDialog;//new要进行delete  ->setInfo(INet::GetipString(lSendIp).c_str());
        chatdialog->setInfo(INet::GetipString(lSendIp).c_str());
        connect(chatdialog,SIGNAL(SIG_SendChatMsg(QString,QString)),this,SLOT(slot_Sendchatmsg(QString,QString)));
        //4.放在map图里面管理
        m_pmapIPToDialog[lSendIp] = chatdialog;
    }
    //5.判断是不是自己
    std::set<long> setIp = INet::GetValidIpList();
    if(setIp.count(lSendIp) > 0)
    {
        return;
    }
    //6.如果不是自己，再发送上线请求回复
    STRU_ONLINE rs;
    rs.nType = _DEF_PROTOCAL_ONLINE_RS;
    gethostname(rs.szName,sizeof(rs.szName));
    m_pUdp->SendData(lSendIp,(char*)&rs,sizeof(rs));
}
//处理上线请求回复
void CKernel::slot_dealOnlineRs(long lSendIp,char *buf,int nLen)
{
    //1.拆包
    STRU_ONLINE* rs = (STRU_ONLINE*)buf;
    //2.添加好友到聊天界面
    m_pMainWnd->slot_addFriend(INet::GetipString(lSendIp).c_str(),QString::fromStdString(rs->szName));
    //判断窗口是否已经存在
    if(0 == m_pmapIPToDialog.count(lSendIp))
    {
        //3.创建聊天窗口
        chatDialog* chatdialog= new chatDialog;//new要进行delete
        chatdialog->setInfo(INet::GetipString(lSendIp).c_str());
        connect(chatdialog,SIGNAL(SIG_SendChatMsg(QString,QString)),this,SLOT(slot_Sendchatmsg(QString,QString)));
        //4.放在map图里面管理
        m_pmapIPToDialog[lSendIp] = chatdialog;
    }

}
//处理下线请求
void CKernel::slot_dealOffline(long lSendIp,char *buf,int nLen)
{
    //1.从好友列表中删除用户
    m_pMainWnd->slot_deleteFriend(INet::GetipString(lSendIp).c_str());
    //2.删除聊天窗口
    auto ite = m_pmapIPToDialog.find(lSendIp);
    if(ite != m_pmapIPToDialog.end())
    {
        chatDialog* chat = ite->second;
        chat->hide();
        delete chat;
    }
}
//处理聊天请求
void CKernel::slot_dealChatRq(long lSendIp,char *buf,int nLen)
{
    //1.拆包
    STRU_DATAINFO_RQ* rq = (STRU_DATAINFO_RQ*)buf;
    //2.取出聊天内容
    QString content = QString::fromStdString(rq->szContent);
    if(m_pmapIPToDialog.count(lSendIp) > 0)
    {
        //3.显示窗口
        chatDialog* chat = m_pmapIPToDialog[lSendIp];
        chat->showNormal();
        //4.聊天内容显示到窗口
        chat->slot_SetChatMsg(content);
    }

}
//处理聊天回复
void CKernel::slot_dealChatRs(long lSendIp,char *buf,int nLen)
{


}

void CKernel::slot_Sendchatmsg(QString ip, QString content)
{
    STRU_DATAINFO_RQ rq;
    strcpy_s(rq.szContent,content.toStdString().c_str());
    m_pUdp->SendData(inet_addr(ip.toStdString().c_str()),(char*)&rq,sizeof(rq));
}

void CKernel::slot_ShowChatDlg(QString ip)
{
    long IP = inet_addr(ip.toStdString().c_str());
    //如果map中有这个窗口，取出窗口并显示
    if(m_pmapIPToDialog[IP] > 0)
    {
        chatDialog* chat = m_pmapIPToDialog[IP];
        chat->showNormal();
    }
}

void CKernel::slot_destory()
{
    //1.发送下线请求
    STRU_OFFLINE rq;
    m_pUdp->SendData(INADDR_BROADCAST,(char*)&rq,sizeof(rq));
    //2.回收聊天窗口
    for(auto ite = m_pmapIPToDialog.begin();ite !=m_pmapIPToDialog.end();)
    {
        chatDialog* chattemp = ite->second;
        chattemp->hide();
        delete chattemp;
        //删除节点之前，需要给迭代器赋新值
        ite = m_pmapIPToDialog.erase(ite);
    }

    //3.回收主窗口
    if(m_pMainWnd)
    {
        m_pMainWnd->hide();
        delete m_pMainWnd;
        m_pMainWnd = NULL;
    }
    //4.关闭网络
    if(m_pUdp)
    {
        m_pUdp->CloseNet();
        delete m_pUdp;
        m_pUdp = NULL;
    }
    exit(0);

}
