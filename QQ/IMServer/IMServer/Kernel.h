#pragma once
#include"CMySql.h"
#include"INetMediator.h"
#include"./net/Package.h"
#include<iostream>
using namespace std;
#include<map>

class CKernel;//����������Ȼtypedef�ᱨ��
//���庯��ָ��
typedef void (CKernel::*FUN)(long lSendIp,char* buf,int nLen);

class CKernel
{
public:
	CKernel(void);
	~CKernel(void);
	bool startServer();
	void closeServer();
	void DealData(long lSendIp,char* buf,int nLen);
	 //�����¼����
    void dealLoginRq(long lSendIp,char* buf,int nLen);
    //����ע������
    void dealRegisterRq(long lSendIp,char* buf,int nLen);
	//����������Ϣ������
	void dealChatRq(long lSendIp,char* buf,int nLen);
	//������Ӻ���������
	void dealAddFriendRq(long lSendIp,char* buf,int nLen);
	//������Ӻ��ѻظ�����
	void dealAddFriendRs(long lSendIp,char* buf,int nLen);
	//��������������
	void dealOfflineRq(long lSendIp,char* buf,int nLen);
private:
	//��ʼ��Э��ӳ���
    void setProtocolMap();
	//��ȡ�����б������Լ�
	void getUserList(int id);
	//����id��ѯ�û���Ϣ
	void getUserInfoFromSql(int id,STRU_FRIEND_INFO* info);
private:
	CMySql* m_sql;
	INetMediator* m_pServer;

	//Э��ӳ�������ָ�����飩��Э��ͷ��-����ָ��
    FUN m_netProtocolMap[_DEF_PROTOCOL_COUNT];
	//����id��sock��ӳ���ϵ
	map<int,int> m_mapIdToSock;

public:
	static CKernel* p_kernel;
};

