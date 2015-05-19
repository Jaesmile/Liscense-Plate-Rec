/**************************************************************************************************
* File Name  : SearchTab.h                                                                        *
* Created   : 15/03/23                                                                            *
* Author   : He Hao                                                                               *
* Model   : 05YOP                                                                                 *
* Description  :   搜索子模块头文件                                                               *
**************************************************************************************************/
#if !defined(AFX_SEARCHTAB_H__71FEC684_2B35_4972_90C5_714A6FD2ED62__INCLUDED_)
#define AFX_SEARCHTAB_H__71FEC684_2B35_4972_90C5_714A6FD2ED62__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SearchTab.h : header file

/////////////////////////////////////////////////////////////////////////////
// SearchTab dialog

class SearchTab : public CDialog
{
	// Construction
public:
	SearchTab(CWnd* pParent = NULL);   // standard constructor

	// Dialog Data
	//{{AFX_DATA(SearchTab)
	enum { IDD = IDD_SEARCH_TAB };
	CListCtrl	m_searchList;
	CString	m_edit_searchKey;
	//}}AFX_DATA

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(SearchTab)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(SearchTab)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonSearachByLpn();
	afx_msg void OnButtonSearchById();
	afx_msg void OnButtonSearchByName();
	afx_msg void OnDblclkSearchList(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SEARCHTAB_H__71FEC684_2B35_4972_90C5_714A6FD2ED62__INCLUDED_)
