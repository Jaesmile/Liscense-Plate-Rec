/**************************************************************************************************
* File Name  : SettingTab.h                                                                       *
* Created   : 15/03/23                                                                            *
* Author   :                                                                                      *
* Model   : 05YOP                                                                                 *
* Description  :   设置子界面                                                                     *
**************************************************************************************************/
#if !defined(AFX_SETTINGTAB_H__8A07DAD6_E548_40E1_BC61_94DDB2B7B5B1__INCLUDED_)
#define AFX_SETTINGTAB_H__8A07DAD6_E548_40E1_BC61_94DDB2B7B5B1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SettingTab.h : header file

/////////////////////////////////////////////////////////////////////////////
// SettingTab dialog

class SettingTab : public CDialog
{
	// Construction
public:
	SettingTab(CWnd* pParent = NULL);   // standard constructor

	// Dialog Data
	//{{AFX_DATA(SettingTab)
	enum { IDD = IDD_SETTING_TAB };
	CListCtrl	m_registerList;
	CString	m_edit_curUser;
	//}}AFX_DATA

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(SettingTab)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(SettingTab)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonChangeUsername();
	afx_msg void OnButtonChangePassward();
	afx_msg void OnButtonApprove();
	afx_msg void OnButtonRefuse();
	afx_msg void OnButtonHelp();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETTINGTAB_H__8A07DAD6_E548_40E1_BC61_94DDB2B7B5B1__INCLUDED_)
