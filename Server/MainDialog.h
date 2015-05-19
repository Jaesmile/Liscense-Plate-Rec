/**************************************************************************************************
* File Name  : MainDialog.h                                                                       *
* Created   : 15/03/24                                                                            *
* Author   : He Hao                                                                               *
* Model   : 05YOP                                                                                 *
* Description  :   主对话框操作                                                                   *
**************************************************************************************************/
#if !defined(AFX_MAINDIALOG_H__E11435DF_22E1_421D_BBF8_BFD9C01FEFE2__INCLUDED_)
#define AFX_MAINDIALOG_H__E11435DF_22E1_421D_BBF8_BFD9C01FEFE2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "RecognitionTab.h"
#include "SearchTab.h"
#include "SettingTab.h"
#include "ServerSocket.h"
// MainDialog.h : header file
//
#define WM_MYMESSAGE 101
#define VM_MYRECEVIEDPICMSG 102

/////////////////////////////////////////////////////////////////////////////
// MainDialog dialog

class MainDialog : public CDialog
{
	// Construction
public:
	MainDialog(CWnd* pParent = NULL);   // standard constructor
	int KillProcess(LPCSTR pszClassName, LPCSTR pszWindowTitle);
	void OnClose();
	PreTranslateMessage(MSG* pMsg);
	void ShowPicture();

	// Dialog Data
	//{{AFX_DATA(MainDialog)
	enum { IDD = IDD_MAIN_DIALOG };
	CEdit	m_edit_state;
	CListCtrl	m_image_list;
	CTabCtrl	m_tab;
	RecognitionTab m_recognition_tab;
	SettingTab m_setting_tab;
	SearchTab m_search_tab;
	int		m_edit_port;
	//}}AFX_DATA

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(MainDialog)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);// DDX/DDV support
	//}}AFX_VIRTUAL

	// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(MainDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickImageList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnServestartButton();
	afx_msg void HandleMessage(WPARAM wParam, LPARAM lParam);
	afx_msg void HandleMessage2(WPARAM wParam, LPARAM lParam);
	afx_msg void OnStopserveButton();
	afx_msg void OnDblclkImageList(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINDIALOG_H__E11435DF_22E1_421D_BBF8_BFD9C01FEFE2__INCLUDED_)
