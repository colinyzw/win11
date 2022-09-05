#pragma  once
#include"TcpClientNet.h"


TcpClientNet::TcpClientNet(INetMediator* pMediator):m_sock(0),m_hthreadHandle(0),m_bIsStop(false)
{
	m_pMediator = pMediator;
}
TcpClientNet::~TcpClientNet()
{
}
//��ʼ������
bool  TcpClientNet::InitNet()
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
    
	//2.(�͸���)�����׽���
	m_sock = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    if (m_sock == INVALID_SOCKET) 
	{
		//WSAGetLastError��ȡ����Ĵ�����
		//printf("socket function failed with error = %d\n", WSAGetLastError());
		UnInitNet();//�ر�����
        return false;
	}

	
	//3.��������
	sockaddr_in server;
	server.sin_family = AF_INET;//IPV4Э����
	server.sin_addr.S_un.S_addr=inet_addr(_DEF_SERVER_IP);//�ͻ��ˣ����ӷ�������ʹ�÷�������IP��ַ
	server.sin_port = htons(_DEF_TCP_PORT);//htons��֤��תΪ�����ֽ���
	result = connect(m_sock,(SOCKADDR*)&server,sizeof(server));
	if (result == SOCKET_ERROR) {
		cout<<"connectʧ��"<<endl;
		UnInitNet();//�ر�����
        return 1;
	}
	
    //��������-�����߳�     C/C++ RunTime ��  strcpy  �����ڴ��  CreateThread�������̲߳�������ڴ�飬����ڴ�й¶
	//ExitThread�˳��߳�   _beginthreadex�ײ�Ҳ�ǵ���CreateThread���˳�ʱ����ExitThread�����һ��մ������ڴ��
	m_hthreadHandle =(HANDLE)_beginthreadex(NULL,0,&recvThread,this,0,NULL);



	return true;
}
unsigned int _stdcall TcpClientNet::recvThread(void* lpvoid)
{
	TcpClientNet* pThis = (TcpClientNet*)lpvoid;
	pThis->RecvData();
	return 0;
}
//�ر�����
void  TcpClientNet::UnInitNet()
{
	//�߳��˳�
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
	//�ر�socket
	if(m_sock != INVALID_SOCKET)
	{
		closesocket(m_sock);
	}
	//ж�ؿ�
	WSACleanup();
}
//��������
bool  TcpClientNet::SendData(long lSendIp,char* buf,int nLen)
{
	//1����������Ϸ���
	if(!buf || nLen <= 0)
	{
		return false;
	}
	//2.���Ͱ���С
	if (int a = send(m_sock,(char*)&nLen,sizeof(int),0) <= 0 )
	{
		printf("a = %d\n", WSAGetLastError());
		return false;
	}
	//3.���Ͱ�������
	if(send(m_sock,buf,nLen,0)<=0)
	{
		return false;
	}
	return true;
}

//��������
void  TcpClientNet::RecvData()
{
	int offset = 0;
	m_bIsStop = false;
	int recvNum=0;
	int nPackSize = 0;
	while(!m_bIsStop)
	{
		//1.�Ƚ��հ��Ĵ�С
		recvNum = recv(m_sock,(char*)&nPackSize,sizeof(int),0);
		if(recvNum > 0)
		{
			//2.���հ��Ĵ�С�����ռ�
			char* Packbuf = new char[nPackSize];
			//3.��ʼ�������ݰ�
			while(nPackSize)
			{
				//4.ÿ�δ�ƫ������ʼд���ݣ�ϣ��дnPackSize��ô������
				recvNum = recv(m_sock,Packbuf + offset,nPackSize,0);
				if(recvNum > 0)
				{
					nPackSize = nPackSize - recvNum;
					offset += recvNum;
				}
			}
			//4.�����ݷ��͸��н�����
			m_pMediator->DealData(m_sock,Packbuf,offset);
			offset = 0;
			//TODO:��������ռ� delete []Packbuf
		}
		else
		{
			break;
		}
	}
}
