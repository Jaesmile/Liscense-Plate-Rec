/**************************************************************************************************
* File Name  : ModifyNameDialog.h                                                                 *
* Created   : 15/03/23                                                                            *
* Author   : He Hao                                                                               *
* Model   : 05YOP                                                                                 *
* Description  :   修改用户名对话框头文件                                                         *
**************************************************************************************************/
#if !defined(AFX_MODIFYNAMEDIALOG_H__F24D681B_3466_4DAA_B6AF_B254E3DC374F__INCLUDED_)
#define AFX_MODIFYNAMEDIALOG_H__F24D681B_3466_4DAA_B6AF_B254E3DC374F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ModifyNameDialog.h : header file

/////////////////////////////////////////////////////////////////////////////
// ModifyNameDialog dialog

class ModifyNameDialog : public CDialog
{
	// Construction
public:
	ModifyNameDialog(CWnd* pParent = NULL);   // standard constructor

	// Dialog Data
	//{{AFX_DATA(ModifyNameDialog)
	enum { IDD = IDD_MODIFYNAME_DIALOG };
	CString	m_edit_newName;
	//}}AFX_DATA

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(ModifyNameDialog)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(ModifyNameDialog)
	afx_msg void OnButtonModify();
	afx_msg void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MODIFYNAMEDIALOG_H__F24D681B_3466_4DAA_B6AF_B254E3DC374F__INCLUDED_)
