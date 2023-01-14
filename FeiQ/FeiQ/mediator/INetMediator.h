#pragma  once
#include<QObject>

class INet;
class INetMediator : public QObject
{
    Q_OBJECT
public:
    INetMediator();
    virtual ~INetMediator();//使用时，父类指针指向子类，使用虚析构函数
	//初始化网络
	virtual  bool  OpenNet()=0;
	//关闭网络
	virtual  void  CloseNet()=0;
	//发送数据
	virtual  bool  SendData(long lSendIp,char  *buf,int  nLen)=0;
	//处理数据
	virtual void  DealData(long lSendIp,char  *buf,int  nLen)=0;
protected:
	INet*  m_pNet;
};
