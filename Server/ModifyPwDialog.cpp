/**************************************************************************************************
* File Name  : ModifyPwDialog.cpp                                                                 *
* Created   : 03/08/29                                                                            *
* Author   : He Hao                                                                               *
* Model   : 05YOP                                                                                 *
* Description  :   修改密码对话框                                                                 *
**************************************************************************************************/

#include "stdafx.h"
#include "Server.h"
#include "ModifyPwDialog.h"
#include "ReceiverInfoRecordset.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ModifyPwDialog dialog

ModifyPwDialog::ModifyPwDialog(CWnd* pParent /*=NULL*/)
	: CDialog(ModifyPwDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(ModifyPwDialog)
	m_edit_newPw = _T("");
	m_edit_oldPw = _T("");
	m_edit_confirmPw = _T("");
	//}}AFX_DATA_INIT
}

void ModifyPwDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(ModifyPwDialog)
	DDX_Text(pDX, IDC_EDIT_NEWPW, m_edit_newPw);
	DDX_Text(pDX, IDC_EDIT_OLDPW, m_edit_oldPw);
	DDX_Text(pDX, IDC_EDIT_CONFIRMPW, m_edit_confirmPw);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(ModifyPwDialog, CDialog)
	//{{AFX_MSG_MAP(ModifyPwDialog)
	ON_BN_CLICKED(IDC_MODIFYPASSWD_BUTTON, OnModifypasswdButton)
	ON_BN_CLICKED(IDC_CANCEL, OnCancel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// ModifyPwDialog message handlers

void ModifyPwDialog::OnModifypasswdButton()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	ReceiverInfoRecordset set;
	if (set.MatchRecord(g_curUser, m_edit_oldPw))
	{
		if (m_edit_confirmPw == m_edit_newPw)
		{
			set.ModifyRecord(g_curUser, m_edit_oldPw, m_edit_newPw);
		}
		else
			AfxMessageBox("两次新密码输入不一致");
	}
	else
	{
		AfxMessageBox("旧密码错误");
	}
	OnOK();
}

void ModifyPwDialog::OnCancel()
{
	// TODO: Add your control notification handler code here
	OnOK();
}
