/**************************************************************************************************
* File Name  : SettingTab.cpp                                                                     *
* Created   : 15/03/23                                                                            *
* Author   : He Hao                                                                               *
* Model   : 05YOP                                                                                 *
* Description  :   设置子窗口                                                                     *
**************************************************************************************************/

#include "stdafx.h"
#include "Server.h"
#include "SettingTab.h"
#include "ReceiverInfoRecordset.h"
#include "ModifyNameDialog.h"
#include "LoginDialog.h"
#include "ModifyPwDialog.h"
#include "SenderInfoRecordset.h"
#include "Help.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// SettingTab dialog

SettingTab::SettingTab(CWnd* pParent /*=NULL*/)
	: CDialog(SettingTab::IDD, pParent)
{
	//{{AFX_DATA_INIT(SettingTab)
	m_edit_curUser = _T("");
	//}}AFX_DATA_INIT
}

void SettingTab::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(SettingTab)
	DDX_Control(pDX, IDC_REGISTER_LIST, m_registerList);
	DDX_Text(pDX, IDC_EDIT_CURRENT_USERNAME, m_edit_curUser);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(SettingTab, CDialog)
	//{{AFX_MSG_MAP(SettingTab)
	ON_BN_CLICKED(IDC_BUTTON_CHANGE_USERNAME, OnButtonChangeUsername)
	ON_BN_CLICKED(IDC_BUTTON_CHANGE_PASSWARD, OnButtonChangePassward)
	ON_BN_CLICKED(IDC_BUTTON_APPROVE, OnButtonApprove)
	ON_BN_CLICKED(IDC_BUTTON_REFUSE, OnButtonRefuse)
	ON_BN_CLICKED(IDC_BUTTON_HELP, OnButtonHelp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// SettingTab message handlers

BOOL SettingTab::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here

	m_edit_curUser = g_curUser;
	UpdateData(FALSE);

	m_registerList.InsertColumn(0, "注册用户名", LVCFMT_LEFT, 120, 0);
	m_registerList.InsertColumn(1, "注册密码", LVCFMT_LEFT, 150, 0);
	m_registerList.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

/*******************************************************************************************
* Function Name : OnButtonChangeUsername                                                   *
* Description  :  更改用户名                                                               *
* Date   : 03/3/20                                                                         *
* Parameter     : 无                                                                       *
* Return Code  :  void                                                                     *
* Author   :    He  hao                                                                    *
*******************************************************************************************/
void SettingTab::OnButtonChangeUsername()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	ModifyNameDialog modifyDialog;
	modifyDialog.DoModal();
	m_edit_curUser = g_curUser;
	UpdateData(FALSE);
}

/*******************************************************************************************
* Function Name : OnButtonChangePassward                                                   *
* Description  :  更改密码                                                                 *
* Date   : 03/3/20                                                                         *
* Parameter     : 无                                                                       *
* Return Code  :  void                                                                     *
* Author   :    He  hao                                                                    *
*******************************************************************************************/
void SettingTab::OnButtonChangePassward()
{
	// TODO: Add your control notification handler code here
	ModifyPwDialog pwDialog;
	pwDialog.DoModal();
}

/*******************************************************************************************
* Function Name : OnButtonApprove                                                          *
* Description  :  批准注册                                                                 *
* Date   : 03/3/20                                                                         *
* Parameter     : 无                                                                       *
* Return Code  :  void                                                                     *
* Author   :    He  hao/ Huang rong                                                        *
*******************************************************************************************/
void SettingTab::OnButtonApprove()
{
	POSITION pos = m_registerList.GetFirstSelectedItemPosition();
	if (pos != NULL)
	{
		int index = m_registerList.GetNextSelectedItem(pos);
		CString registerName = m_registerList.GetItemText(index, 0);
		CString registerPasswd = m_registerList.GetItemText(index, 1);
		SenderInfoRecordset set;
		set.AddRecord(registerName, registerPasswd);
		m_registerList.DeleteItem(index);
	}
	else
	{
		MessageBox("请选择待批准用户");
	}


}

/*******************************************************************************************
* Function Name : OnButtonRefuse                                                           *
* Description  :  拒绝注册                                                                 *
* Date   : 03/3/20                                                                         *
* Parameter     : 无                                                                       *
* Return Code  :  void                                                                     *
* Author   :    He  hao                                                                    *
*******************************************************************************************/
void SettingTab::OnButtonRefuse()
{
	POSITION pos = m_registerList.GetFirstSelectedItemPosition();
	if (pos != NULL)
	{
		int index = m_registerList.GetNextSelectedItem(pos);
		m_registerList.DeleteItem(index);
	}
	else
	{
		MessageBox("请选择待批准用户");
	}
}

/*******************************************************************************************
* Function Name : OnButtonHelp                                                             *
* Description  :  弹出帮助对话框                                                           *
* Date   : 03/3/20                                                                         *
* Parameter     : 无                                                                       *
* Return Code  :  void                                                                     *
* Author   :    He  hao                                                                    *
*******************************************************************************************/
void SettingTab::OnButtonHelp()
{
	// TODO: Add your control notification handler code here
	Help help;
	help.DoModal();
}
