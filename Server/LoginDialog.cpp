/**************************************************************************************************
* File Name  : LoginDialog.cpp                                                                    *
* Created   : 15/03/23                                                                            *
* Author   : He Hao                                                                               *
* Model   : 05YOP                                                                                 *
* Description  :   登陆对话框                                                                     *
**************************************************************************************************/

#include "stdafx.h"
#include "Server.h"
#include "LoginDialog.h"
#include "RegisterDialog.h"
#include "MainDialog.h"
#include "ReceiverInfoRecordset.h"

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
// LoginDialog dialog

LoginDialog::LoginDialog(CWnd* pParent /*=NULL*/)
	: CDialog(LoginDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(LoginDialog)
	m_edit_username = _T("");
	m_edit_passwd = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void LoginDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(LoginDialog)
	DDX_Text(pDX, IDC_EDIT1, m_edit_username);
	DDX_Text(pDX, IDC_EDIT2, m_edit_passwd);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(LoginDialog, CDialog)
	//{{AFX_MSG_MAP(LoginDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_REGISTER, OnButtonRegister)
	ON_BN_CLICKED(IDC_BUTTON_LOGIN, OnButtonLogin)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// LoginDialog message handlers

BOOL LoginDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

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

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void LoginDialog::OnSysCommand(UINT nID, LPARAM lParam)
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

void LoginDialog::OnPaint()
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
HCURSOR LoginDialog::OnQueryDragIcon()
{
	return (HCURSOR)m_hIcon;
}

/************************************************************************************************
* Function Name : OnButtonRegister                                                              *
* Description  : 注册按钮的响应函数，完成打开注册界面操作                                       *
* Date   : 15/03/23                                                                             *
* Parameter     : 无                                                                            *
* Return Code  : void                                                                           *
* Author   :  He Hao                                                                            *
************************************************************************************************/
void LoginDialog::OnButtonRegister()
{
	//创建注册窗口类并将其打开
	RegisterDialog registerdialog;
	registerdialog.DoModal();
}

/************************************************************************************************
* Function Name : OnButtonLogin                                                                 *
* Description  :  登陆按钮响应函数,完成登陆操作                                                 *
* Date   : 15/03/23                                                                             *
* Parameter     : 无                                                                            *
* Return Code  : void                                                                           *
* Author   : He Hao                                                                             *
************************************************************************************************/
void LoginDialog::OnButtonLogin()
{
	UpdateData(TRUE);

	//将用户名与密码与数据库进行匹配来完成登陆操作

	ReceiverInfoRecordset receiverSet;
	if (receiverSet.MatchRecord(m_edit_username, m_edit_passwd))
	{
		g_curUser = m_edit_username;
		ShowWindow(SW_HIDE);
		MainDialog maindialog;
		maindialog.DoModal();
		OnOK();
	}
}
