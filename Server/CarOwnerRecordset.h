/**************************************************************************************************
* File Name  : CarOwnerRecordset.h                                                                *
* Created   : 15/03/23                                                                            *
* Author   : Huang Rong                                                                           *
* Model   : 05YOP                                                                                 *
* Description  :  CarOwnerRecordset类的声明文件                                                   *
**************************************************************************************************/

#if !defined(AFX_CAROWNERRECORDSET_H__BD19CCB0_699B_47FC_80EA_AA8C7C19A8CE__INCLUDED_)
#define AFX_CAROWNERRECORDSET_H__BD19CCB0_699B_47FC_80EA_AA8C7C19A8CE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CarOwnerRecordset.h : header file

#include <afxdao.h>

/////////////////////////////////////////////////////////////////////////////
// CarOwnerRecordset DAO recordset

class CarOwnerRecordset : private CDaoRecordset
{
public:
	CarOwnerRecordset(CDaoDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CarOwnerRecordset)

	CString FindRecordByName(CString name, long &pos);
	CString FindRecordById(CString ID);

	// Field/Param Data
	//{{AFX_FIELD(CarOwnerRecordset, CDaoRecordset)
	CString	m_Name;
	CString	m_ID;
	CString	m_PlateNum;
	//}}AFX_FIELD

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CarOwnerRecordset)
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

#endif // !defined(AFX_CAROWNERRECORDSET_H__BD19CCB0_699B_47FC_80EA_AA8C7C19A8CE__INCLUDED_)
