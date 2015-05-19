/**************************************************************************************************
* File Name  : DelPicture.cpp                                                                     *
* Created   : 03/08/29                                                                            *
* Author   : He Hao                                                                               *
* Model   : 05YOP                                                                                 *
* Description  :   删除图片                                                                       *
**************************************************************************************************/

#include "stdafx.h"
#include "Server.h"
#include "DelPicture.h"

#define VM_MYRECEVIEDPICMSG 102
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DelPicture dialog

DelPicture::DelPicture(CWnd* pParent /*=NULL*/)
	: CDialog(DelPicture::IDD, pParent)
{
	//{{AFX_DATA_INIT(DelPicture)
	// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

void DelPicture::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DelPicture)
	// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(DelPicture, CDialog)
	//{{AFX_MSG_MAP(DelPicture)
	ON_BN_CLICKED(IDC_BUTTON1, OnButtonConfirm)
	ON_BN_CLICKED(IDC_BUTTON2, OnButtonCancle)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DelPicture message handlers

/*******************************************************************************************
* Function Name : OnButtonConfirmDel                                                       *
* Description  :  Delete picture                                                           *
* Date   : 03/3/20                                                                         *
* Parameter     : 无                                                                       *
* Return Code  :  void                                                                     *
* Author   :   He Hao                                                                      *
*******************************************************************************************/
void DelPicture::OnButtonConfirm()
{
	DeleteFile(g_picPath);
	g_refresh="REFRESH";   //更新全局变量
	OnOK();

}


/*******************************************************************************************
* Function Name : OnButtonCancle                                                           *
* Description  :  Quit                                                                     *
* Date   : 03/3/20                                                                         *
* Parameter     : 无                                                                       *
* Return Code  :  void                                                                     *
* Author   :   He Hao                                                                      *
*******************************************************************************************/
void DelPicture::OnButtonCancle() 
{
	OnOK();
	
}
