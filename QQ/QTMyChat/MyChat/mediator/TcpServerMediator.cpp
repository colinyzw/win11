#include"TcpServerMediator.h"
#include"TcpServerNet.h"
#include<iostream>
using  namespace std;

TcpServerMediator::TcpServerMediator()
{
	m_pNet=new TcpServerNet(this);
}
TcpServerMediator::~TcpServerMediator()//使用时，父类指针指向子类，使用虚析构函数
{
    if(m_pNet)
    {
        delete m_pNet;
        m_pNet=NULL;
    }
}
//初始化网络
bool  TcpServerMediator::OpenNet()
{
	if(!m_pNet->InitNet())
	{
		return false;
	}
	return  true;
}
//关闭网络
void  TcpServerMediator::CloseNet()
{
	m_pNet->UnInitNet();
}
//发送数据
bool  TcpServerMediator::SendData(long lSendIp,char  *buf,int  nLen)
{
	if(!m_pNet->SendData(lSendIp,buf,nLen))
	{
		return false;
    }
	return  true;
}

void  TcpServerMediator::DealData(long lSendIp,char* buf,int nLen)
{
	//TODO:通过信号在QT发送给处理类
    cout<< lSendIp <<":" << buf <<endl;
	SendData(lSendIp,buf,nLen);
}
