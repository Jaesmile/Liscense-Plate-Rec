/**************************************************************************************************
* File Name  : SenderInfoRecordset.h                                                              *
* Created   : 15/03/23                                                                            *
* Author   : Huang Rong                                                                           *
* Model   : 05YOP                                                                                 *
* Description  :  SenderInfoRecordset类的声明文件                                                 *
**************************************************************************************************/

#if !defined(AFX_SENDERINFORECORDSET_H__5FD90360_3037_490A_886C_E24D78A9AA33__INCLUDED_)
#define AFX_SENDERINFORECORDSET_H__5FD90360_3037_490A_886C_E24D78A9AA33__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SenderInfoRecordset.h : header file

#include <afxdao.h>

/////////////////////////////////////////////////////////////////////////////
// SenderInfoRecordset DAO recordset

class SenderInfoRecordset : private CDaoRecordset
{
public:
	SenderInfoRecordset(CDaoDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(SenderInfoRecordset)

	BOOL MatchRecord(CString username, CString passwd);
	BOOL AddRecord(CString username, CString passwd);

	// Field/Param Data
	//{{AFX_FIELD(SenderInfoRecordset, CDaoRecordset)
	CString	m_Username;
	CString	m_Password;
	//}}AFX_FIELD

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(SenderInfoRecordset)
public:
	virtual CString GetDefaultDBName();		// Default database name
	virtual CString GetDefaultSQL();		// Default SQL for Recordset
	virtual void DoFieldExchange(CDaoFieldExchange* pFX);  // RFX support
	//}}AFX_VIRTUAL

	// Implementation
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SENDERINFORECORDSET_H__5FD90360_3037_490A_886C_E24D78A9AA33__INCLUDED_)
