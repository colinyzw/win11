#pragma  once
#include"INetMediator.h"

class TcpServerMediator : public INetMediator
{
public:
	TcpServerMediator();
	 ~TcpServerMediator();//ʹ��ʱ������ָ��ָ�����࣬ʹ������������
	//��ʼ������
	bool  OpenNet();
	//�ر�����
	void  CloseNet();
	//��������
    bool  SendData(long lSendIp,char *buf,int nLen);
protected:
    void  DealData(long lSendIp,char *buf,int nLen);
};
