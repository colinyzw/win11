#include<iostream>
#include"CMySql.h"
#include<windows.h>
#include"Kernel.h"
using namespace std;


int main()
{
	//INetMediator* m_server = new TcpServerMediator;
	//if(!m_server->OpenNet())
	//{
	//	cout<<"������ʧ��"<<endl;
	//}
	//INetMediator* m_client = new TcpClientMediator;
	//if(!m_client->OpenNet())
	//{
	//	cout<<"������ʧ��"<<endl;
	//}
	//m_client->SendData(0,"hello world",strlen("hello world")+1);
	//
	////ѭ��һ�ᣬ��Ϊ�����շ���Ҫʱ��
	//while(1)
	//{
	//	Sleep(1000);
	//	cout<<"server running"<<endl;
	//}
	CKernel kernel;
	if(kernel.startServer())
	{
		while(1)
		{
			cout<<"server running"<<endl;
			Sleep(1000);
		}
	}
	else
	{
		cout << "server failed" <<endl;
	}




	system("pause");
	return 0;
}