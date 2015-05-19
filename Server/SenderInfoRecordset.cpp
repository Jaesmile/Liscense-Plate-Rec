/**************************************************************************************************
* File Name  : SenderInfoRecordset.cpp                                                            *
* Created   : 15/03/23                                                                            *
* Author   : Huang Rong                                                                                     *
* Model   : 05YOP                                                                                 *
* Description  : SenderInfoRecordset class implementation file                                    *
**************************************************************************************************/

#include "stdafx.h"
#include "SenderInfoRecordset.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// SenderInfoRecordset

IMPLEMENT_DYNAMIC(SenderInfoRecordset, CDaoRecordset)

SenderInfoRecordset::SenderInfoRecordset(CDaoDatabase* pdb)
: CDaoRecordset(pdb)
{
	//{{AFX_FIELD_INIT(SenderInfoRecordset)
	m_Username = _T("");
	m_Password = _T("");
	m_nFields = 2;
	//}}AFX_FIELD_INIT
	m_nDefaultType = dbOpenDynaset;
}

CString SenderInfoRecordset::GetDefaultDBName()
{
	return _T("DB.mdb");
}

CString SenderInfoRecordset::GetDefaultSQL()
{
	return _T("[SenderInfo]");
}

void SenderInfoRecordset::DoFieldExchange(CDaoFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(SenderInfoRecordset)
	pFX->SetFieldType(CDaoFieldExchange::outputColumn);
	DFX_Text(pFX, _T("[Username]"), m_Username);
	DFX_Text(pFX, _T("[Password]"), m_Password);
	//}}AFX_FIELD_MAP
}

/**************************************************************************************************
* Function Name : MatchRecord                                                                     *
* Description  : 若两个参数能在数据表中匹配，返回TRUE，否则返回FALSE                              *
* Date   : 15/03/23                                                                               *
* Parameter     : CString username,CString passwd                                                 *
* Return Code  : TRUE or FALSE                                                                    *
* Author   : Huang Rong                                                                           *
**************************************************************************************************/
BOOL SenderInfoRecordset::MatchRecord(CString username, CString passwd)
{
	if (!IsOpen())
	{
		Open();
	}

	MoveFirst();
	while (!IsEOF())
	{
		if (username == m_Username)
		{
			if (passwd == m_Password)
			{
				return TRUE;
			}
			return FALSE;
		}
		MoveNext();
	}
	return FALSE;
}

/**************************************************************************************************
* Function Name : AddRecord                                                                       *
* Description  : 将两个参数作为一条记录添加到数据表，成功返回TRUE，否则返回FALSE                  *
* Date   : 15/03/23                                                                               *
* Parameter     : CString username,CString passwd                                                 *
* Return Code  : TRUE or FALSE                                                                    *
* Author   : Huang Rong                                                                           *
**************************************************************************************************/
BOOL SenderInfoRecordset::AddRecord(CString username, CString passwd)
{
	if (!IsOpen()){
		Open();
	}

	if (!CanAppend()){
		return FALSE;
	}

	MoveFirst();
	while (!IsEOF())
	{
		if (username == m_Username)
		{
			AfxMessageBox("用户名已存在");
			return FALSE;
		}
		MoveNext();
	}

	AddNew();
	if (username.IsEmpty())
	{
		return FALSE;
	}

	if (passwd.IsEmpty())
	{
		return FALSE;
	}

	m_Username = username;
	m_Password = passwd;

	if (!CanUpdate())
	{
		return FALSE;
	}
	Update();
	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////
// SenderInfoRecordset diagnostics

#ifdef _DEBUG
void SenderInfoRecordset::AssertValid() const
{
	CDaoRecordset::AssertValid();
}

void SenderInfoRecordset::Dump(CDumpContext& dc) const
{
	CDaoRecordset::Dump(dc);
}
#endif //_DEBUG
