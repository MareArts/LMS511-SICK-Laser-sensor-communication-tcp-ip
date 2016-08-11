// LMS511_interfaceDlg.h : header file
//

#pragma once
#include "afxcmn.h"
#include "MySyncSocket.h"
#include <string>
#include "afxwin.h"

using namespace std;

// CLMS511_interfaceDlg dialog
class CLMS511_interfaceDlg : public CDialog
{
// Construction
public:
	CLMS511_interfaceDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_LMS511_INTERFACE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnIpnFieldchangedIpaddress1(NMHDR *pNMHDR, LRESULT *pResult);
	CIPAddressCtrl m_IpAdd_LMS511;
	afx_msg void OnBnClickedSendMessage();
	DWORD m_MyPort;
	afx_msg void OnBnClickedServerOn();

	CString m_MyMsg;
	CListBox m_ListBox;
	UINT m_LMS511Port;


	CMySyncSocket MySSocket;


	afx_msg void OnBnClickedScanDataOn();
	afx_msg void OnBnClickedScanDataOff();
	afx_msg void OnBnClickedConnect();
	afx_msg void OnBnClickedSocketOff();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	int DrawData(void);
	CStatic m_Picture;

	int PicW;
	int PicH;
	int PicXS;
	int PicYS;

//	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedZoomIn();
	afx_msg void OnBnClickedZoomOut();
};
