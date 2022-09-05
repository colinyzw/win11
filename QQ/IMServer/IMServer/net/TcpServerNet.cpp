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
//��ʼ������
bool  TcpServerNet::InitNet()
{
	//1.ѡ����Ŀ-���ؿ�   wsa-windows socket api
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
		UnInitNet();//�ر�����
        return false;
    }
    /*else
        printf("The Winsock 2.2 dll was found okay\n");
 */
	//2.(�͸���)�����׽���
	m_sock = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    if (m_sock == INVALID_SOCKET) 
	{
		//WSAGetLastError��ȡ����Ĵ�����
		//printf("socket function failed with error = %d\n", WSAGetLastError());
		UnInitNet();//�ر�����
        return false;
	}


	//3.��IP��ַ
	sockaddr_in addrClient;
	addrClient.sin_family = AF_INET;//IPV4Э����
	addrClient.sin_addr.S_un.S_addr=INADDR_ANY;//�������������Ƽ���������ʹ��
	addrClient.sin_port = htons(_DEF_TCP_PORT);//htons��֤��תΪ�����ֽ���
	result = bind(m_sock,(SOCKADDR*)&addrClient,sizeof(addrClient));
	if (result == SOCKET_ERROR) {
		UnInitNet();//�ر�����
        return false;
	}
	
	//4.����
	result = listen(m_sock,10);
	if(SOCKET_ERROR == result)
	{
		UnInitNet();//�ر�����
		return 1;
	}
	
    //5.��������-�����߳�     C/C++ RunTime ��  strcpy  �����ڴ��  CreateThread�������̲߳�������ڴ�飬����ڴ�й¶
	//ExitThread�˳��߳�   _beginthreadex�ײ�Ҳ�ǵ���CreateThread���˳�ʱ����ExitThread�����һ��մ������ڴ��
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
		//1.���ܿͻ�������
		SOCKET sockWaiter = accept(pThis->m_sock,(SOCKADDR*)&addrClient,&nSize);
		cout<<"ip:"<<inet_ntoa(addrClient.sin_addr)<<"connect"<<endl;

		//2.�������ӵĿͻ��˽������ݵ��߳�
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
	//1.��Ϊ�̴߳����Ժ����̾Ϳ�ʼִ�У����ǰ��߳�id��sock����������Ҫһ��ʱ�䣬��Ҫ��һ��
	Sleep(100);
	TcpServerNet* pThis = (TcpServerNet*)lpvoid;
	//2.ȡ����ǰ�̶߳�Ӧ��socket
	SOCKET sockWaiter = pThis->m_mapThreadIdToSocket[GetCurrentThreadId()];
	//3.�ж�socket�ĺϷ���
	if(!sockWaiter || INVALID_SOCKET == sockWaiter)
	{
		return 1;
	}
	//4.��������
	int offset = 0;
	int recvNum=0;
	int nPackSize = 0;//�洢����С
	pThis->m_bIsStop = false;
	while(!pThis->m_bIsStop)
	{
		//1.�Ƚ��հ��Ĵ�С
		recvNum = recv(sockWaiter,(char*)&nPackSize,sizeof(int),0);
		if(recvNum > 0)
		{
			//2.���հ��Ĵ�С�����ռ�
			char* Packbuf = new char[nPackSize];
			//3.��ʼ�������ݰ�
			while(nPackSize)
			{
				//4.ÿ�δ�ƫ������ʼд���ݣ�ϣ��дnPackSize��ô������
				recvNum = recv(sockWaiter,Packbuf + offset,nPackSize,0);
				if(recvNum > 0)
				{
					nPackSize = nPackSize - recvNum;
					offset += recvNum;
				}
			}
			//4.�����ݷ��͸��н�����
			cout<<"���յ�������"<<endl;
			pThis->m_pMediator->DealData(sockWaiter,Packbuf,offset);
			offset = 0;
			//TODO:��������ռ� delete []Packbuf
		}	
		else
		{
			break;
		}
	}
	return 0;
}
//�ر�����
void  TcpServerNet::UnInitNet()
{
	//�߳��˳�
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
	
	//�ر�socket
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

	//ж�ؿ�
	WSACleanup();
}
//��������
bool  TcpServerNet::SendData(long lSendIp,char* buf,int nLen)
{
	//1����������Ϸ���
	if(!buf || nLen <= 0)
	{
		cout<<"����������Ϸ�"<<endl;
		return false;
	}
	//2.���Ͱ���С
	if (send(lSendIp,(char*)&nLen,sizeof(int),0)<=0 )
	{
		return false;
	}
	//3.���Ͱ�������
	if(send(lSendIp,buf,nLen,0)<=0)
	{
		return false;
	}
	return true;
 }
//��������
void  TcpServerNet::RecvData()
{
	
}
