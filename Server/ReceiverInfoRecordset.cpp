/**************************************************************************************************
* File Name  : ReceiverInfoRecordset.cpp                                                          *
* Created   : 15/03/23                                                                            *
* Author   : Huang Rong                                                                           *
* Model   : 05YOP                                                                                 *
* Description  : ReceiverInfoRecordset class implementation file                                  *
**************************************************************************************************/

#include "stdafx.h"
#include "ReceiverInfoRecordset.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ReceiverInfoRecordset

IMPLEMENT_DYNAMIC(ReceiverInfoRecordset, CDaoRecordset)

ReceiverInfoRecordset::ReceiverInfoRecordset(CDaoDatabase* pdb)
: CDaoRecordset(pdb)
{
	//{{AFX_FIELD_INIT(ReceiverInfoRecordset)
	m_Username = _T("");
	m_Password = _T("");
	m_nFields = 2;
	//}}AFX_FIELD_INIT
	m_nDefaultType = dbOpenDynaset;
}

CString ReceiverInfoRecordset::GetDefaultDBName()
{
	return _T("DB.mdb");
}

CString ReceiverInfoRecordset::GetDefaultSQL()
{
	return _T("[ReceiverInfo]");
}

void ReceiverInfoRecordset::DoFieldExchange(CDaoFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(ReceiverInfoRecordset)
	pFX->SetFieldType(CDaoFieldExchange::outputColumn);
	DFX_Text(pFX, _T("[Username]"), m_Username);
	DFX_Text(pFX, _T("[Password]"), m_Password);
	//}}AFX_FIELD_MAP
}

/**************************************************************************************************
* Function Name : MatchRecord                                                                     *
* Description  : 如果数据表中第一个参数能匹配第二个参数，返回TRUE，否则返回FALSE                  *
* Date   : 15/03/23                                                                               *
* Parameter     : CString username,CString passwd                                                 *
* Return Code  : TRUE or FALSE                                                                    *
* Author   : Huang Rong                                                                           *
**************************************************************************************************/
BOOL ReceiverInfoRecordset::MatchRecord(CString username, CString passwd)
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
				Close();
				return TRUE;
			}
			AfxMessageBox("密码错误");
			return FALSE;
		}
		MoveNext();
	}
	AfxMessageBox("用户名不存在");
	Close();
	return FALSE;
}

/**************************************************************************************************
* Function Name : AddRecord                                                                       *
* Description  : 将两个参数作为一条记录添加到数据表中。                                           *
* Date   : 15/03/23                                                                               *
* Parameter     : CString username,CString passwd                                                 *
* Return Code  : void                                                                             *
* Author   : Huang Rong                                                                           *
**************************************************************************************************/
void ReceiverInfoRecordset::AddRecord(CString username, CString passwd)
{
	if (!IsOpen())
	{
		Open();
	}

	if (!CanAppend())
	{
		AfxMessageBox("无法添加");
		return;
	}

	MoveFirst();
	while (!IsEOF())
	{
		if (username == m_Username)
		{
			AfxMessageBox("用户名已存在");
			return;
		}
		MoveNext();
	}

	AddNew();
	if (username.IsEmpty())
	{
		AfxMessageBox("用户名不可为空");
		return;
	}

	if (passwd.IsEmpty())
	{
		AfxMessageBox("用户名不可为空");
		return;
	}

	m_Username = username;
	m_Password = passwd;

	if (!CanUpdate())
	{
		AfxMessageBox("添加失败");
		return;
	}
	Update();
	AfxMessageBox("添加成功");
}

/**************************************************************************************************
* Function Name : ModifyRecord                                                                    *
* Description  : 修改数据表中的值，已第二个参数的值替换第一个参数的值                             *
* Date   : 15/03/23                                                                               *
* Parameter     : CString username,CString passwd                                                 *
* Return Code  : void                                                                             *
* Author   : Huang Rong                                                                           *
**************************************************************************************************/
void ReceiverInfoRecordset::ModifyRecord(CString username, CString newUsername)
{
	if (!IsOpen())
	{
		Open();
	}

	if (!CanAppend())
	{
		AfxMessageBox("无法修改");
		return;
	}

	//检查用户名是否重复
	MoveFirst();
	while (!IsEOF())
	{
		if (newUsername == m_Username)
		{
			AfxMessageBox("用户名已存在");
			return;
		}
		MoveNext();
	}

	//用户名不重复则进行修改
	MoveFirst();
	while (!IsEOF())
	{
		if (username == m_Username)
		{
			Edit();
			m_Username = newUsername;
			Update();
			AfxMessageBox("修改成功");
			return;
		}
		MoveNext();
	}
}

/**************************************************************************************************
* Function Name : MatchRecord                                                                     *
* Description  : 将数据表中第一个参数所对应的值，由第二个值替换为第一个值                         *
* Date   : 15/03/23                                                                               *
* Parameter     : CString username,CString passwd                                                 *
* Return Code  : TRUE or FALSE                                                                    *
* Author   : Huang Rong                                                                           *
**************************************************************************************************/
void ReceiverInfoRecordset::ModifyRecord(CString username, CString passwd, CString newPasswd)
{
	if (!IsOpen())
	{
		Open();
	}
	if (!CanAppend())
	{
		AfxMessageBox("无法修改");
		return;
	}

	MoveFirst();
	while (!IsEOF())
	{
		if (username == m_Username)
		{
			Edit();
			m_Password = newPasswd;
			Update();
			AfxMessageBox("修改成功");
			return;
		}
		MoveNext();
	}
}
/////////////////////////////////////////////////////////////////////////////
// ReceiverInfoRecordset diagnostics

#ifdef _DEBUG
void ReceiverInfoRecordset::AssertValid() const
{
	CDaoRecordset::AssertValid();
}

void ReceiverInfoRecordset::Dump(CDumpContext& dc) const
{
	CDaoRecordset::Dump(dc);
}
#endif //_DEBUG
