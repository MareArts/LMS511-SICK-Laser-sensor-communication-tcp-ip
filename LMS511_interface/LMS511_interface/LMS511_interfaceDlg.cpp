// LMS511_interfaceDlg.cpp : implementation file
//

#include "stdafx.h"
#include "LMS511_interface.h"
#include "LMS511_interfaceDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CLMS511_interfaceDlg dialog




CLMS511_interfaceDlg::CLMS511_interfaceDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLMS511_interfaceDlg::IDD, pParent)
	, m_MyPort(0)
	, m_MyMsg(_T(""))
	, m_LMS511Port(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CLMS511_interfaceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IPADDRESS1, m_IpAdd_LMS511);
	DDX_Text(pDX, IDC_EDIT2, m_MyPort);
	DDV_MinMaxUInt(pDX, m_MyPort, 0, 10000);
	DDX_Text(pDX, IDC_SEND_TEXT, m_MyMsg);
	DDX_Control(pDX, IDC_LIST1, m_ListBox);
	DDX_Text(pDX, IDC_LMS511_PORT, m_LMS511Port);
	DDV_MinMaxUInt(pDX, m_LMS511Port, 0, 99999);
	DDX_Control(pDX, IDC_PICTURE, m_Picture);
}

BEGIN_MESSAGE_MAP(CLMS511_interfaceDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
//	ON_NOTIFY(IPN_FIELDCHANGED, IDC_IPADDRESS1, &CLMS511_interfaceDlg::OnIpnFieldchangedIpaddress1)
ON_BN_CLICKED(IDC_SEND_MESSAGE, &CLMS511_interfaceDlg::OnBnClickedSendMessage)
ON_BN_CLICKED(IDC_SERVER_ON, &CLMS511_interfaceDlg::OnBnClickedServerOn)
ON_BN_CLICKED(IDC_SCAN_DATA_ON, &CLMS511_interfaceDlg::OnBnClickedScanDataOn)
ON_BN_CLICKED(IDC_SCAN_DATA_OFF, &CLMS511_interfaceDlg::OnBnClickedScanDataOff)
ON_BN_CLICKED(IDC_CONNECT, &CLMS511_interfaceDlg::OnBnClickedConnect)
ON_BN_CLICKED(IDC_SOCKET_OFF, &CLMS511_interfaceDlg::OnBnClickedSocketOff)
ON_WM_TIMER()
//ON_BN_CLICKED(IDC_BUTTON1, &CLMS511_interfaceDlg::OnBnClickedButton1)
ON_BN_CLICKED(IDC_ZOOM_IN, &CLMS511_interfaceDlg::OnBnClickedZoomIn)
ON_BN_CLICKED(IDC_ZOOM_OUT, &CLMS511_interfaceDlg::OnBnClickedZoomOut)
END_MESSAGE_MAP()


// CLMS511_interfaceDlg message handlers

BOOL CLMS511_interfaceDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	m_MyPort = 3000;

	m_IpAdd_LMS511.SetAddress(169, 254, 178, 7);
	m_LMS511Port = 2111;
	UpdateData(false);

	MySSocket.ParentPoint(this);

	CRect rect;
	m_Picture.GetWindowRect(rect);
	PicW = rect.Width();
	PicH = rect.Height();
	PicXS = 10;
	PicYS = 10;


	

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CLMS511_interfaceDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CLMS511_interfaceDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		
		CDialog::OnPaint();

		DrawData();

	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CLMS511_interfaceDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//void CLMS511_interfaceDlg::OnIpnFieldchangedIpaddress1(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	LPNMIPADDRESS pIPAddr = reinterpret_cast<LPNMIPADDRESS>(pNMHDR);
//	// TODO: Add your control notification handler code here
//	*pResult = 0;
//}

void CLMS511_interfaceDlg::OnBnClickedSendMessage()
{
	// TODO: Add your control notification handler code here
	
	//////////////////////////////////////////////////////////////////////////////////////////////
	UpdateData(true);

	MySSocket.SendData(m_MyMsg);

}

void CLMS511_interfaceDlg::OnBnClickedServerOn()
{
	// TODO: Add your control notification handler code here

	UpdateData(true);	
	MySSocket.ServerOn(m_MyPort);

}

void CLMS511_interfaceDlg::OnBnClickedScanDataOn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	MySSocket.SendData("sEN LMDscandata 1");
	//MySSocket.SendData("sRN LMDscandata");

}

void CLMS511_interfaceDlg::OnBnClickedScanDataOff()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	MySSocket.SendData("sEN LMDscandata 0");
}

void CLMS511_interfaceDlg::OnBnClickedConnect()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(true);
	BYTE A, B, C, D;
	m_IpAdd_LMS511.GetAddress(A,B,C,D);
	CString ServerIP;
	ServerIP.Format("%d.%d.%d.%d", A, B, C, D);
	
	if( MySSocket.ServerConnect(ServerIP, m_LMS511Port) == 1)
		SetTimer(100, 100, 0);
	//MySSocket.SendData(sendMsg, m_LMS511Port, YourIP);


}

void CLMS511_interfaceDlg::OnBnClickedSocketOff()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.


	MySSocket.SocketClose();
	KillTimer(100);

}

void CLMS511_interfaceDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default

	if(nIDEvent == 100)
	{
		DrawData();
	}

	CDialog::OnTimer(nIDEvent);
}

int CLMS511_interfaceDlg::DrawData(void)
{

	
	CDC* pDC = m_Picture.GetDC();		
	CDC memDC; 		
	CBitmap bmp;		
	memDC.CreateCompatibleDC(pDC);		
	bmp.CreateCompatibleBitmap( pDC, PicW, PicH);		
	memDC.SelectObject(bmp);

	memDC.SelectStockObject(WHITE_BRUSH);
	memDC.SelectStockObject(WHITE_PEN);

	pDC->SelectStockObject(WHITE_BRUSH);
	pDC->SelectStockObject(WHITE_PEN);


	int yGap = 10;
	memDC.Ellipse( PicW/2-5, PicH-yGap-5, PicW/2+5, PicH-yGap+5);
	int x,y;

	int init=0;
	CString a;
	for(int i=0; i<381; ++i)
	{
		if( MySSocket.m_XYDist[1][i]==0)
			continue;

		x = (PicW/2.0) +( MySSocket.m_XYDist[0][i] / PicXS);
		y = PicH-yGap - ( MySSocket.m_XYDist[1][i] / PicYS);

		if(init==0)
		{
			memDC.MoveTo(x,y);
			init=1;
		}

		

		memDC.Ellipse(x-2,y-2,x+2,y+2);
		memDC.LineTo(x,y);
		memDC.SetPixel( x, y, RGB(255,0,0));			
	}
	//memDC.LineTo(x,y);

	pDC->StretchBlt(0,0,PicW,PicH,&memDC,0,0,PicW,PicH,SRCCOPY); 		
	bmp.DeleteObject();
	memDC.DeleteDC();
	ReleaseDC(pDC);


	return 0;
}

//void CLMS511_interfaceDlg::OnBnClickedButton1()
//{
//	// TODO: Add your control notification handler code here
//}

void CLMS511_interfaceDlg::OnBnClickedZoomIn()
{
	// TODO: Add your control notification handler code here

	

	if(PicXS != 1){
		PicXS = PicXS-1;
		PicYS = PicYS-1;
	}

}

void CLMS511_interfaceDlg::OnBnClickedZoomOut()
{
	// TODO: Add your control notification handler code here

	PicXS = PicXS+1;
	PicYS = PicYS+1;
	

}
