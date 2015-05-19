/**************************************************************************************************
* File Name  : ReceiverInfoRecordset.h                                                            *
* Created   : 15/03/23                                                                            *
* Author   : Huang Rong                                                                           *
* Model   : 05YOP                                                                                 *
* Description  :  ReceiverInfoRecordset类的声明文件                                               *
**************************************************************************************************/

#if !defined(AFX_RECEIVERINFORECORDSET_H__47E5D850_E163_4AD9_8C6E_5B194D68201F__INCLUDED_)
#define AFX_RECEIVERINFORECORDSET_H__47E5D850_E163_4AD9_8C6E_5B194D68201F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ReceiverInfoRecordset.h : header file

#include <afxdao.h>

/////////////////////////////////////////////////////////////////////////////
// ReceiverInfoRecordset DAO recordset

class ReceiverInfoRecordset : private CDaoRecordset
{
public:
	ReceiverInfoRecordset(CDaoDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(ReceiverInfoRecordset)

	BOOL MatchRecord(CString username, CString passwd);
	void AddRecord(CString username, CString passwd);
	void ModifyRecord(CString username, CString newUsername);
	void ModifyRecord(CString username, CString passwd, CString newPasswd);

	// Field/Param Data
	//{{AFX_FIELD(ReceiverInfoRecordset, CDaoRecordset)
	CString	m_Username;
	CString	m_Password;
	//}}AFX_FIELD

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(ReceiverInfoRecordset)
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

#endif // !defined(AFX_RECEIVERINFORECORDSET_H__47E5D850_E163_4AD9_8C6E_5B194D68201F__INCLUDED_)
