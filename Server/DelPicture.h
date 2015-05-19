#if !defined(AFX_DELPICTURE_H__4D2B2046_B195_4361_8E7D_1C47BDB9305C__INCLUDED_)
#define AFX_DELPICTURE_H__4D2B2046_B195_4361_8E7D_1C47BDB9305C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DelPicture.h : header file

/////////////////////////////////////////////////////////////////////////////
// DelPicture dialog

class DelPicture : public CDialog
{
	// Construction
public:
	DelPicture(CWnd* pParent = NULL);   // standard constructor

	// Dialog Data
	//{{AFX_DATA(DelPicture)
	enum { IDD = IDD_DEL_PICTURE_DIALOG };
	// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DelPicture)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DelPicture)
	afx_msg void OnButtonConfirm();
	afx_msg void OnButtonCancle();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DELPICTURE_H__4D2B2046_B195_4361_8E7D_1C47BDB9305C__INCLUDED_)
