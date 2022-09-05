#pragma  once
#include"TcpClientNet.h"


TcpClientNet::TcpClientNet(INetMediator* pMediator):m_sock(0),m_hthreadHandle(0),m_bIsStop(false)
{
	m_pMediator = pMediator;
}
TcpClientNet::~TcpClientNet()
{
}
//初始化网络
bool  TcpClientNet::InitNet()
{
	//1.选个项目-加载库   wsa-windows socket api
	WORD wVersionRequested={0};
	WSADATA wsaData={0};
    int result=0;

    wVersionRequested = MAKEWORD(2,2);

    result = WSAStartup(wVersionRequested, &wsaData);
    if (result != 0) {
        return false;
    }


    if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2) {

		UnInitNet();//关闭网络
        return false;
    }
    
	//2.(雇个人)创建套接字
	m_sock = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    if (m_sock == INVALID_SOCKET) 
	{

		UnInitNet();//关闭网络
        return false;
	}

	
	//3.发起连接
	sockaddr_in server;
	server.sin_family = AF_INET;//IPV4协议族
	server.sin_addr.S_un.S_addr=inet_addr(_DEF_SERVER_IP);//客户端，连接服务器，使用服务器的IP地址
	server.sin_port = htons(_DEF_TCP_PORT);//htons将证书转为网络字节序
	result = connect(m_sock,(SOCKADDR*)&server,sizeof(server));
	if (result == SOCKET_ERROR) {
		cout<<"connect失败"<<endl;
		UnInitNet();//关闭网络
        return 1;
	}
	
    //接收数据-创建线程     C/C++ RunTime 库  strcpy  创建内存块  CreateThread创建的线程不会回收内存块，造成内存泄露
	//ExitThread退出线程   _beginthreadex底层也是调用CreateThread，退出时调用ExitThread，并且回收创建的内存块
	m_hthreadHandle =(HANDLE)_beginthreadex(NULL,0,&recvThread,this,0,NULL);



	return true;
}
unsigned int _stdcall TcpClientNet::recvThread(void* lpvoid)
{
	TcpClientNet* pThis = (TcpClientNet*)lpvoid;
	pThis->RecvData();
	return 0;
}
//关闭网络
void  TcpClientNet::UnInitNet()
{
	//线程退出
	m_bIsStop = true;
	if(m_hthreadHandle)
	{
		if(WAIT_TIMEOUT == WaitForSingleObject(m_hthreadHandle,100))
		{
			TerminateThread(m_hthreadHandle,-1);
		}
		CloseHandle(m_hthreadHandle);
		m_hthreadHandle = 0;
	}
	//关闭socket
	if(m_sock != INVALID_SOCKET)
	{
		closesocket(m_sock);
	}
	//卸载库
	WSACleanup();
}
//发送数据
bool  TcpClientNet::SendData(long lSendIp,char* buf,int nLen)
{
    //1校验输入参数合法性
	if(!buf || nLen <= 0)
	{
		return false;
	}
	//2.发送包大小
	if (int a = send(m_sock,(char*)&nLen,sizeof(int),0) <= 0 )
	{
		printf("a = %d\n", WSAGetLastError());
		return false;
	}
	//3.发送包的内容
	if(send(m_sock,buf,nLen,0)<=0)
	{
		return false;
	}
	return true;
}

//接收数据
void  TcpClientNet::RecvData()
{
	int offset = 0;
	m_bIsStop = false;
	int recvNum=0;
	int nPackSize = 0;
	while(!m_bIsStop)
	{
		//1.先接收包的大小
		recvNum = recv(m_sock,(char*)&nPackSize,sizeof(int),0);
		if(recvNum > 0)
		{
			//2.按照包的大小创建空间
			char* Packbuf = new char[nPackSize];
			//3.开始接收数据包
			while(nPackSize)
			{
				//4.每次从偏移量开始写数据，希望写nPackSize这么多数据
				recvNum = recv(m_sock,Packbuf + offset,nPackSize,0);
				if(recvNum > 0)
				{
					nPackSize = nPackSize - recvNum;
					offset += recvNum;
				}
			}
			//4.把数据发送给中介者类
			m_pMediator->DealData(m_sock,Packbuf,offset);
            offset = 0;
			//TODO:回收这个空间 delete []Packbuf
		}
		else
		{
			break;
		}
	}
}
