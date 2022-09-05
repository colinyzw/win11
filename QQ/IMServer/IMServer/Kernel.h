#pragma once
#include"CMySql.h"
#include"INetMediator.h"
#include"./net/Package.h"
#include<iostream>
using namespace std;
#include<map>

class CKernel;//先声明，不然typedef会报错
//定义函数指针
typedef void (CKernel::*FUN)(long lSendIp,char* buf,int nLen);

class CKernel
{
public:
	CKernel(void);
	~CKernel(void);
	bool startServer();
	void closeServer();
	void DealData(long lSendIp,char* buf,int nLen);
	 //处理登录请求
    void dealLoginRq(long lSendIp,char* buf,int nLen);
    //处理注册请求
    void dealRegisterRq(long lSendIp,char* buf,int nLen);
	//处理聊天信息请求函数
	void dealChatRq(long lSendIp,char* buf,int nLen);
	//处理添加好友请求函数
	void dealAddFriendRq(long lSendIp,char* buf,int nLen);
	//处理添加好友回复函数
	void dealAddFriendRs(long lSendIp,char* buf,int nLen);
	//处理下线请求函数
	void dealOfflineRq(long lSendIp,char* buf,int nLen);
private:
	//初始化协议映射表
    void setProtocolMap();
	//获取好友列表，包括自己
	void getUserList(int id);
	//根据id查询用户信息
	void getUserInfoFromSql(int id,STRU_FRIEND_INFO* info);
private:
	CMySql* m_sql;
	INetMediator* m_pServer;

	//协议映射表（函数指针数组）【协议头】-函数指针
    FUN m_netProtocolMap[_DEF_PROTOCOL_COUNT];
	//保存id和sock的映射关系
	map<int,int> m_mapIdToSock;

public:
	static CKernel* p_kernel;
};

