/**************************************************************************************************
* File Name  : ModifyPwDialog.h                                                                   *
* Created   : 15/03/23                                                                            *
* Author   : He Hao                                                                               *
* Model   : 05YOP                                                                                 *
* Description  :   修改密码对话框头文件                                                           *
**************************************************************************************************/
#if !defined(AFX_MODIFYPWDIALOG_H__2DBBFC41_8F54_4373_9989_2085250DF835__INCLUDED_)
#define AFX_MODIFYPWDIALOG_H__2DBBFC41_8F54_4373_9989_2085250DF835__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ModifyPwDialog.h : header file

/////////////////////////////////////////////////////////////////////////////
// ModifyPwDialog dialog

class ModifyPwDialog : public CDialog
{
	// Construction
public:
	ModifyPwDialog(CWnd* pParent = NULL);   // standard constructor

	// Dialog Data
	//{{AFX_DATA(ModifyPwDialog)
	enum { IDD = IDD_MODIFYPASSWD_DIALOG };
	CString	m_edit_newPw;
	CString	m_edit_oldPw;
	CString	m_edit_confirmPw;
	//}}AFX_DATA

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(ModifyPwDialog)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(ModifyPwDialog)
	afx_msg void OnModifypasswdButton();
	afx_msg void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MODIFYPWDIALOG_H__2DBBFC41_8F54_4373_9989_2085250DF835__INCLUDED_)
