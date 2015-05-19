/**************************************************************************************************
* File Name  : CarOwnerRecordset.cpp                                                              *
* Created   : 15/03/23                                                                            *
* Author   : Huang Rong                                                                           *
* Model   : 05YOP                                                                                 *
* Description  : CarOwnerRecordset class implementation file                                      *
**************************************************************************************************/

#include "StdAfx.h"
#include "CarOwnerRecordset.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CarOwnerRecordset

IMPLEMENT_DYNAMIC(CarOwnerRecordset, CDaoRecordset)

CarOwnerRecordset::CarOwnerRecordset(CDaoDatabase* pdb)
: CDaoRecordset(pdb)
{
	//{{AFX_FIELD_INIT(CarOwnerRecordset)
	m_Name = _T("");
	m_ID = _T("");
	m_PlateNum = _T("");
	m_nFields = 3;
	//}}AFX_FIELD_INIT
	m_nDefaultType = dbOpenDynaset;
}

CString CarOwnerRecordset::GetDefaultDBName()
{
	return _T("DB.mdb");
}

CString CarOwnerRecordset::GetDefaultSQL()
{
	return _T("[CarOwnerInfo]");
}

void CarOwnerRecordset::DoFieldExchange(CDaoFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CarOwnerRecordset)
	pFX->SetFieldType(CDaoFieldExchange::outputColumn);
	DFX_Text(pFX, _T("[Name]"), m_Name);
	DFX_Text(pFX, _T("[ID]"), m_ID);
	DFX_Text(pFX, _T("[PlateNum]"), m_PlateNum);
	//}}AFX_FIELD_MAP
}

/**************************************************************************************************
* Function Name : FindRecordByName                                                                *
* Description  : Find plate number in database by name                                            *
* Date   : 15/03/23                                                                               *
* Parameter     : CString name,long &pos                                                          *
* Return Code  : "" or other                                                                      *
* Author   : Huang Rong                                                                           *
**************************************************************************************************/
CString CarOwnerRecordset::FindRecordByName(CString name, long &pos)
{
	if (!IsOpen()){
		Open();
	}

	if (pos >= GetRecordCount())
	{
		return "";
	}

	SetAbsolutePosition(pos);
	while (!IsEOF())
	{
		if (name == m_Name)
		{
			pos = GetAbsolutePosition() + 1;
			return m_PlateNum;
		}
		MoveNext();
	}
	return "";
}

/**************************************************************************************************
* Function Name : FindRecordById                                                                  *
* Description  : Find plate number in database by ID                                              *
* Date   : 15/03/23                                                                               *
* Parameter     : CString ID                                                                      *
* Return Code  : "" or other                                                                      *
* Author   : Huang Rong                                                                           *
**************************************************************************************************/
CString CarOwnerRecordset::FindRecordById(CString ID)
{
	if (!IsOpen())
	{
		Open();
	}

	MoveFirst();
	while (!IsEOF())
	{
		if (ID == m_ID)
		{
			Close();
			return m_PlateNum;
		}
		MoveNext();
	}
	return "";
}

/////////////////////////////////////////////////////////////////////////////
// CarOwnerRecordset diagnostics

#ifdef _DEBUG
void CarOwnerRecordset::AssertValid() const
{
	CDaoRecordset::AssertValid();
}

void CarOwnerRecordset::Dump(CDumpContext& dc) const
{
	CDaoRecordset::Dump(dc);
}
#endif //_DEBUG
