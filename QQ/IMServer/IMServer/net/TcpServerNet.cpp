#pragma  once
#include"TcpServerNet.h"
#include<windows.h>

TcpServerNet::TcpServerNet(INetMediator* pMediator):m_sock(0),m_bIsStop(false)
{
	m_pMediator = pMediator;
}
TcpServerNet::~TcpServerNet()
{
}
//初始化网络
bool  TcpServerNet::InitNet()
{
	//1.选个项目-加载库   wsa-windows socket api
	WORD wVersionRequested={0};
	WSADATA wsaData={0};
    int result=0;

	/* Use the MAKEWORD(lowbyte, highbyte) macro declared in Windef.h */
    wVersionRequested = MAKEWORD(2,2);

    result = WSAStartup(wVersionRequested, &wsaData);
    if (result != 0) {
        /* Tell the user that we could not find a usable */
        /* Winsock DLL.                                  */
        return false;
    }

	/* Confirm that the WinSock DLL supports 2.2.*/
	/* Note that if the DLL supports versions greater    */
	/* than 2.2 in addition to 2.2, it will still return */
	/* 2.2 in wVersion since that is the version we      */
	/* requested.                                        */

    if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2) {
        /* Tell the user that we could not find a usable */
        /* WinSock DLL.                                  */
        //printf("Could not find a usable version of Winsock.dll\n");
		UnInitNet();//关闭网络
        return false;
    }
    /*else
        printf("The Winsock 2.2 dll was found okay\n");
 */
	//2.(雇个人)创建套接字
	m_sock = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    if (m_sock == INVALID_SOCKET) 
	{
		//WSAGetLastError获取具体的错误码
		//printf("socket function failed with error = %d\n", WSAGetLastError());
		UnInitNet();//关闭网络
        return false;
	}


	//3.绑定IP地址
	sockaddr_in addrClient;
	addrClient.sin_family = AF_INET;//IPV4协议族
	addrClient.sin_addr.S_un.S_addr=INADDR_ANY;//绑定任意网卡，推荐服务器端使用
	addrClient.sin_port = htons(_DEF_TCP_PORT);//htons将证书转为网络字节序
	result = bind(m_sock,(SOCKADDR*)&addrClient,sizeof(addrClient));
	if (result == SOCKET_ERROR) {
		UnInitNet();//关闭网络
        return false;
	}
	
	//4.监听
	result = listen(m_sock,10);
	if(SOCKET_ERROR == result)
	{
		UnInitNet();//关闭网络
		return 1;
	}
	
    //5.接收连接-创建线程     C/C++ RunTime 库  strcpy  创建内存块  CreateThread创建的线程不会回收内存块，造成内存泄露
	//ExitThread退出线程   _beginthreadex底层也是调用CreateThread，退出时调用ExitThread，并且回收创建的内存块
	HANDLE threadHandle =(HANDLE)_beginthreadex(NULL,0,&AcceptThread,this,0,NULL);
	if(threadHandle)
	{
		m_hThreadHandleList.push_back(threadHandle);
	}


	return true;
}

unsigned int _stdcall TcpServerNet::AcceptThread(void* lpvoid)
{
	TcpServerNet* pThis = (TcpServerNet*)lpvoid;
	sockaddr_in addrClient;
	int nSize = sizeof(addrClient);
	while(!pThis->m_bIsStop)
	{
		//1.接受客户端连接
		SOCKET sockWaiter = accept(pThis->m_sock,(SOCKADDR*)&addrClient,&nSize);
		cout<<"ip:"<<inet_ntoa(addrClient.sin_addr)<<"connect"<<endl;

		//2.创建连接的客户端接收数据的线程
		unsigned int threadId = 0;
		HANDLE threadHandle = (HANDLE)_beginthreadex(NULL,0,&RecvThread,pThis,0,&threadId);
		if(threadHandle)
		{
			pThis->m_hThreadHandleList.push_back(threadHandle);
			pThis->m_mapThreadIdToSocket[threadId] = sockWaiter;
		}
	} 
	return 0;
}
unsigned int _stdcall TcpServerNet::RecvThread(void* lpvoid)
{
	//1.因为线程创建以后立刻就开始执行，但是把线程id和sock保存起来需要一定时间，需要等一会
	Sleep(100);
	TcpServerNet* pThis = (TcpServerNet*)lpvoid;
	//2.取出当前线程对应的socket
	SOCKET sockWaiter = pThis->m_mapThreadIdToSocket[GetCurrentThreadId()];
	//3.判断socket的合法性
	if(!sockWaiter || INVALID_SOCKET == sockWaiter)
	{
		return 1;
	}
	//4.接收数据
	int offset = 0;
	int recvNum=0;
	int nPackSize = 0;//存储包大小
	pThis->m_bIsStop = false;
	while(!pThis->m_bIsStop)
	{
		//1.先接收包的大小
		recvNum = recv(sockWaiter,(char*)&nPackSize,sizeof(int),0);
		if(recvNum > 0)
		{
			//2.按照包的大小创建空间
			char* Packbuf = new char[nPackSize];
			//3.开始接收数据包
			while(nPackSize)
			{
				//4.每次从偏移量开始写数据，希望写nPackSize这么多数据
				recvNum = recv(sockWaiter,Packbuf + offset,nPackSize,0);
				if(recvNum > 0)
				{
					nPackSize = nPackSize - recvNum;
					offset += recvNum;
				}
			}
			//4.把数据发送给中介者类
			cout<<"接收到了数据"<<endl;
			pThis->m_pMediator->DealData(sockWaiter,Packbuf,offset);
			offset = 0;
			//TODO:回收这个空间 delete []Packbuf
		}	
		else
		{
			break;
		}
	}
	return 0;
}
//关闭网络
void  TcpServerNet::UnInitNet()
{
	//线程退出
	m_bIsStop = true;
	for(auto ite = m_hThreadHandleList.begin();ite != m_hThreadHandleList.end();)
	{
		if(*ite)
		{
			if(WAIT_TIMEOUT == WaitForSingleObject(*ite,100))
			{
				TerminateThread(*ite,-1);
			}
			CloseHandle(*ite);
			*ite = NULL;
		}
		ite = m_hThreadHandleList.erase(ite);
	}
	
	//关闭socket
	if(m_sock && m_sock != INVALID_SOCKET)
	{
		closesocket(m_sock);
	}
	for(auto ite = m_mapThreadIdToSocket.begin();ite != m_mapThreadIdToSocket.end();)
	{
		if(ite->second && ite->second != INVALID_SOCKET)
		{
			closesocket(ite->second);
		}
		ite = m_mapThreadIdToSocket.erase(ite);
	}

	//卸载库
	WSACleanup();
}
//发送数据
bool  TcpServerNet::SendData(long lSendIp,char* buf,int nLen)
{
	//1断输入参数合法性
	if(!buf || nLen <= 0)
	{
		cout<<"输入参数不合法"<<endl;
		return false;
	}
	//2.发送包大小
	if (send(lSendIp,(char*)&nLen,sizeof(int),0)<=0 )
	{
		return false;
	}
	//3.发送包的内容
	if(send(lSendIp,buf,nLen,0)<=0)
	{
		return false;
	}
	return true;
 }
//接收数据
void  TcpServerNet::RecvData()
{
	
}
