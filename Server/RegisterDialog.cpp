/**************************************************************************************************
* File Name  : RegisterDialog.cpp                                                                 *
* Created   : 15/03/23                                                                            *
* Author   : He Hao                                                                               *
* Model   : 05YOP                                                                                 *
* Description  :   注册对话框类                                                                   *
**************************************************************************************************/
#include "stdafx.h"
#include "Server.h"
#include "RegisterDialog.h"
#include "ReceiverInfoRecordset.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// RegisterDialog dialog

RegisterDialog::RegisterDialog(CWnd* pParent /*=NULL*/)
	: CDialog(RegisterDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(RegisterDialog)
	m_edit_username = _T("");
	m_edit_passwd = _T("");
	m_edit_confirmPW = _T("");
	//}}AFX_DATA_INIT
}

void RegisterDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(RegisterDialog)
	DDX_Text(pDX, IDC_EDIT1, m_edit_username);
	DDX_Text(pDX, IDC_EDIT2, m_edit_passwd);
	DDX_Text(pDX, IDC_EDIT3, m_edit_confirmPW);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(RegisterDialog, CDialog)
	//{{AFX_MSG_MAP(RegisterDialog)
	ON_BN_CLICKED(IDC_BUTTON_CONFIRM_REGISTER, OnButtonConfirmRegister)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// RegisterDialog message handlers

/*******************************************************************************************
* Function Name : OnButtonConfirmRegister                                                  *
* Description  : 确认注册                                                                  *
* Date   : 03/3/20                                                                         *
* Parameter     : 无                                                                       *
* Return Code  :  void                                                                     *
* Author   :    He  hao /Huang rong                                                        *
*******************************************************************************************/
void RegisterDialog::OnButtonConfirmRegister()
{
	
	UpdateData(TRUE);
	if (m_edit_confirmPW != m_edit_passwd)
	{
		AfxMessageBox("两次输入密码不一致");
		return;
	}

	ReceiverInfoRecordset receiverSet;
	receiverSet.AddRecord(m_edit_username, m_edit_passwd);

	UpdateData(FALSE);
	OnOK();
}
