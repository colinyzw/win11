#include"TcpClientMediator.h"
#include"TcpClientNet.h"
#include<iostream>
using  namespace std;

TcpClientMediator::TcpClientMediator()
{
	m_pNet=new TcpClientNet(this);
}
TcpClientMediator::~TcpClientMediator()
{
    if(m_pNet)
    {
        delete m_pNet;
        m_pNet=NULL;
    }
}
//初始化网络
bool  TcpClientMediator::OpenNet()
{
	if(!m_pNet->InitNet())
	{
		return false;
	}
	return  true;
}
//关闭网络
void  TcpClientMediator::CloseNet()
{
	m_pNet->UnInitNet();
}
//发送数据
bool  TcpClientMediator::SendData(long lSendIp,char  *buf,int  nLen)
{
	if(!m_pNet->SendData(lSendIp,buf,nLen))
	{
		return false;
    }
	return  true;
}

void  TcpClientMediator::DealData(long lSendIp,char *buf,int nLen)
{
	//TODO:通过信号在QT发送给处理类
    cout<< lSendIp <<":" << buf <<endl;
    Q_EMIT SIG_ReadyData(lSendIp,buf,nLen);
}
