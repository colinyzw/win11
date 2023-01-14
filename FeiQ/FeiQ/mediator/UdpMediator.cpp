#include"UdpMediator.h"
#include"UdpNet.h"
#include"iostream"
using  namespace std;

UdpMediator::UdpMediator()
{
	m_pNet=new UdpNet(this);
}
UdpMediator::~UdpMediator()//使用时，父类指针指向子类，使用虚析构函数
{
    if(m_pNet)
    {
        delete m_pNet;
        m_pNet=NULL;
    }
}
//初始化网络
bool  UdpMediator::OpenNet()
{
	if(!m_pNet->InitNet())
	{
		return false;
	}
	return  true;
}
//关闭网络
void  UdpMediator::CloseNet()
{
	m_pNet->UnInitNet();
}
//发送数据
bool  UdpMediator::SendData(long lSendIp,char  *buf,int  nLen)
{
	if(!m_pNet->SendData(lSendIp,buf,nLen))
	{
		return false;
    }
	return  true;
}

void  UdpMediator::DealData(long lSendIp,char *buf,int nLen)
{
	//TODO:通过信号在QT发送给处理类
    //cout<<buf<<endl;
    Q_EMIT SIG_ReadyData(lSendIp,buf,nLen);
}
