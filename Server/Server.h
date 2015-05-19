/**************************************************************************************************
* File Name  : Server.h                                                                           *
* Created   : 15/03/23                                                                            *
* Author   :                                                                                      *
* Model   : 05YOP                                                                                 *
* Description  :   项目入口类头文件                                                               *
**************************************************************************************************/
#if !defined(AFX_SERVER_H__2C550F85_D746_439D_8A56_A7DADAD087C5__INCLUDED_)
#define AFX_SERVER_H__2C550F85_D746_439D_8A56_A7DADAD087C5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CServerApp:
// See Server.cpp for the implementation of this class

class CServerApp : public CWinApp
{
public:
	CServerApp();

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CServerApp)
public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

	// Implementation

	//{{AFX_MSG(CServerApp)
	// NOTE - the ClassWizard will add and remove member functions here.
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SERVER_H__2C550F85_D746_439D_8A56_A7DADAD087C5__INCLUDED_)
