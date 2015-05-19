#if !defined(AFX_REGISTERDIALOG_H__A932C7FB_3F20_4643_8C88_6AA26C82E15C__INCLUDED_)
#define AFX_REGISTERDIALOG_H__A932C7FB_3F20_4643_8C88_6AA26C82E15C__INCLUDED_

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
	RegisterDialog(CWnd *pParent = NULL);   // standard constructor

	// Dialog Data
	//{{AFX_DATA(RegisterDialog)
	enum { IDD = IDD_REGISTER_DIALOG };
	CString	m_edit_userName;
	CString	m_edit_password;
	CString	m_edit_confirmPasswd;
	//}}AFX_DATA

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(RegisterDialog)
protected:
	virtual void DoDataExchange(CDataExchange *pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	// Implementation

protected:
	// Generated message map functions
	//{{AFX_MSG(RegisterDialog)
	afx_msg void OnButtonRegister();
	afx_msg void OnButtonReturn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REGISTERDIALOG_H__A932C7FB_3F20_4643_8C88_6AA26C82E15C__INCLUDED_)
