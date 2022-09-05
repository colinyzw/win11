#include "Kernel.h"
#include"mediator\TcpServerMediator.h"

//设置协议映射关系
#define NetPackFunMap(a) m_netProtocolMap[a - _DEF_PROTOCAL_BASE - 10]

//初始化协议映射表
void CKernel::setProtocolMap()
{
    //1.初始化数组
    memset(m_netProtocolMap,0,sizeof(m_netProtocolMap));
    //2.设置数组
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
	//1.连接数据库
	m_sql = new CMySql;
	if(!m_sql->ConnectMySql("127.0.0.1","root","y15840213581","0524im"))
	{
		cout << "连接数据库失败" <<endl;
		return false;
	}
	//2.打开网络 
	m_pServer = new TcpServerMediator;
	if(!m_pServer->OpenNet())
	{
		cout << "打开网络失败" << endl;
		return false;
	}
	return true;
	cout << "startServer success" << endl;
}

void CKernel::closeServer()
{
	//回收其他资源
	//断开数据库连接
	if(m_sql)
	{
		m_sql->DisConnect();
		delete m_sql;
		m_sql = NULL;
	}
	//关闭网络
	if(m_pServer)
	{
		m_pServer->CloseNet();
		delete m_pServer;
		m_pServer = NULL;
	}
}

void CKernel::DealData(long lSendIp,char* buf,int nLen)
{
	cout << "处理数据" << endl;
	 //1.拆包
    int nType = *(int*)buf;
    //2.根据协议头处理不同的数据
    //2.1判断协议头是不是在数组下标范围内
    if(nType >= (_DEF_PROTOCAL_BASE + 10)
            && nType < (_DEF_PROTOCAL_BASE + 10 + _DEF_PROTOCOL_COUNT))
    {
        //2.2计算出数组下标对应的函数
        FUN pf = NetPackFunMap(nType);
        //2.3执行函数
        if(pf)
        {
            (this->*pf)(lSendIp,buf,nLen);
        }
    }
    //3.回收接收函数创建的空间
    delete[] buf;
}

//处理登录请求
void CKernel::dealLoginRq(long lSendIp,char* buf,int nLen)
{
	cout << "处理登录请求" <<endl;
	//1.拆包
	STRU_LOGIN_RQ* rq = (STRU_LOGIN_RQ*)buf;
	//2.根据tel查询用户是否存在
	list<string> resList;
	char sqlBuf[1024] = "";
	sprintf(sqlBuf,"select id,password from t_user where tel = '%s'",rq->tel);
	if(!m_sql->SelectMySql(sqlBuf,2,resList))
	{
		cout << "select error:" << sqlBuf << endl;
	}
	//3.如果不存在，直接返回登录错误信息--用户不存在
	STRU_LOGIN_RS rs;
	int nId = 0;
	string strPass = "";
	if(resList.size() == 0)
	{
		rs.result = user_not_exist;
		//发送登录结果
		m_pServer->SendData(lSendIp,(char*)&rs,sizeof(rs));
	}
	else
	{
		//4.如果存在，先获取用户的id和密码
		nId = atoi(resList.front().c_str());
		resList.pop_front();
		strPass = resList.front();
		resList.pop_front();
		//5.检查密码是否错误
		if(strcmp(strPass.c_str(),rq->password) != 0)
		{
			//6.密码不正确，直接返回登录错误信息--密码不正确
			rs.result = password_error;
			//发送登录结果
			m_pServer->SendData(lSendIp,(char*)&rs,sizeof(rs));
		}
		else
		{
			//7.密码正确，直接返回登录结果--用户的id
			rs.result = login_success;
			rs.userId = nId;
			//发送登录结果
			m_pServer->SendData(lSendIp,(char*)&rs,sizeof(rs));

			//8.保存id和sock的映射关系
			m_mapIdToSock[nId] = lSendIp;
			//9.获取好友列表
			getUserList(nId);
		}
	}
	
}

//处理注册请求
void CKernel::dealRegisterRq(long lSendIp,char* buf,int nLen)
{
	//1.拆包
	STRU_REGISTER_RQ* rq = (STRU_REGISTER_RQ*)buf;
	//2.拿到tel,以tel为条件查询数据库
	list<string> resList;
	char sqlBuf[1024]="";
	sprintf(sqlBuf,"select tel from t_user where tel = '%s'",rq->tel);
	if(!m_sql->SelectMySql(sqlBuf,1,resList))
	{
		cout<< "查询数据库失败" <<endl;
		return;
	}
	//3.判断是否已经有这个人了
	STRU_REGISTER_RS rs;
	if(resList.size() > 0)//有这个人
	{
		//回复客户端该用户已经存在
		rs.result = user_is_exist;
	}
	else
	{//没有这个人
		//4.添加到数据库
		rs.result = register_success;
		sprintf(sqlBuf,"insert into t_user (tel,password,name,icon,feeling)"
			"values ('%s','%s','%s',1,'比较懒，什么也没写')",rq->tel,rq->password,rq->name);
		if(!m_sql->UpdateMySql(sqlBuf))
		{
			cout<< "更新数据库失败" << endl;
			rs.result = register_fail;
		}
	}
	
	//5.给客户端发送注册结果
	m_pServer->SendData(lSendIp,(char*)&rs,sizeof(rs));
}

//获取好友列表，包括自己
void CKernel::getUserList(int id)
{
	cout<<"获取好友列表，包括自己"<<endl;
	//1.先查询自己的信息
	STRU_FRIEND_INFO loginRq;
	getUserInfoFromSql(id,&loginRq);
	//2.取出自己的sock
	if(m_mapIdToSock.find(id) == m_mapIdToSock.end())
	{
		return;
	}
	SOCKET sockLoginer = m_mapIdToSock[id];
	//3.发送自己的信息给客户端
	m_pServer->SendData(sockLoginer,(char*)&loginRq,sizeof(loginRq));
	//4.查询用户所有好友的id
	list<string> lstRes;
	char sqlBuf[1024] = "";
	sprintf(sqlBuf,"select idB from t_friend where idA = '%d';",id);
	if(!m_sql->SelectMySql(sqlBuf,1,lstRes))
	{
		cout << "select error:" << sqlBuf << endl;
	}
	//5.遍历查询结果
	int friendId = 0;
	STRU_FRIEND_INFO friendRq;
	while(lstRes.size() > 0)
	{
		//取出好友id
		friendId = atoi(lstRes.front().c_str());
		lstRes.pop_front();

		//6.根据id查询好友的用户信息
		getUserInfoFromSql(friendId,&friendRq);

		//7.给客户端发送该用户的好友信息
		m_pServer->SendData(sockLoginer,(char*)&friendRq,sizeof(friendRq));

		//8.如果好友在线，发送登录人的信息给好友
		if(m_mapIdToSock.find(friendId) == m_mapIdToSock.end())
		{
			continue;
		}

		//9.好友在线，取出好友的sock，通知好友登录人已经上线
		m_pServer->SendData(m_mapIdToSock[friendId],(char*)&loginRq,sizeof(loginRq));
	
	}
	
}

//根据id查询用户信息
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

	//判断用户是否在线
	if(m_mapIdToSock.find(id) != m_mapIdToSock.end())
	{
		info->status = 1;
	}
	else
	{
		info->status = 0;//未找到，状态为0，不在线
	}

}

//处理聊天信息请求函数
void CKernel::dealChatRq(long lSendIp,char* buf,int nLen)
{
	//1.拆包
	STRU_CHAT_RQ* rq = (STRU_CHAT_RQ*)buf;
	//2.判断对方是否在线
	if(m_mapIdToSock.find(rq->friendId) == m_mapIdToSock.end() )
	{ 
		//2.1不在线，回复客户端好友不在线
		STRU_CHAT_RS rs;
		rs.userId = rq->userId;
		rs.friendId = rq->friendId;
		rs.result = user_offline;
		m_pServer->SendData(lSendIp,(char*)&rs,sizeof(rs));
	}
	else
	{
		//2.2在线，根据id取出sock转发数据
		SOCKET sockFriend = m_mapIdToSock[rq->friendId];
		m_pServer->SendData(sockFriend,buf,nLen);
	}

}

//处理添加好友请求函数
void CKernel::dealAddFriendRq(long lSendIp,char* buf,int nLen)
{
	//1.拆包
	STRU_ADD_FRIEND_RQ* rq = (STRU_ADD_FRIEND_RQ*)buf;
	//2.用户是否存在
	list<string> lstRes;
	char sqlBuf[1024] = "";
	sprintf(sqlBuf,"select id from t_user where name = '%s';",rq->friendName);
	if(!m_sql->SelectMySql(sqlBuf,1,lstRes))
	{
		cout << "select error:" << sqlBuf << endl;
	}
	if(lstRes.size() > 0)
	{
		//3.取出用户id
		int friendId = atoi(lstRes.front().c_str());
		lstRes.pop_front();
		//4.用户是否在线
		if(m_mapIdToSock.count(friendId) > 0)
		{
			//4.1在线，先获取套接字，转发添加好友请求
			SOCKET sock = m_mapIdToSock[friendId];
			m_pServer->SendData(sock,buf,nLen);
		}
		else
		{
			//4.2不在线，直接回复用户不在线
			STRU_ADD_FRIEND_RS rs;
			rs.friendId = friendId;
			rs.userId = rq->userId;
			rs.result = user_offline;
			m_pServer->SendData(lSendIp,(char*)&rs,sizeof(rs));
		}
	}
	else
	{
		//2.2用户不存在，直接回复用户不存在
		STRU_ADD_FRIEND_RS rs;
		rs.friendId = 0;
		rs.userId = rq->userId;
		rs.result = no_this_user;
		m_pServer->SendData(lSendIp,(char*)&rs,sizeof(rs));
	}

}

//处理添加好友回复函数
void CKernel::dealAddFriendRs(long lSendIp,char* buf,int nLen)
{
	//1.拆包
	STRU_ADD_FRIEND_RS* rs = (STRU_ADD_FRIEND_RS*)buf;
	//2.判断对方是否同意添加好友
	if(rs->result == add_success)
	{
		//2.1如果同意，往数据库里写入好友关系
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
		//2.2更新好友列表
		getUserList(rs->friendId);
	}
	//3.不管对方是否同意添加好友，都需要转发请求结果
	if(m_mapIdToSock.count(rs->userId) > 0)
	{
		SOCKET sock = m_mapIdToSock[rs->userId];
		m_pServer->SendData(sock,buf,nLen);
	}
}

//处理下线请求函数
void CKernel::dealOfflineRq(long lSendIp,char* buf,int nLen)
{
	//1.拆包
	STRU_OFFLINE_RQ* rq = (STRU_OFFLINE_RQ*)buf;

	//2.获取id
	int userId = rq->userId;

	//3.根据id查询好友id
	list<string> lstRes;
	char sqlBuf[1024] = "";
	sprintf(sqlBuf,"select idB from t_friend where idA = '%d';",userId);
	if(!m_sql->SelectMySql(sqlBuf,1,lstRes))
	{
		cout << "select error:" << sqlBuf << endl;
	}
	//4.遍历查询结果
	int friendId = 0;
	while(lstRes.size() > 0)
	{
		//5.根据好友id发送下线通知
		friendId = atoi(lstRes.front().c_str());
		lstRes.pop_front();
		if(0 == m_mapIdToSock.count(friendId))
		{
			continue;
		}
		SOCKET sock = m_mapIdToSock[friendId];
		m_pServer->SendData(sock,buf,nLen);
	}

	//6.把下线用户从map中删除
	if(m_mapIdToSock.count(userId) > 0)
	{
		m_mapIdToSock.erase(userId);
	}
	
}