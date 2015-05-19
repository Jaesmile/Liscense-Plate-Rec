/**************************************************************************************************
* File Name  : DelDialog.cpp                                                                      *
* Created   : 15/3/23                                                                             *
* Author   :  He Hao                                                                              *
* Model   : 05YOP                                                                                 *
* Description  : DelDialog类成员函数的定义                                                        *
**************************************************************************************************/

#include "stdafx.h"
#include "Server.h"
#include "DelDialog.h"
#include "ViolationRecordset.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DelDialog dialog


DelDialog::DelDialog(CWnd* pParent /*=NULL*/)
	: CDialog(DelDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(DelDialog)
	// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void DelDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DelDialog)
	// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DelDialog, CDialog)
	//{{AFX_MSG_MAP(DelDialog)
	ON_BN_CLICKED(IDC_BUTTON_CONFIRM_DEL, OnButtonConfirmDel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DelDialog message handlers


/*******************************************************************************************
* Function Name : OnButtonConfirmDel                                                       *
* Description  :  Delete record from database                                              *
* Date   : 03/3/20                                                                         *
* Parameter     : 无                                                                       *
* Return Code  :  void                                                                     *
* Author   :   He Hao                                                                      *
*******************************************************************************************/
void DelDialog::OnButtonConfirmDel()
{
	ViolationRecordset record;                        //创建ViolationRecordset类型的变量record
	record.DelRecord(m_plateNum, m_shootDate);  //调用record的成员函数DelRecord删除数据
	g_refresh="REFRESH";
	OnOK();                                           //退出
}
