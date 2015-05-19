// LoginDlg.h : header file

#if !defined(AFX_LOGINDLG_H__712CD0C5_BE52_45FA_8136_40D3D7D965DA__INCLUDED_)
#define AFX_LOGINDLG_H__712CD0C5_BE52_45FA_8136_40D3D7D965DA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// LogintDlg dialog

class LogintDlg : public CDialog
{
	// Construction
public:
	LogintDlg(CWnd* pParent = NULL);	// standard constructor

	// Dialog Data
	//{{AFX_DATA(LogintDlg)
	enum { IDD = IDD_CLIENT_DIALOG };
	CString	m_userName;
	CString	m_password;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(LogintDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL
	// Implementation

protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(LogintDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButtonSetting();
	afx_msg void OnButtonRegister();
	afx_msg void OnButtonLogin();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOGINDLG_H__712CD0C5_BE52_45FA_8136_40D3D7D965DA__INCLUDED_)
