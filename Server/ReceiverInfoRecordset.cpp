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
* Description  : ������ݱ��е�һ��������ƥ��ڶ�������������TRUE�����򷵻�FALSE                  *
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
			AfxMessageBox("�������");
			return FALSE;
		}
		MoveNext();
	}
	AfxMessageBox("�û���������");
	Close();
	return FALSE;
}

/**************************************************************************************************
* Function Name : AddRecord                                                                       *
* Description  : ������������Ϊһ����¼��ӵ����ݱ��С�                                           *
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
		AfxMessageBox("�޷����");
		return;
	}

	MoveFirst();
	while (!IsEOF())
	{
		if (username == m_Username)
		{
			AfxMessageBox("�û����Ѵ���");
			return;
		}
		MoveNext();
	}

	AddNew();
	if (username.IsEmpty())
	{
		AfxMessageBox("�û�������Ϊ��");
		return;
	}

	if (passwd.IsEmpty())
	{
		AfxMessageBox("�û�������Ϊ��");
		return;
	}

	m_Username = username;
	m_Password = passwd;

	if (!CanUpdate())
	{
		AfxMessageBox("���ʧ��");
		return;
	}
	Update();
	AfxMessageBox("��ӳɹ�");
}

/**************************************************************************************************
* Function Name : ModifyRecord                                                                    *
* Description  : �޸����ݱ��е�ֵ���ѵڶ���������ֵ�滻��һ��������ֵ                             *
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
		AfxMessageBox("�޷��޸�");
		return;
	}

	//����û����Ƿ��ظ�
	MoveFirst();
	while (!IsEOF())
	{
		if (newUsername == m_Username)
		{
			AfxMessageBox("�û����Ѵ���");
			return;
		}
		MoveNext();
	}

	//�û������ظ�������޸�
	MoveFirst();
	while (!IsEOF())
	{
		if (username == m_Username)
		{
			Edit();
			m_Username = newUsername;
			Update();
			AfxMessageBox("�޸ĳɹ�");
			return;
		}
		MoveNext();
	}
}

/**************************************************************************************************
* Function Name : MatchRecord                                                                     *
* Description  : �����ݱ��е�һ����������Ӧ��ֵ���ɵڶ���ֵ�滻Ϊ��һ��ֵ                         *
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
		AfxMessageBox("�޷��޸�");
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
			AfxMessageBox("�޸ĳɹ�");
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
