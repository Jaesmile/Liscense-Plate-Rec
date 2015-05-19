/**************************************************************************************************
* File Name  : ViolationRecordset.h                                                               *
* Created   : 15/03/23                                                                            *
* Author   : Huang Rong                                                                           *
* Model   : 05YOP                                                                                 *
* Description  :  ViolationRecordset类的声明文件                                                  *
**************************************************************************************************/

#if !defined(AFX_VIOLATIONRECORDSET_H__064066FD_F6B2_42EB_9247_B475A34F6524__INCLUDED_)
#define AFX_VIOLATIONRECORDSET_H__064066FD_F6B2_42EB_9247_B475A34F6524__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ViolationRecordset.h : header file

#include <afxdao.h>

/////////////////////////////////////////////////////////////////////////////
// ViolationRecordset DAO recordset

class ViolationRecordset : private CDaoRecordset
{
public:
	ViolationRecordset(CDaoDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(ViolationRecordset)

	CString FindRecord(CString plateNum, long &pos);
	BOOL AddRecord(CString plateNum, CString shootDate);
	void DelRecord(CString plateNum, CString shootDate);

	// Field/Param Data
	//{{AFX_FIELD(ViolationRecordset, CDaoRecordset)
	CString	m_PlateNum;
	CString	m_ShootDate;
	//}}AFX_FIELD

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(ViolationRecordset)
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

#endif // !defined(AFX_VIOLATIONRECORDSET_H__064066FD_F6B2_42EB_9247_B475A34F6524__INCLUDED_)
