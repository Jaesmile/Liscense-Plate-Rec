/**************************************************************************************************
* File Name  : RecognitionTab.h                                                                   *
* Created   : 15/03/23                                                                            *
* Author   :                                                                                      *
* Model   : 05YOP                                                                                 *
* Description  :   [[CN]] 识别tab头文件 [[CN]]                                                      *
**************************************************************************************************/
#if !defined(AFX_RECOGNITIONTAB_H__24161139_64F9_47F8_95CD_DC811827FDEC__INCLUDED_)
#define AFX_RECOGNITIONTAB_H__24161139_64F9_47F8_95CD_DC811827FDEC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 
#include "Recognition.h"
// _MSC_VER > 1000
// RecognitionTab.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// RecognitionTab dialog

class RecognitionTab : public CDialog
{
	// Construction
public:
	RecognitionTab(CWnd* pParent = NULL);   // standard constructor

	// Dialog Data
	//{{AFX_DATA(RecognitionTab)
	enum { IDD = IDD_RECOGNITION_TAB };
	CStatic	m_picture;
	CString	m_plateInfo;
	CString	m_shootDate;
	//}}AFX_DATA


	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(RecognitionTab)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(RecognitionTab)
	afx_msg void OnButtonRecognition();
	afx_msg void OnButtonKeyrecognition();
	afx_msg void OnButtonSave();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RECOGNITIONTAB_H__24161139_64F9_47F8_95CD_DC811827FDEC__INCLUDED_)
