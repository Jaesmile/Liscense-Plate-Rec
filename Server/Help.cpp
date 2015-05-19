/**************************************************************************************************
* File Name  : Help.cpp                                                                           *
* Created   : 03/08/29                                                                            *
* Author   : Hu xin ru                                                                            *
* Model   : 05YOP                                                                                 *
* Description  :   ∞Ô÷˙ΩÁ√Ê                                                                       *
**************************************************************************************************/

#include "stdafx.h"
#include "Server.h"
#include "Help.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Help dialog

Help::Help(CWnd* pParent /*=NULL*/)
	: CDialog(Help::IDD, pParent)
{
	//{{AFX_DATA_INIT(Help)
	// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

void Help::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(Help)
	// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(Help, CDialog)
	//{{AFX_MSG_MAP(Help)
	ON_BN_CLICKED(IDC_OK, OnOk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Help message handlers

void Help::OnOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}
