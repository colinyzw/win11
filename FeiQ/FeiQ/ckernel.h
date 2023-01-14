#ifndef CKERNEL_H
#define CKERNEL_H
#include"feiq.h"
#include <QObject>
#include"udpMediator.h"
#include<QMessageBox>
#include<winsock2.h>
#include<qdebug.h>
#include"Package.h"
#include<INet.h>
#include<chatdialog.h>
#include<QString>


class CKernel : public QObject
{
    Q_OBJECT
public:
    explicit CKernel(QObject *parent = 0);
    ~CKernel();


signals:

public slots:
    void slot_ReadyData(long lSendIp,char *buf,int nLen);
    void slot_dealOnlineRq(long lSendIp,char *buf,int nLen);
    void slot_dealOnlineRs(long lSendIp,char *buf,int nLen);
    void slot_dealOffline(long lSendIp,char *buf,int nLen);
    void slot_dealChatRq(long lSendIp,char *buf,int nLen);
    void slot_dealChatRs(long lSendIp,char *buf,int nLen);

    //处理发送聊天数据
    void slot_Sendchatmsg(QString ip,QString content);
    //双击好友时槽函数
    void slot_ShowChatDlg(QString ip);
    //关闭窗口
    void slot_destory();
private:
    FeiQ*  m_pMainWnd;
    UdpMediator* m_pUdp;
    std::map<long,chatDialog*> m_pmapIPToDialog;
};

#endif // CKERNEL_H
