#pragma  once
#include"INetMediator.h"

class UdpMediator : public INetMediator
{
public:
	UdpMediator();
	 ~UdpMediator();//使用时，父类指针指向子类，使用虚析构函数
	//初始化网络
	bool  OpenNet();
	//关闭网络
	void  CloseNet();
	//发送数据
    bool  SendData(long lSendIp,char *buf,int nLen);
protected:
    void  DealData(long lSendIp,char *buf,int nLen);
};
