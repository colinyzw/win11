#pragma  once
#include"INet.h"


class TcpClientNet  :  public  INet
{
public:
	TcpClientNet(INetMediator* pMediator);
	~TcpClientNet();
	//��ʼ������  
	bool  InitNet();
	//�ر�����
	void  UnInitNet();
	//��������
	bool  SendData(long lSendIp,char  *buf,int  nLen);
protected:
	void  RecvData();
	static unsigned int _stdcall recvThread(void* lpvoid);
	SOCKET m_sock;
	HANDLE m_hthreadHandle;
	bool m_bIsStop;

};