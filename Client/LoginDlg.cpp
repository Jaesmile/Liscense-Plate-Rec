// LoginDlg.cpp : implementation file

#include "stdafx.h"
#include "Client.h"
#include "LoginDlg.h"
#include "SettingDialog.h"
#include "RegisterDialog.h"
#include "MainDialog.h"
#include "ClientSocket.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

	// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// LogintDlg dialog

LogintDlg::LogintDlg(CWnd* pParent /*=NULL*/)
	: CDialog(LogintDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(LogintDlg)
	m_userName = _T("");
	m_password = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void LogintDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(LogintDlg)
	DDX_Text(pDX, IDC_EDIT1, m_userName);
	DDX_Text(pDX, IDC_EDIT2, m_password);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(LogintDlg, CDialog)
	//{{AFX_MSG_MAP(LogintDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_SETTING, OnButtonSetting)
	ON_BN_CLICKED(IDC_BUTTON_REGISTER, OnButtonRegister)
	ON_BN_CLICKED(IDC_BUTTON_LOGIN, OnButtonLogin)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// LogintDlg message handlers

BOOL LogintDlg::OnInitDialog()
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


	CStdioFile file("网络参数.txt", CFile::modeRead);
	file.ReadString(port);
	file.ReadString(IPadd);
	file.Close();

	// TODO: Add extra initialization here
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void LogintDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void LogintDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM)dc.GetSafeHdc(), 0);

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
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR LogintDlg::OnQueryDragIcon()
{
	return (HCURSOR)m_hIcon;
}

void LogintDlg::OnButtonSetting()
{
	// TODO: Add your control notification handler code here
	SettingDialog setting;
	setting.DoModal();
}

void LogintDlg::OnButtonRegister()
{
	// TODO: Add your control notification handler code here
	RegisterDialog reg;
	ShowWindow(SW_HIDE);
	reg.DoModal();
	ShowWindow(SW_NORMAL);
}

void LogintDlg::OnButtonLogin()
{

	UpdateData(TRUE);

	//初始化网络参数
	//判断用户是否输入用户名或者密码
	if (m_userName == "\0" || m_password == "\0")
	{
		MessageBox("请输入用户名或者密码", NULL, MB_OK);
		return;
	}

	//创建套接字
	ClientSocket socket;
	if (socket.CreateSocket(IPadd, port) == 0)
	{
		//SetTimer(1,3000,NULL);
		socket.SendTextToServer("LOGIN#", m_userName, m_password);
		if (socket.ReceiveTextFromServer())
		{
			//KillTimer(1);
			ShowWindow(SW_HIDE);
			MainDialog main;
			main.DoModal();
			OnOK();
		}
		socket.CloseSocket();
	}
	else
	{
		AfxMessageBox("与服务器连接失败!");
	}
}

void LogintDlg::OnTimer(UINT nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if (nIDEvent == 1)
	{
		KillTimer(1);
		MessageBox("连接超时");
		exit(0);

	}
	CDialog::OnTimer(nIDEvent);
}
