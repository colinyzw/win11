#pragma  once
#include"INetMediator.h"

class TcpClientMediator : public INetMediator
{
public:
	TcpClientMediator();
	 ~TcpClientMediator();//ʹ��ʱ������ָ��ָ�����࣬ʹ������������
	//��ʼ������
	bool  OpenNet();
	//�ر�����
	void  CloseNet();
	//��������
    bool  SendData(long lSendIp,char *buf,int nLen);
protected:
    void  DealData(long lSendIp,char *buf,int nLen);
};
