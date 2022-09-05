#include"TcpServerMediator.h"
#include"TcpServerNet.h"
#include"./../Kernel.h"
#include<iostream>
using  namespace std;

TcpServerMediator::TcpServerMediator()
{
	m_pNet=new TcpServerNet(this);
}
TcpServerMediator::~TcpServerMediator()//ʹ��ʱ������ָ��ָ�����࣬ʹ������������
{
    if(m_pNet)
    {
        delete m_pNet;
        m_pNet=NULL;
    }
}
//��ʼ������
bool  TcpServerMediator::OpenNet()
{
	if(!m_pNet->InitNet())
	{
		return false;
	}
	return  true;
}
//�ر�����
void  TcpServerMediator::CloseNet()
{
	m_pNet->UnInitNet();
}
//��������
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
	cout<< "TcpServerMediator::DealData" <<endl;
	//TODO:ͨ���ź���QT���͸�������
    cout<< lSendIp <<":" << buf <<endl;
	CKernel::p_kernel->DealData(lSendIp,buf,nLen);
}
