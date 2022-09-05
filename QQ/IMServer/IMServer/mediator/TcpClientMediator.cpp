#include"TcpClientMediator.h"
#include"TcpClientNet.h"
#include<iostream>
using  namespace std;

TcpClientMediator::TcpClientMediator()
{
	m_pNet=new TcpClientNet(this);
}
TcpClientMediator::~TcpClientMediator()//ʹ��ʱ������ָ��ָ�����࣬ʹ������������
{
    if(m_pNet)
    {
        delete m_pNet;
        m_pNet=NULL;
    }
}
//��ʼ������
bool  TcpClientMediator::OpenNet()
{
	if(!m_pNet->InitNet())
	{
		return false;
	}
	return  true;
}
//�ر�����
void  TcpClientMediator::CloseNet()
{
	m_pNet->UnInitNet();
}
//��������
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
	//TODO:ͨ���ź���QT���͸�������
    cout<< lSendIp <<":" << buf <<endl;
}
