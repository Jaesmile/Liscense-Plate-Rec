/**************************************************************************************************
* File Name  : LoginDialog.h                                                                      *
* Created   : 15/03/23                                                                            *
* Author   : He Hao                                                                               *
* Model   : 05YOP                                                                                 *
* Description  :   登陆界面头文件                                                                 *
**************************************************************************************************/

#if !defined(AFX_LOGINDIALOG_H__D5E2DF52_E0A2_4360_A25C_351A84AE2361__INCLUDED_)
#define AFX_LOGINDIALOG_H__D5E2DF52_E0A2_4360_A25C_351A84AE2361__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// LoginDialog dialog

class LoginDialog : public CDialog
{
	// Construction
public:
	LoginDialog(CWnd* pParent = NULL);	// standard constructor

	// Dialog Data
	//{{AFX_DATA(LoginDialog)
	enum { IDD = IDD_SERVER_DIALOG };
	CString	m_edit_username;
	CString	m_edit_passwd;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(LoginDialog)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

	// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(LoginDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButtonRegister();
	afx_msg void OnButtonLogin();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOGINDIALOG_H__D5E2DF52_E0A2_4360_A25C_351A84AE2361__INCLUDED_)
