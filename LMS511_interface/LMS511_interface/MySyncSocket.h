#pragma once
#include <string>
using namespace std;
// CMySyncSocket 명령 대상입니다.

class CMySyncSocket : public CSocket
{
public:
	CMySyncSocket();
	virtual ~CMySyncSocket();
	virtual void OnReceive(int nErrorCode);

	int ServerOn(int Port);
	int ParentPoint(void* parent);
	int ServerConnect(CString ServerIP, int ServerPort);
	int SendData(CString SendStr);

	void* pParent;
	int ListBoxManage(void);
	int SocketClose(void);
	int GetReceiveData(void);

	unsigned int m_DIST[381];
	int m_XYDist[2][381];
};


