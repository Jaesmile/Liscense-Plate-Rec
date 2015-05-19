/**************************************************************************************************
* File Name  : Recognition.h                                                                      *
* Created   : 15/03/23                                                                            *
* Author   : He Hao                                                                               *
* Model   : 05YOP                                                                                 *
* Description  :   分步识别界面头文件                                                             *
**************************************************************************************************/
#if !defined(AFX_RECOGNITION_H__9FE9DE8F_CBEB_4DD4_930A_DE06D9A49F5C__INCLUDED_)
#define AFX_RECOGNITION_H__9FE9DE8F_CBEB_4DD4_930A_DE06D9A49F5C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Recognition.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// Recognition dialog

class Recognition : public CDialog
{
	// Construction
public:
	Recognition(CWnd* pParent = NULL);   // standard constructor

	// Dialog Data
	//{{AFX_DATA(Recognition)
	enum { IDD = IDD_RECOGNITION_DIALOG };
	CString	m_plateInfo;
	CString	m_ratio;
	CString	m_shootDate;
	//}}AFX_DATA


	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(Recognition)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(Recognition)
	afx_msg void OnButtonLocate();
	afx_msg void OnButtonDoGray();
	afx_msg void OnButtonDoBinaryZ();
	afx_msg void OnButtonDoDenois();
	afx_msg void OnButtonSegmPlateChar();
	afx_msg void OnButtonPlateCharRecog();
	afx_msg void OnButtonSave();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RECOGNITION_H__9FE9DE8F_CBEB_4DD4_930A_DE06D9A49F5C__INCLUDED_)
