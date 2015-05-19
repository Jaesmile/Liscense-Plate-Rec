#if !defined(AFX_SETTINGDIALOG_H__CDF8C7A8_1BAA_48F5_BBC2_37E9F083878D__INCLUDED_)
#define AFX_SETTINGDIALOG_H__CDF8C7A8_1BAA_48F5_BBC2_37E9F083878D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SettingDialog.h : header file

/////////////////////////////////////////////////////////////////////////////
// SettingDialog dialog

class SettingDialog : public CDialog
{
	// Construction
public:
	SettingDialog(CWnd *pParent = NULL);   // standard constructor
	// Dialog Data
	//{{AFX_DATA(SettingDialog)
	enum { IDD = IDD_SETTING_DIALOG };
	CIPAddressCtrl	m_IPAddress;
	CString	m_port;
	//}}AFX_DATA

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(SettingDialog)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	// Implementation

protected:
	// Generated message map functions
	//{{AFX_MSG(SettingDialog)
	afx_msg void OnButtonOk();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETTINGDIALOG_H__CDF8C7A8_1BAA_48F5_BBC2_37E9F083878D__INCLUDED_)
