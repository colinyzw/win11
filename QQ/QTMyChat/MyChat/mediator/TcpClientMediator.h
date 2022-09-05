#pragma  once
#include"INetMediator.h"

class TcpClientMediator : public INetMediator
{
    Q_OBJECT
public:
	TcpClientMediator();
	 ~TcpClientMediator();//使用时，父类指针指向子类，使用虚析构函数

	bool  OpenNet();

	void  CloseNet();
	//发送数据
    bool  SendData(long lSendIp,char *buf,int nLen);
protected:
    void  DealData(long lSendIp,char *buf,int nLen);

signals:
    void SIG_ReadyData(long lSendIp,char *buf,int nLen);
};
