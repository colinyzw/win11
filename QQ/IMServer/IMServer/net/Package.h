#pragma once

#define _DEF_UDP_PORT  (12345)
#define _DEF_TCP_PORT  (54321)
#define _DEF_SERVER_IP ("192.168.21.56")
#define _DEF_BUFFER_SIZE  (4096)
#define _DEF_NAME_SIZE  (60)
#define _DEF_CONTENT_SIZE (1024)


#define _DEF_MAX_SIZE (100)

#define _DEF_PROTOCOL_COUNT (10)

//UDP协议头 
#define _DEF_PROTOCAL_BASE (1000)
//UDP上线通知  回复
#define _DEF_PROTOCAL_ONLINE_RQ  (_DEF_PROTOCAL_BASE+1)
#define _DEF_PROTOCAL_ONLINE_RS  (_DEF_PROTOCAL_BASE+2)
//UDP下线通知
#define _DEF_PROTOCAL_OFFLINE  (_DEF_PROTOCAL_BASE+3)

//UDP聊天信息
#define _DEF_PROTOCAL_DATEINFO_RQ  (_DEF_PROTOCAL_BASE+4)
#define _DEF_PROTOCAL_DATEINFO_RS  (_DEF_PROTOCAL_BASE+5)

//tcp协议头
//注册
#define _DEF_PACK_REGISTER_RQ (_DEF_PROTOCAL_BASE+10)
#define _DEF_PACK_REGISTER_RS (_DEF_PROTOCAL_BASE+11)
//登录
#define _DEF_PACK_LOGIN_RQ (_DEF_PROTOCAL_BASE+12)
#define _DEF_PACK_LOGIN_RS (_DEF_PROTOCAL_BASE+13)
//好友信息
#define _DEF_PACK_FRIEND_INFO (_DEF_PROTOCAL_BASE+14)
//添加好友
#define _DEF_PACK_ADDFRIEND_RQ (_DEF_PROTOCAL_BASE+15)
#define _DEF_PACK_ADDFRIEND_RS (_DEF_PROTOCAL_BASE+16)
//聊天
#define _DEF_PACK_CHAT_RQ (_DEF_PROTOCAL_BASE+17)
#define _DEF_PACK_CHAT_RS (_DEF_PROTOCAL_BASE+18)
//下线
#define _DEF_PACK_OFFINE_RQ (_DEF_PROTOCAL_BASE+19)

//TCP返回结果
//注册请求结果
#define user_is_exist    (0)
#define register_success (1)
#define register_fail    (2)
//登录请求结果
#define user_not_exist   (0)
#define password_error   (1)
#define login_success    (2)
//添加好友请求结果
#define no_this_user (0)
#define user_refuse  (1)
#define user_offline (2)
#define add_success  (3)
//好友状态
#define status_online  (0)
#define status_offline (1)
//聊天信息发送结果
#define send_success (0)
#define send_error   (1)


//UDP协议结构
//UDP上线通知、回复   //协议头、IP、名字
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

//UDP下线通知 //协议头
struct STRU_OFFLINE
{
	STRU_OFFLINE():nType(_DEF_PROTOCAL_OFFLINE)
	{
	}
	int nType;
};

//UDP聊天信息请求   //协议头 IP  聊天内容 序列号
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
//UDP聊天信息回复   //协议头 IP  聊天内容 序列号
struct STRU_DATAINFO_RS
{
	STRU_DATAINFO_RS():nType(_DEF_PROTOCAL_DATEINFO_RS),seq(0)
	{
	}
	int nType;
	//long  ip;
	int seq;
};

//TCP请求结构体
typedef int PackType;
//注册
struct STRU_REGISTER_RQ
{
	STRU_REGISTER_RQ():type(_DEF_PACK_REGISTER_RQ)
	{
		memset(tel,0,_DEF_MAX_SIZE);
		memset(name,0,_DEF_MAX_SIZE);
		memset(password,0,_DEF_MAX_SIZE);
	}
	//昵称、密码、手机号
	PackType type;
	char tel[_DEF_MAX_SIZE];
	char name[_DEF_MAX_SIZE];
	char password[_DEF_MAX_SIZE];
};

struct STRU_REGISTER_RS
{
	STRU_REGISTER_RS():type(_DEF_PACK_REGISTER_RS),result(register_success)
	{}
	//协议头、注册结果
	PackType type;
	int result;
};

//登录
struct STRU_LOGIN_RQ
{
	STRU_LOGIN_RQ():type(_DEF_PACK_LOGIN_RQ)
	{
		memset(tel,0,_DEF_MAX_SIZE);
		memset(password,0,_DEF_MAX_SIZE);
	}
	//密码、手机号
	PackType type;
	char tel[_DEF_MAX_SIZE];
	char password[_DEF_MAX_SIZE];
};
struct STRU_LOGIN_RS
{
	STRU_LOGIN_RS():type(_DEF_PACK_LOGIN_RS),result(login_success),userId(0)
	{}
	//登录结果，用户id
	PackType type;
	int result;
	int userId;
};

//好友信息
struct STRU_FRIEND_INFO
{
	STRU_FRIEND_INFO():type(_DEF_PACK_FRIEND_INFO),userId(0),iconId(0),status(0)
	{
		memset(name,0,_DEF_MAX_SIZE);
		memset(feeling,0,_DEF_MAX_SIZE);
	}
	//用户id，头像id，昵称，签名，状态
	PackType type;
	int userId;
	int iconId;
	char name[_DEF_MAX_SIZE];
	char feeling[_DEF_MAX_SIZE];
	int status;
};

//添加好友
struct STRU_ADD_FRIEND_RQ
{
	STRU_ADD_FRIEND_RQ():type(_DEF_PACK_ADDFRIEND_RQ),userId(0)
	{
		memset(friendName,0,_DEF_MAX_SIZE);
		memset(userName,0,_DEF_MAX_SIZE);
	}
	//本用户id
	PackType type;
	int userId;
	char friendName[_DEF_MAX_SIZE];
	char userName[_DEF_MAX_SIZE];
};

struct STRU_ADD_FRIEND_RS
{
	STRU_ADD_FRIEND_RS():type(_DEF_PACK_ADDFRIEND_RS),userId(0),friendId(0),result(add_success)
	{}
	//本用户id
	PackType type;
	int userId;
	int friendId;
	int result;
};
//聊天
struct STRU_CHAT_RQ
{
	STRU_CHAT_RQ():type(_DEF_PACK_CHAT_RQ),userId(0),friendId(0)
	{
		memset(content,0,_DEF_MAX_SIZE);
	}
	//聊天发送者id，接收者id，聊天内容
	PackType type;
	int userId;
	int friendId;
	char content[_DEF_CONTENT_SIZE];
};

struct STRU_CHAT_RS
{
	STRU_CHAT_RS():type(_DEF_PACK_CHAT_RS),userId(0),friendId(0),result(send_success)
	{}
	//发送结果
	PackType type;
	int userId;
	int friendId;
	int result;
};

//下线
struct STRU_OFFLINE_RQ
{
	STRU_OFFLINE_RQ():type(_DEF_PACK_OFFINE_RQ),userId(0)
	{}
	//发送结果
	PackType type;
	int userId;
};














