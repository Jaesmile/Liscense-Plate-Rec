/**************************************************************************************************
* File Name  : DelDialog.h                                                                        *
* Created   : 15/03/23                                                                            *
* Author   : He Hao                                                                               *
* Model   : 05YOP                                                                                 *
* Description  :   删除对话框头文件                                                               *
**************************************************************************************************/
#if !defined(AFX_DELDIALOG_H__FA373E8D_2102_4AC3_84AD_A35CDBCE3FF9__INCLUDED_)
#define AFX_DELDIALOG_H__FA373E8D_2102_4AC3_84AD_A35CDBCE3FF9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DelDialog.h : header file

/////////////////////////////////////////////////////////////////////////////
// DelDialog dialog

class DelDialog : public CDialog
{
	// Construction
public:
	DelDialog(CWnd* pParent = NULL);   // standard constructor

	CString m_plateNum;
	CString m_shootDate;

	// Dialog Data
	//{{AFX_DATA(DelDialog)
	enum { IDD = IDD_DEL_DIALOG };
	// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DelDialog)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DelDialog)
	afx_msg void OnButtonConfirmDel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DELDIALOG_H__FA373E8D_2102_4AC3_84AD_A35CDBCE3FF9__INCLUDED_)
