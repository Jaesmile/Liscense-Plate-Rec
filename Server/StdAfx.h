/**************************************************************************************************
* File Name  : StdAfx.h                                                                           *
* Created   : 15/03/23                                                                            *                                                                                     *
* Model   : 05YOP                                                                                 *
* Description  : MFC自动生成头文件                                                                *
**************************************************************************************************/


#if !defined(AFX_STDAFX_H__6795701F_D327_41A9_B456_B25D36523BA3__INCLUDED_)
#define AFX_STDAFX_H__6795701F_D327_41A9_B456_B25D36523BA3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT


#include "ServerSocket.h"
#include "SkinPPWTL.h"
#pragma comment(lib,"SkinPPWTL.lib")

extern CString g_curUser;
extern CString g_picPath;
extern SOCKET g_serSocket;
extern HWND g_hwnd;
extern CString g_refresh;

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__6795701F_D327_41A9_B456_B25D36523BA3__INCLUDED_)
