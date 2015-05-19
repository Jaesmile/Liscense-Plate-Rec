/**************************************************************************************************
* File Name  : ViolationRecordset.cpp                                                             *
* Created   : 15/03/23                                                                            *
* Author   : Huang Rong                                                                           *
* Model   : 05YOP                                                                                 *
* Description  : ViolationRecordset class implementation file                                     *
**************************************************************************************************/

#include "stdafx.h"
#include "ViolationRecordset.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ViolationRecordset

IMPLEMENT_DYNAMIC(ViolationRecordset, CDaoRecordset)

ViolationRecordset::ViolationRecordset(CDaoDatabase* pdb)
: CDaoRecordset(pdb)
{
	//{{AFX_FIELD_INIT(ViolationRecordset)
	m_PlateNum = _T("");
	m_ShootDate = _T("");
	m_nFields = 2;
	//}}AFX_FIELD_INIT
	m_nDefaultType = dbOpenDynaset;
}


CString ViolationRecordset::GetDefaultDBName()
{
	return _T("DB.mdb");
}

CString ViolationRecordset::GetDefaultSQL()
{
	return _T("[ViolationRecord]");
}

void ViolationRecordset::DoFieldExchange(CDaoFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(ViolationRecordset)
	pFX->SetFieldType(CDaoFieldExchange::outputColumn);
	DFX_Text(pFX, _T("[PlateNum]"), m_PlateNum);
	DFX_Text(pFX, _T("[ShootDate]"), m_ShootDate);
	//}}AFX_FIELD_MAP
}

/**************************************************************************************************
* Function Name : FindRecord                                                                      *
* Description  : 查找记录                                                                         *
* Date   : 15/03/23                                                                               *
* Parameter     : CString plateNum,long &pos                                                      *
* Return Code  : "" or other                                                                    *
* Author   : Huang Rong                                                                           *
**************************************************************************************************/
CString ViolationRecordset::FindRecord(CString plateNum, long &pos)
{
	if (!IsOpen()){
		Open();
	}

	if (pos == GetRecordCount())
	{
		return "";
	}

	SetAbsolutePosition(pos);	//设置起始位置
	while (!IsEOF())
	{
		if (plateNum == m_PlateNum)
		{
			pos = GetAbsolutePosition() + 1;
			return m_ShootDate;
		}
		MoveNext();
	}
	Close();
	return "";
}

/**************************************************************************************************
* Function Name : AddRecord                                                                       *
* Description  : 将两个参数作为一条记录添加到数据表，成功返回TRUE，否则返回FALSE                  *
* Date   : 15/03/23                                                                               *
* Parameter     : CString plateNum,CString shootDate                                              *
* Return Code  : TRUE or FALSE                                                                    *
* Author   : Huang Rong                                                                           *
**************************************************************************************************/
BOOL ViolationRecordset::AddRecord(CString plateNum, CString shootDate)
{
	if (plateNum.IsEmpty())
	{
		AfxMessageBox("车牌号不可为空");
		return FALSE;
	}

	if (shootDate.IsEmpty())
	{
		AfxMessageBox("日期不可为空");
		return FALSE;
	}

	if (!IsOpen())
	{
		Open();
	}

	if (!CanAppend())
	{
		return FALSE;
	}

	AddNew();
	m_PlateNum = plateNum;
	m_ShootDate = shootDate;

	if (!CanUpdate())
	{
		return FALSE;
	}
	Update();
	return TRUE;
}

/**************************************************************************************************
* Function Name : DelRecord                                                                       *
* Description  : 将数据表中的记录删除                                                             *
* Date   : 15/03/23                                                                               *
* Parameter     : CString plateNum,CString shootDate                                              *
* Return Code  : void                                                                             *
* Author   : Huang Rong                                                                           *
**************************************************************************************************/
void ViolationRecordset::DelRecord(CString plateNum, CString shootDate)
{
	if (!IsOpen())
	{
		Open();
	}

	MoveFirst();
	while (!IsEOF())
	{
		if (plateNum == m_PlateNum&&shootDate == m_ShootDate)
		{
			Delete();
			return;
		}
		MoveNext();
	}

}

/////////////////////////////////////////////////////////////////////////////
// ViolationRecordset diagnostics

#ifdef _DEBUG
void ViolationRecordset::AssertValid() const
{
	CDaoRecordset::AssertValid();
}

void ViolationRecordset::Dump(CDumpContext& dc) const
{
	CDaoRecordset::Dump(dc);
}
#endif //_DEBUG
