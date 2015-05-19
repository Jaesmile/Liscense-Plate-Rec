/**************************************************************************************************
* File Name  : Help.cpp                                                                           *
* Created   : 03/08/29                                                                            *
* Author   : Hu xin ru                                                                            *
* Model   : 05YOP                                                                                 *
* Description  :   helpÀàµÄÉùÃ÷                                                                   *
**************************************************************************************************/
#if !defined(AFX_HELP_H__2B2DD318_B63D_49B9_A89A_678A360A27E2__INCLUDED_)
#define AFX_HELP_H__2B2DD318_B63D_49B9_A89A_678A360A27E2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Help.h : header file

/////////////////////////////////////////////////////////////////////////////
// Help dialog

class Help : public CDialog
{
	// Construction
public:
	Help(CWnd* pParent = NULL);   // standard constructor

	// Dialog Data
	//{{AFX_DATA(Help)
	enum { IDD = IDD_HELP_DIALOG };
	// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(Help)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(Help)
	afx_msg void OnOk();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HELP_H__2B2DD318_B63D_49B9_A89A_678A360A27E2__INCLUDED_)
