/**************************************************************************************************
* File Name  : RegisterDialog.h                                                                   *
* Created   : 15/03/23                                                                            *
* Author   : He Hao                                                                               *
* Model   : 05YOP                                                                                 *
* Description  :   注册对话框头文件                                                               *
**************************************************************************************************/
#if !defined(AFX_REGISTERDIALOG_H__6742E482_7CF5_4D33_B6E9_4D35762CD645__INCLUDED_)
#define AFX_REGISTERDIALOG_H__6742E482_7CF5_4D33_B6E9_4D35762CD645__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RegisterDialog.h : header file

/////////////////////////////////////////////////////////////////////////////
// RegisterDialog dialog

class RegisterDialog : public CDialog
{
	// Construction
public:
	RegisterDialog(CWnd* pParent = NULL);   // standard constructor

	// Dialog Data
	//{{AFX_DATA(RegisterDialog)
	enum { IDD = IDD_REGISTER_DIALOG };
	CString	m_edit_username;
	CString	m_edit_passwd;
	CString	m_edit_confirmPW;
	//}}AFX_DATA

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(RegisterDialog)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(RegisterDialog)
	afx_msg void OnButtonConfirmRegister();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REGISTERDIALOG_H__6742E482_7CF5_4D33_B6E9_4D35762CD645__INCLUDED_)
