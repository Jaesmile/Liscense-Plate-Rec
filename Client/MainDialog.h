#if !defined(AFX_MAINDIALOG_H__B4A3986E_16D1_4BFA_82B4_8AFD4355D7BA__INCLUDED_)
#define AFX_MAINDIALOG_H__B4A3986E_16D1_4BFA_82B4_8AFD4355D7BA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MainDialog.h : header file

#include <vector>
using std::vector;
/////////////////////////////////////////////////////////////////////////////
// MainDialog dialog

class MainDialog : public CDialog
{
	// Construction
public:
	MainDialog(CWnd *pParent = NULL);   // standard constructor

	vector<CString> picPathVector;

	// Dialog Data
	//{{AFX_DATA(MainDialog)
	enum { IDD = IDD_MAIN_DIALOG };
	CProgressCtrl	m_progress;
	CListCtrl	m_sendList;
	CString	m_process;
	//}}AFX_DATA


	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(MainDialog)
protected:
	virtual void DoDataExchange(CDataExchange *pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(MainDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonScan();
	afx_msg void OnClickSendList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnButtonSend();
	afx_msg void HandleMessage(WPARAM wParam, LPARAM lParam);
	afx_msg void OnButtonStopSend();
	afx_msg void OnDblclkSendList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnButtonClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINDIALOG_H__B4A3986E_16D1_4BFA_82B4_8AFD4355D7BA__INCLUDED_)
