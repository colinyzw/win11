#pragma  once
#include<Winsock2.h>
#include<iostream>
#include<package.h>
#include<process.h>
#include"INetMediator.h"
#include<string>
#include<set>
#include <ws2tcpip.h>
using namespace std;

// Need to link with Ws2_32.lib
#pragma comment(lib, "ws2_32.lib")//QT中链接库不认识vs中的



//class INetMediator;
class INet
{
public:
	INet(){}	//父类--没有源文件实现的构造析构函数，需要在这里加上{}，不然会报错，有源文件实现的只需要用分号；
	virtual ~INet(){}//使用时，父类指针指向子类，使用虚析构函数
	//初始化网络
	virtual  bool  InitNet()=0;
	//关闭网络
	virtual  void  UnInitNet()=0;
	//发送数据
	virtual  bool  SendData(long lSendIp,char  *buf,int  nLen)=0;
    //IP地址转换
    static std::string GetipString(long ip)
    {
        SOCKADDR_IN sockaddr;
        sockaddr.sin_addr.S_un.S_addr=ip;
        return inet_ntoa(sockaddr.sin_addr);
    }
    //获取本地ip地址列表
    static std::set<long>  GetValidIpList()
    {
        //1.获取主机名
        char host[100] = "";
        if(SOCKET_ERROR != gethostname(host,100))
        {
            //2.获取IP地址
            std::set<long> SetIp;
            struct hostent *remoteHost;//用以存储主机的各种信息
            int i = 0;
            remoteHost = gethostbyname(host);//通过主机名得到特定主机的信息
            while(remoteHost->h_addr_list[i] != 0)
            {
                SetIp.insert(*(u_long*)remoteHost->h_addr_list[i++]);
            }
            return SetIp;
        }
        else
        {
            return std::set<long>();
        }

    }
protected:
	virtual void  RecvData()=0;
	INetMediator*  m_pMediator;//中介者接口指针
};
