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
	//初始化网络  
	bool  InitNet();
	//关闭网络
	void  UnInitNet();
	//发送数据
	bool  SendData(long lSendIp,char  *buf,int  nLen);
protected:
	static unsigned int _stdcall AcceptThread(void* lpvoid);
	static unsigned int _stdcall RecvThread(void* lpvoid);
	void  RecvData();
	SOCKET m_sock;//主套接字，用来监听的，每个客户端收发数据的套接字要单独创建，后面需要再加。
	bool m_bIsStop;
	list<HANDLE> m_hThreadHandleList;
	map<unsigned int,SOCKET> m_mapThreadIdToSocket;

};