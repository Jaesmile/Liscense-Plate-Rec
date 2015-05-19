// SettingDialog.cpp : implementation file

#include "stdafx.h"
#include "Client.h"
#include "SettingDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// SettingDialog dialog

SettingDialog::SettingDialog(CWnd* pParent /*=NULL*/)
	: CDialog(SettingDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(SettingDialog)
	m_port = _T("");
	//}}AFX_DATA_INIT
}

void SettingDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(SettingDialog)
	DDX_Control(pDX, IDC_IPADDRESS1, m_IPAddress);
	DDX_Text(pDX, IDC_EDIT_PORT, m_port);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(SettingDialog, CDialog)
	//{{AFX_MSG_MAP(SettingDialog)
	ON_BN_CLICKED(IDC_BUTTON_OK, OnButtonOk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// SettingDialog message handlers

void SettingDialog::OnButtonOk()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	BYTE IP0, IP1, IP2, IP3;
	m_IPAddress.GetAddress(IP0, IP1, IP2, IP3);
	IPadd.Format(_T("%d.%d.%d.%d"), IP0, IP1, IP2, IP3);

	port = m_port;
	CStdioFile file("网络参数.txt", CFile::modeReadWrite);
	file.WriteString(port + "\r\n");
	file.WriteString(IPadd + "\r\n");
	file.Flush();
	file.Close();

	MessageBox("IP地址为:" + IPadd + "\t\n" + "端口号为:" + port, "网络参数");
	OnOK();
}

BOOL SettingDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	CStdioFile file("网络参数.txt", CFile::modeRead);

	file.ReadString(m_port);
	m_port = m_port.Left(m_port.GetLength() - 1);

	file.ReadString(IPadd);
	m_IPAddress.SetWindowText(IPadd);

	UpdateData(FALSE);

	file.Close();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
