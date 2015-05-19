// RegisterDialog.cpp : implementation file

#include "stdafx.h"
#include "Client.h"
#include "RegisterDialog.h"
#include "LoginDlg.h"
#include "ClientSocket.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// RegisterDialog dialog

RegisterDialog::RegisterDialog(CWnd *pParent /*=NULL*/)
	: CDialog(RegisterDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(RegisterDialog)
	m_edit_userName = _T("");
	m_edit_password = _T("");
	m_edit_confirmPasswd = _T("");
	//}}AFX_DATA_INIT
}

void RegisterDialog::DoDataExchange(CDataExchange *pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(RegisterDialog)
	DDX_Text(pDX, IDC_EDIT1, m_edit_userName);
	DDX_Text(pDX, IDC_EDIT2, m_edit_password);
	DDX_Text(pDX, IDC_EDIT3, m_edit_confirmPasswd);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(RegisterDialog, CDialog)
	//{{AFX_MSG_MAP(RegisterDialog)
	ON_BN_CLICKED(IDC_BUTTON_REGISTER, OnButtonRegister)
	ON_BN_CLICKED(IDC_BUTTON_RETURN, OnButtonReturn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// RegisterDialog message handlers

void RegisterDialog::OnButtonRegister()
{

	UpdateData(TRUE);
	if (m_edit_password != m_edit_confirmPasswd)
	{
		MessageBox("两次密码输入不一致");
		return;
	}

	ClientSocket socket;
	if (socket.CreateSocket(IPadd, port) == 0)
	{
		socket.SendTextToServer("REGIS#", m_edit_userName, m_edit_password);
		if (socket.ReceiveTextFromServer())
		{
			OnOK();
		}
		socket.CloseSocket();
	}
	else
	{
		AfxMessageBox("与服务器连接失败!");
	}
}

void RegisterDialog::OnButtonReturn()
{

	LogintDlg log;
	OnOK();
	log.DoModal();
}
