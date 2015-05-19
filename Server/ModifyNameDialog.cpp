/**************************************************************************************************
* File Name  : ModifyNameDialog.cpp                                                               *
* Created   : 15/03/24                                                                            *
* Author   : He Hao                                                                               *
* Model   : 05YOP                                                                                 *
* Description  :   修改用户名对话框                                                               *
**************************************************************************************************/
#include "stdafx.h"
#include "Server.h"
#include "ModifyNameDialog.h"
#include "ReceiverInfoRecordset.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ModifyNameDialog dialog

ModifyNameDialog::ModifyNameDialog(CWnd* pParent /*=NULL*/)
	: CDialog(ModifyNameDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(ModifyNameDialog)
	m_edit_newName = _T("");
	//}}AFX_DATA_INIT
}

void ModifyNameDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(ModifyNameDialog)
	DDX_Text(pDX, IDC_EDIT_NEWNAME, m_edit_newName);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(ModifyNameDialog, CDialog)
	//{{AFX_MSG_MAP(ModifyNameDialog)
	ON_BN_CLICKED(IDC_BUTTON_MODIFY, OnButtonModify)
	ON_BN_CLICKED(IDC_CANCEL, OnCancel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// ModifyNameDialog message handlers

void ModifyNameDialog::OnButtonModify()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if (m_edit_newName.IsEmpty())
	{
		MessageBox("用户名不可为空.");
	}
	else
	{
		ReceiverInfoRecordset set;
		set.ModifyRecord(g_curUser, m_edit_newName);
		g_curUser = m_edit_newName;
		OnOK();
	}
}

void ModifyNameDialog::OnCancel()
{
	// TODO: Add your control notification handler code here
	OnOK();
}
