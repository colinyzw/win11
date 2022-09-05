#pragma  once
#include"INet.h"


class TcpClientNet  :  public  INet
{
public:
	TcpClientNet(INetMediator* pMediator);
	~TcpClientNet();
	//初始化网络  
	bool  InitNet();
	//关闭网络
	void  UnInitNet();
	//发送数据
	bool  SendData(long lSendIp,char  *buf,int  nLen);
protected:
	void  RecvData();
	static unsigned int _stdcall recvThread(void* lpvoid);
	SOCKET m_sock;
	HANDLE m_hthreadHandle;
	bool m_bIsStop;

};