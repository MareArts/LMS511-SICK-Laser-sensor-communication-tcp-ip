// MySyncSocket.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "LMS511_interface.h"
#include "MySyncSocket.h"
#include "LMS511_interfaceDlg.h"
#include <math.h>
static UINT DataGetThread(LPVOID pParam);

// CMySyncSocket

CMySyncSocket::CMySyncSocket()
{
}

CMySyncSocket::~CMySyncSocket()
{
}


// CMySyncSocket 멤버 함수

void CMySyncSocket::OnReceive(int nErrorCode)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	((CLMS511_interfaceDlg*)pParent)->m_ListBox.AddString("Receive");

	CString strIPAddress,tmpStr;
	UINT uPortNumber=0;
	char szBuffer[2048] = {0,};

	int bytesRead=0;
	if( (bytesRead = ReceiveFrom(szBuffer, 2048, (CString&)strIPAddress, (UINT&)uPortNumber)) == SOCKET_ERROR)
	{
		tmpStr.Format("[%s:%d] - Receive Error", strIPAddress, uPortNumber);
		((CLMS511_interfaceDlg*)pParent)->m_ListBox.AddString(tmpStr);

	}else{
		tmpStr.Format("[%s:%d:%dbytes]", strIPAddress, uPortNumber, bytesRead);
		((CLMS511_interfaceDlg*)pParent)->m_ListBox.AddString(tmpStr);
	}

	char checkStrA1[16] = "sRA LMDscandata";
	char checkStrA2[16] = "sSN LMDscandata";

	CString inStr;
	for(int i=1; i<16; ++i)
		inStr.Format("%s%c", inStr, szBuffer[i]);


	//check initial String
	if( (inStr == checkStrA1) || (inStr == checkStrA2) )
	{
		//find data number table
		int cnt=0;
		int pCnt=0;
		int dCnt=0;
		while(1)
		{
			if( szBuffer[pCnt++] == 0x20 )
			{
				cnt++;

				if(cnt == 25)
					dCnt=pCnt;
				if(cnt == 26)
					break;
			}
			
		}

		memset(m_DIST, 381, 0);

		CString testStr;
		CString OneDIST;
		char s[4];
		unsigned long tx, x;

		//read data count
		sprintf(s,"%c%c%c", szBuffer[dCnt], szBuffer[dCnt+1], szBuffer[dCnt+2]);		  
		tx = strtoul(s, 0, 16);
		testStr.Format("---- %d 0x20=%d %d-----",tx, dCnt, pCnt);
		((CLMS511_interfaceDlg*)pParent)->m_ListBox.AddString(testStr);

		//Get data
		int Di=pCnt;
		for(int i=0; i<tx; ++i)
		{

			OneDIST="";
			int sI=0;
			while( szBuffer[Di] != 0x20)
			{
				OneDIST.Format("%s%c", OneDIST, szBuffer[Di++]);
			}
			Di++;

			if(OneDIST == "0")
			{
				m_DIST[i] = 0;
			}else{

				char* st = LPSTR(LPCTSTR(OneDIST));
				x = strtoul(OneDIST, 0, 16);
				m_DIST[i] = x;
			}
		}

		//
		for(int i=0; i<tx; ++i)
		{

			float theta = (i*0.5-5) / 180 * 3.1415;
			float x = cos( theta ) * m_DIST[i];
			float y = sin( theta ) * m_DIST[i];

			m_XYDist[0][i] = x;
			m_XYDist[1][i] = y;

		}
	}		
	
	ListBoxManage();
	
	CSocket::OnReceive(nErrorCode);
}

/* memo

//char str[1000];
int s=0;
CString str;
for(int i=0; i<bytesRead; ++i)
{

//if( szBuffer[i] == 0x20)
//{
//	((CLMS511_interfaceDlg*)pParent)->m_ListBox.AddString(str);
//	str = "";
//}		
str.Format("%x - %c",szBuffer[i],szBuffer[i]);
((CLMS511_interfaceDlg*)pParent)->m_ListBox.AddString(str);

s++;
if(szBuffer[i] == 0x20 )
{
CString a;
a.Format("len = %d",s);
((CLMS511_interfaceDlg*)pParent)->m_ListBox.AddString(a);
((CLMS511_interfaceDlg*)pParent)->m_ListBox.AddString("--------");
s=0;
}


}
*/


int CMySyncSocket::ServerOn(int Port)
{

	if( !Create(Port, SOCK_STREAM)) //SOCK_DGRAM)) //
	{
		((CLMS511_interfaceDlg*)pParent)->m_ListBox.AddString("Socket Create Error");
	}else{
		((CLMS511_interfaceDlg*)pParent)->m_ListBox.AddString("Socket Create Success");
	}

	return 0;
}


int CMySyncSocket::ParentPoint(void* parent)
{
	pParent = parent;
	return 0;
}


int CMySyncSocket::ServerConnect(CString ServerIP, int ServerPort)
{

	CString tempStr;
	tempStr.Format("Connect Success - (Server IP:%s, Port:%d)", ServerIP, ServerPort);
	

	if( Connect(ServerIP, ServerPort) == SOCKET_ERROR )
	{
		((CLMS511_interfaceDlg*)pParent)->m_ListBox.AddString("Connect Error");
		return 0;
	}else{
		((CLMS511_interfaceDlg*)pParent)->m_ListBox.AddString(tempStr);
		return 1;
	}

	return 1;

}


int CMySyncSocket::SendData(CString SendStr)
{
	//데이터 보내기
	int bytesSent;
	
	char SendMsg[100];
	sprintf(SendMsg, "%c%s%c", 0x02, SendStr, 0x03);
	
	CString tempStr;
	
	if( (bytesSent = Send(SendMsg, strlen(SendMsg))) == SOCKET_ERROR ) 
		((CLMS511_interfaceDlg*)pParent)->m_ListBox.AddString("Send Error");
	else
	{
		tempStr.Format("Send Success - (%s - %dbytes)", SendMsg, bytesSent);
		((CLMS511_interfaceDlg*)pParent)->m_ListBox.AddString(tempStr);	
	}
	
	return 0;
}


int CMySyncSocket::ListBoxManage(void)
{
	int nCount = ((CLMS511_interfaceDlg*)pParent)->m_ListBox.GetCount();
	if( nCount > 1000)
		((CLMS511_interfaceDlg*)pParent)->m_ListBox.ResetContent();

	if(nCount> 0)
		((CLMS511_interfaceDlg*)pParent)->m_ListBox.SetCurSel(nCount-1);

	return 0;
}

int CMySyncSocket::SocketClose(void)
{

	//Disconnect();
	ShutDown();
	Close();

	return 0;
}

UINT DataGetThread(LPVOID pParam)
{
	((CMySyncSocket*)pParam)->GetReceiveData();

	return 0;
}
 

int CMySyncSocket::GetReceiveData(void)
{
	((CLMS511_interfaceDlg*)pParent)->m_ListBox.AddString("Receive");

	CString strIPAddress,tmpStr;
	UINT uPortNumber=0;
	char szBuffer[2048] = {0,};

	int bytesRead=0;
	if( (bytesRead = ReceiveFrom(szBuffer, 2048, (CString&)strIPAddress, (UINT&)uPortNumber)) == SOCKET_ERROR)
	{
		tmpStr.Format("[%s:%d] - Receive Error", strIPAddress, uPortNumber);
		((CLMS511_interfaceDlg*)pParent)->m_ListBox.AddString(tmpStr);
		//::AfxMessageBox(tmpStr);

	}else{
		tmpStr.Format("[%s:%d:%dbytes] : %s", strIPAddress, uPortNumber, bytesRead, szBuffer);
		((CLMS511_interfaceDlg*)pParent)->m_ListBox.AddString(tmpStr);
	}


	/*
	//char str[1000];
	int s=0;
	CString str;
	for(int i=0; i<bytesRead; ++i)
	{

		//if( szBuffer[i] == 0x20)
		//{
		//	((CLMS511_interfaceDlg*)pParent)->m_ListBox.AddString(str);
		//	str = "";
		//}		
		str.Format("%x - %c",szBuffer[i],szBuffer[i]);
		((CLMS511_interfaceDlg*)pParent)->m_ListBox.AddString(str);

		s++;
		if(szBuffer[i] == 0x20 )
		{
			CString a;
			a.Format("len = %d",s);
			((CLMS511_interfaceDlg*)pParent)->m_ListBox.AddString(a);
			((CLMS511_interfaceDlg*)pParent)->m_ListBox.AddString("--------");
			s=0;
		}
	}
	*/

	return 0;
}
