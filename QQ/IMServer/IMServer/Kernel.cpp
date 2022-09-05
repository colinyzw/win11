#include "Kernel.h"
#include"mediator\TcpServerMediator.h"

//����Э��ӳ���ϵ
#define NetPackFunMap(a) m_netProtocolMap[a - _DEF_PROTOCAL_BASE - 10]

//��ʼ��Э��ӳ���
void CKernel::setProtocolMap()
{
    //1.��ʼ������
    memset(m_netProtocolMap,0,sizeof(m_netProtocolMap));
    //2.��������
    NetPackFunMap(_DEF_PACK_REGISTER_RQ) = &CKernel::dealRegisterRq;
	NetPackFunMap(_DEF_PACK_LOGIN_RQ)= &CKernel::dealLoginRq;
	NetPackFunMap(_DEF_PACK_CHAT_RQ)= &CKernel::dealChatRq;
	NetPackFunMap(_DEF_PACK_ADDFRIEND_RQ)= &CKernel::dealAddFriendRq;
	NetPackFunMap(_DEF_PACK_ADDFRIEND_RS)= &CKernel::dealAddFriendRs;
	NetPackFunMap(_DEF_PACK_OFFINE_RQ)= &CKernel::dealOfflineRq;

}

CKernel* CKernel::p_kernel = NULL;
CKernel::CKernel(void)
{
	CKernel::p_kernel = this;
	setProtocolMap();
}


CKernel::~CKernel(void)
{
	closeServer();
}

bool CKernel::startServer()
{
	//1.�������ݿ�
	m_sql = new CMySql;
	if(!m_sql->ConnectMySql("127.0.0.1","root","y15840213581","0524im"))
	{
		cout << "�������ݿ�ʧ��" <<endl;
		return false;
	}
	//2.������ 
	m_pServer = new TcpServerMediator;
	if(!m_pServer->OpenNet())
	{
		cout << "������ʧ��" << endl;
		return false;
	}
	return true;
	cout << "startServer success" << endl;
}

void CKernel::closeServer()
{
	//����������Դ
	//�Ͽ����ݿ�����
	if(m_sql)
	{
		m_sql->DisConnect();
		delete m_sql;
		m_sql = NULL;
	}
	//�ر�����
	if(m_pServer)
	{
		m_pServer->CloseNet();
		delete m_pServer;
		m_pServer = NULL;
	}
}

void CKernel::DealData(long lSendIp,char* buf,int nLen)
{
	cout << "��������" << endl;
	 //1.���
    int nType = *(int*)buf;
    //2.����Э��ͷ����ͬ������
    //2.1�ж�Э��ͷ�ǲ����������±귶Χ��
    if(nType >= (_DEF_PROTOCAL_BASE + 10)
            && nType < (_DEF_PROTOCAL_BASE + 10 + _DEF_PROTOCOL_COUNT))
    {
        //2.2����������±��Ӧ�ĺ���
        FUN pf = NetPackFunMap(nType);
        //2.3ִ�к���
        if(pf)
        {
            (this->*pf)(lSendIp,buf,nLen);
        }
    }
    //3.���ս��պ��������Ŀռ�
    delete[] buf;
}

//�����¼����
void CKernel::dealLoginRq(long lSendIp,char* buf,int nLen)
{
	cout << "�����¼����" <<endl;
	//1.���
	STRU_LOGIN_RQ* rq = (STRU_LOGIN_RQ*)buf;
	//2.����tel��ѯ�û��Ƿ����
	list<string> resList;
	char sqlBuf[1024] = "";
	sprintf(sqlBuf,"select id,password from t_user where tel = '%s'",rq->tel);
	if(!m_sql->SelectMySql(sqlBuf,2,resList))
	{
		cout << "select error:" << sqlBuf << endl;
	}
	//3.��������ڣ�ֱ�ӷ��ص�¼������Ϣ--�û�������
	STRU_LOGIN_RS rs;
	int nId = 0;
	string strPass = "";
	if(resList.size() == 0)
	{
		rs.result = user_not_exist;
		//���͵�¼���
		m_pServer->SendData(lSendIp,(char*)&rs,sizeof(rs));
	}
	else
	{
		//4.������ڣ��Ȼ�ȡ�û���id������
		nId = atoi(resList.front().c_str());
		resList.pop_front();
		strPass = resList.front();
		resList.pop_front();
		//5.��������Ƿ����
		if(strcmp(strPass.c_str(),rq->password) != 0)
		{
			//6.���벻��ȷ��ֱ�ӷ��ص�¼������Ϣ--���벻��ȷ
			rs.result = password_error;
			//���͵�¼���
			m_pServer->SendData(lSendIp,(char*)&rs,sizeof(rs));
		}
		else
		{
			//7.������ȷ��ֱ�ӷ��ص�¼���--�û���id
			rs.result = login_success;
			rs.userId = nId;
			//���͵�¼���
			m_pServer->SendData(lSendIp,(char*)&rs,sizeof(rs));

			//8.����id��sock��ӳ���ϵ
			m_mapIdToSock[nId] = lSendIp;
			//9.��ȡ�����б�
			getUserList(nId);
		}
	}
	
}

//����ע������
void CKernel::dealRegisterRq(long lSendIp,char* buf,int nLen)
{
	//1.���
	STRU_REGISTER_RQ* rq = (STRU_REGISTER_RQ*)buf;
	//2.�õ�tel,��telΪ������ѯ���ݿ�
	list<string> resList;
	char sqlBuf[1024]="";
	sprintf(sqlBuf,"select tel from t_user where tel = '%s'",rq->tel);
	if(!m_sql->SelectMySql(sqlBuf,1,resList))
	{
		cout<< "��ѯ���ݿ�ʧ��" <<endl;
		return;
	}
	//3.�ж��Ƿ��Ѿ����������
	STRU_REGISTER_RS rs;
	if(resList.size() > 0)//�������
	{
		//�ظ��ͻ��˸��û��Ѿ�����
		rs.result = user_is_exist;
	}
	else
	{//û�������
		//4.��ӵ����ݿ�
		rs.result = register_success;
		sprintf(sqlBuf,"insert into t_user (tel,password,name,icon,feeling)"
			"values ('%s','%s','%s',1,'�Ƚ�����ʲôҲûд')",rq->tel,rq->password,rq->name);
		if(!m_sql->UpdateMySql(sqlBuf))
		{
			cout<< "�������ݿ�ʧ��" << endl;
			rs.result = register_fail;
		}
	}
	
	//5.���ͻ��˷���ע����
	m_pServer->SendData(lSendIp,(char*)&rs,sizeof(rs));
}

//��ȡ�����б������Լ�
void CKernel::getUserList(int id)
{
	cout<<"��ȡ�����б������Լ�"<<endl;
	//1.�Ȳ�ѯ�Լ�����Ϣ
	STRU_FRIEND_INFO loginRq;
	getUserInfoFromSql(id,&loginRq);
	//2.ȡ���Լ���sock
	if(m_mapIdToSock.find(id) == m_mapIdToSock.end())
	{
		return;
	}
	SOCKET sockLoginer = m_mapIdToSock[id];
	//3.�����Լ�����Ϣ���ͻ���
	m_pServer->SendData(sockLoginer,(char*)&loginRq,sizeof(loginRq));
	//4.��ѯ�û����к��ѵ�id
	list<string> lstRes;
	char sqlBuf[1024] = "";
	sprintf(sqlBuf,"select idB from t_friend where idA = '%d';",id);
	if(!m_sql->SelectMySql(sqlBuf,1,lstRes))
	{
		cout << "select error:" << sqlBuf << endl;
	}
	//5.������ѯ���
	int friendId = 0;
	STRU_FRIEND_INFO friendRq;
	while(lstRes.size() > 0)
	{
		//ȡ������id
		friendId = atoi(lstRes.front().c_str());
		lstRes.pop_front();

		//6.����id��ѯ���ѵ��û���Ϣ
		getUserInfoFromSql(friendId,&friendRq);

		//7.���ͻ��˷��͸��û��ĺ�����Ϣ
		m_pServer->SendData(sockLoginer,(char*)&friendRq,sizeof(friendRq));

		//8.����������ߣ����͵�¼�˵���Ϣ������
		if(m_mapIdToSock.find(friendId) == m_mapIdToSock.end())
		{
			continue;
		}

		//9.�������ߣ�ȡ�����ѵ�sock��֪ͨ���ѵ�¼���Ѿ�����
		m_pServer->SendData(m_mapIdToSock[friendId],(char*)&loginRq,sizeof(loginRq));
	
	}
	
}

//����id��ѯ�û���Ϣ
void CKernel::getUserInfoFromSql(int id,STRU_FRIEND_INFO* info)
{
	info->userId = id;
	list<string> lstRes;
	char sqlBuf[1024] = "";
	sprintf(sqlBuf,"select name,icon,feeling from t_user where id = '%d';",id);
	if(!m_sql->SelectMySql(sqlBuf,3,lstRes))
	{
		cout << "select error:" << sqlBuf << endl;
	}
	while(lstRes.size() > 0)
	{
		strcpy(info->name,lstRes.front().c_str());
		lstRes.pop_front();

		info->iconId = atoi(lstRes.front().c_str());
		lstRes.pop_front();

		strcpy(info->feeling,lstRes.front().c_str());
		lstRes.pop_front();
	}

	//�ж��û��Ƿ�����
	if(m_mapIdToSock.find(id) != m_mapIdToSock.end())
	{
		info->status = 1;
	}
	else
	{
		info->status = 0;//δ�ҵ���״̬Ϊ0��������
	}

}

//����������Ϣ������
void CKernel::dealChatRq(long lSendIp,char* buf,int nLen)
{
	//1.���
	STRU_CHAT_RQ* rq = (STRU_CHAT_RQ*)buf;
	//2.�ж϶Է��Ƿ�����
	if(m_mapIdToSock.find(rq->friendId) == m_mapIdToSock.end() )
	{ 
		//2.1�����ߣ��ظ��ͻ��˺��Ѳ�����
		STRU_CHAT_RS rs;
		rs.userId = rq->userId;
		rs.friendId = rq->friendId;
		rs.result = user_offline;
		m_pServer->SendData(lSendIp,(char*)&rs,sizeof(rs));
	}
	else
	{
		//2.2���ߣ�����idȡ��sockת������
		SOCKET sockFriend = m_mapIdToSock[rq->friendId];
		m_pServer->SendData(sockFriend,buf,nLen);
	}

}

//������Ӻ���������
void CKernel::dealAddFriendRq(long lSendIp,char* buf,int nLen)
{
	//1.���
	STRU_ADD_FRIEND_RQ* rq = (STRU_ADD_FRIEND_RQ*)buf;
	//2.�û��Ƿ����
	list<string> lstRes;
	char sqlBuf[1024] = "";
	sprintf(sqlBuf,"select id from t_user where name = '%s';",rq->friendName);
	if(!m_sql->SelectMySql(sqlBuf,1,lstRes))
	{
		cout << "select error:" << sqlBuf << endl;
	}
	if(lstRes.size() > 0)
	{
		//3.ȡ���û�id
		int friendId = atoi(lstRes.front().c_str());
		lstRes.pop_front();
		//4.�û��Ƿ�����
		if(m_mapIdToSock.count(friendId) > 0)
		{
			//4.1���ߣ��Ȼ�ȡ�׽��֣�ת����Ӻ�������
			SOCKET sock = m_mapIdToSock[friendId];
			m_pServer->SendData(sock,buf,nLen);
		}
		else
		{
			//4.2�����ߣ�ֱ�ӻظ��û�������
			STRU_ADD_FRIEND_RS rs;
			rs.friendId = friendId;
			rs.userId = rq->userId;
			rs.result = user_offline;
			m_pServer->SendData(lSendIp,(char*)&rs,sizeof(rs));
		}
	}
	else
	{
		//2.2�û������ڣ�ֱ�ӻظ��û�������
		STRU_ADD_FRIEND_RS rs;
		rs.friendId = 0;
		rs.userId = rq->userId;
		rs.result = no_this_user;
		m_pServer->SendData(lSendIp,(char*)&rs,sizeof(rs));
	}

}

//������Ӻ��ѻظ�����
void CKernel::dealAddFriendRs(long lSendIp,char* buf,int nLen)
{
	//1.���
	STRU_ADD_FRIEND_RS* rs = (STRU_ADD_FRIEND_RS*)buf;
	//2.�ж϶Է��Ƿ�ͬ����Ӻ���
	if(rs->result == add_success)
	{
		//2.1���ͬ�⣬�����ݿ���д����ѹ�ϵ
		char sqlBuf[1024] = "";
		sprintf(sqlBuf,"insert into t_friend values (%d,%d);",rs->friendId,rs->userId);
		if(!m_sql->UpdateMySql(sqlBuf))
		{
			cout << "update error:" << sqlBuf << endl;
		}
		sprintf(sqlBuf,"insert into t_friend values (%d,%d);",rs->userId,rs->friendId);
		if(!m_sql->UpdateMySql(sqlBuf))
		{
			cout << "update error:" << sqlBuf << endl;
		}
		//2.2���º����б�
		getUserList(rs->friendId);
	}
	//3.���ܶԷ��Ƿ�ͬ����Ӻ��ѣ�����Ҫת��������
	if(m_mapIdToSock.count(rs->userId) > 0)
	{
		SOCKET sock = m_mapIdToSock[rs->userId];
		m_pServer->SendData(sock,buf,nLen);
	}
}

//��������������
void CKernel::dealOfflineRq(long lSendIp,char* buf,int nLen)
{
	//1.���
	STRU_OFFLINE_RQ* rq = (STRU_OFFLINE_RQ*)buf;

	//2.��ȡid
	int userId = rq->userId;

	//3.����id��ѯ����id
	list<string> lstRes;
	char sqlBuf[1024] = "";
	sprintf(sqlBuf,"select idB from t_friend where idA = '%d';",userId);
	if(!m_sql->SelectMySql(sqlBuf,1,lstRes))
	{
		cout << "select error:" << sqlBuf << endl;
	}
	//4.������ѯ���
	int friendId = 0;
	while(lstRes.size() > 0)
	{
		//5.���ݺ���id��������֪ͨ
		friendId = atoi(lstRes.front().c_str());
		lstRes.pop_front();
		if(0 == m_mapIdToSock.count(friendId))
		{
			continue;
		}
		SOCKET sock = m_mapIdToSock[friendId];
		m_pServer->SendData(sock,buf,nLen);
	}

	//6.�������û���map��ɾ��
	if(m_mapIdToSock.count(userId) > 0)
	{
		m_mapIdToSock.erase(userId);
	}
	
}