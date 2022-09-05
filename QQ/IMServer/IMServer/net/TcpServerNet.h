#pragma  once
#include"INet.h"
#include<list>
#include<map>
#include<iostream>
using namespace std;

class TcpServerNet  :  public  INet
{
public:
	TcpServerNet(INetMediator* pMediator);
	~TcpServerNet();
	//��ʼ������  
	bool  InitNet();
	//�ر�����
	void  UnInitNet();
	//��������
	bool  SendData(long lSendIp,char  *buf,int  nLen);
protected:
	static unsigned int _stdcall AcceptThread(void* lpvoid);
	static unsigned int _stdcall RecvThread(void* lpvoid);
	void  RecvData();
	SOCKET m_sock;//���׽��֣����������ģ�ÿ���ͻ����շ����ݵ��׽���Ҫ����������������Ҫ�ټӡ�
	bool m_bIsStop;
	list<HANDLE> m_hThreadHandleList;
	map<unsigned int,SOCKET> m_mapThreadIdToSocket;

};