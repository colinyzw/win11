#pragma once

#define _DEF_UDP_PORT  (12345)
#define _DEF_BUFFER_SIZE  (4096)
#define _DEF_NAME_SIZE  (60)
#define _DEF_CONTENT_SIZE (1024)

//协议头 

#define _DEF_PROTOCAL_BASE (1000)
//上线通知  回复
#define _DEF_PROTOCAL_ONLINE_RQ  (_DEF_PROTOCAL_BASE+1)
#define _DEF_PROTOCAL_ONLINE_RS  (_DEF_PROTOCAL_BASE+2)
//下线通知
#define _DEF_PROTOCAL_OFFLINE  (_DEF_PROTOCAL_BASE+3)

//聊天信息
#define _DEF_PROTOCAL_DATEINFO_RQ  (_DEF_PROTOCAL_BASE+4)
#define _DEF_PROTOCAL_DATEINFO_RS  (_DEF_PROTOCAL_BASE+5)


//协议结构
//上线通知、回复   //协议头、IP、名字
struct STRU_ONLINE
{
	STRU_ONLINE():nType(_DEF_PROTOCAL_ONLINE_RQ)
	{
		memset(szName,0,_DEF_NAME_SIZE);
	}
	int nType;
	//long ip;
	char szName[_DEF_NAME_SIZE];
};

//下线通知 //协议头
struct STRU_OFFLINE
{
	STRU_OFFLINE():nType(_DEF_PROTOCAL_OFFLINE)
	{
	}
	int nType;
};

//聊天信息请求   //协议头 IP  聊天内容 序列号
struct STRU_DATAINFO_RQ
{
	STRU_DATAINFO_RQ():nType(_DEF_PROTOCAL_DATEINFO_RQ),seq(0)
	{
		memset(szContent,0,_DEF_CONTENT_SIZE);
	}
	int nType;
	//long  ip;
	char szContent[_DEF_CONTENT_SIZE];
	int seq;
};
//聊天信息回复   //协议头 IP  聊天内容 序列号
struct STRU_DATAINFO_RS
{
	STRU_DATAINFO_RS():nType(_DEF_PROTOCAL_DATEINFO_RS),seq(0)
	{
	}
	int nType;
	//long  ip;
	int seq;
};