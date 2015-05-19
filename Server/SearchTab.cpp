/**************************************************************************************************
* File Name  : SearchTab.cpp                                                                      *
* Created   : 03/08/29                                                                            *
* Author   : He Hao                                                                                     *
* Model   : 05YOP                                                                                 *
* Description  :   [[CN]] ��ѯ�ӽ��� [[CN]]                                                       *
**************************************************************************************************/

#include "stdafx.h"
#include "Server.h"
#include "SearchTab.h"
#include "CarOwnerRecordset.h"
#include "ViolationRecordset.h"
#include "DelDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// SearchTab dialog


SearchTab::SearchTab(CWnd* pParent /*=NULL*/)
	: CDialog(SearchTab::IDD, pParent)
{
	//{{AFX_DATA_INIT(SearchTab)
	m_edit_searchKey = _T("");
	//}}AFX_DATA_INIT
}

void SearchTab::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(SearchTab)
	DDX_Control(pDX, IDC_SEARCH_LIST, m_searchList);
	DDX_Text(pDX, IDC_EDIT_SEARCH_KEY, m_edit_searchKey);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(SearchTab, CDialog)
	//{{AFX_MSG_MAP(SearchTab)
	ON_BN_CLICKED(IDC_BUTTON_SEARACH_BY_LPN, OnButtonSearachByLpn)
	ON_BN_CLICKED(IDC_BUTTON_SEARCH_BY_ID, OnButtonSearchById)
	ON_BN_CLICKED(IDC_BUTTON_SEARCH_BY_NAME, OnButtonSearchByName)
	ON_NOTIFY(NM_DBLCLK, IDC_SEARCH_LIST, OnDblclkSearchList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// SearchTab message handlers
/*******************************************************************************************
* Function Name : OnInitDialog                                                             *
* Description  :  Tab�ؼ��ĳ�ʼ��                                                          *
* Date   : 03/3/20                                                                         *
* Parameter     : ��                                                                       *
* Return Code  :  void                                                                     *
* Author   :    He  hao                                                                    *
*******************************************************************************************/
BOOL SearchTab::OnInitDialog()
{
	CDialog::OnInitDialog();

	//��ʼ�����
	m_searchList.InsertColumn(0, "Υ�³���", LVCFMT_LEFT, 168, 0);
	m_searchList.InsertColumn(2, "Υ��ʱ��", LVCFMT_LEFT, 168, 0);
	m_searchList.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

/*******************************************************************************************
* Function Name : OnButtonSearachByLpn                                                     *
* Description  :  �����Ʋ�ѯ                                                               *
* Date   : 03/3/20                                                                         *
* Parameter     : ��                                                                       *
* Return Code  :  void                                                                     *
* Author   :    Huang rong                                                                 *
*******************************************************************************************/
void SearchTab::OnButtonSearachByLpn()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_searchList.DeleteAllItems();
	
	ViolationRecordset set;
	
	long pos = 0;
	CString date = set.FindRecord(m_edit_searchKey, pos);
	if (date.IsEmpty())
	{
		MessageBox("δ�ҵ�Υ�¼�¼.");
		return;
	}
	while (!date.IsEmpty())
	{
		int listIndex = m_searchList.InsertItem(0, m_edit_searchKey);
		m_searchList.SetItemText(listIndex, 1, date);
		date = set.FindRecord(m_edit_searchKey, pos);
	}
}

/*******************************************************************************************
* Function Name : OnButtonSearchById                                                       *
* Description  :  �����֤��ѯ                                                             *
* Date   : 03/3/20                                                                         *
* Parameter     : ��                                                                       *
* Return Code  :  void                                                                     *
* Author   :    Huang rong                                                                 *
*******************************************************************************************/
void SearchTab::OnButtonSearchById()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_searchList.DeleteAllItems();
	
	CarOwnerRecordset ownrSet;
	ViolationRecordset vioSet;
	
	long pos = 0;
	CString plateNum = ownrSet.FindRecordById(m_edit_searchKey);
	
	if (!plateNum.IsEmpty())
	{
		CString date = vioSet.FindRecord(plateNum, pos);
		if (date.IsEmpty())
		{
			MessageBox("δ�ҵ�Υ�¼�¼.");
			return;
		}
		while (!date.IsEmpty())
		{			
			int listIndex = m_searchList.InsertItem(0, plateNum);
			m_searchList.SetItemText(listIndex, 1, date);
			date = vioSet.FindRecord(plateNum, pos);
		}
	}
	else{
		AfxMessageBox("δ�ҵ������֤��.");
	}
}

/*******************************************************************************************
* Function Name : OnButtonSearchByName                                                     *
* Description  :  ��������ѯ                                                               *
* Date   : 03/3/20                                                                         *
* Parameter     : ��                                                                       *
* Return Code  :  void                                                                     *
* Author   :    Huang rong                                                                 *
*******************************************************************************************/
void SearchTab::OnButtonSearchByName()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_searchList.DeleteAllItems();
	
	CarOwnerRecordset ownrSet;
	ViolationRecordset vioSet;
	
	long ownrRecordIndex = 0;
	long vioRecordIndex = 0;
	CString date;
	BOOL isFound=FALSE;
	
	CString plateNum = ownrSet.FindRecordByName(m_edit_searchKey, ownrRecordIndex);
	if (plateNum.IsEmpty())
	{
		AfxMessageBox("δ�ҵ�������.");
		return;
	}
	while (!plateNum.IsEmpty())
	{
		date = vioSet.FindRecord(plateNum, vioRecordIndex);
		while (!date.IsEmpty())
		{
			int listIndex = m_searchList.InsertItem(0, plateNum);
			m_searchList.SetItemText(listIndex, 1, date);
			isFound=TRUE;
			date = vioSet.FindRecord(plateNum, vioRecordIndex);
		}
		vioRecordIndex = 0;
		plateNum = ownrSet.FindRecordByName(m_edit_searchKey, ownrRecordIndex);
	}
	if(!isFound)
	{
		MessageBox("δ�ҵ�Υ�¼�¼.");
	}
}

/*******************************************************************************************
* Function Name : OnDblclkSearchList                                                       *
* Description  :  ˫����Ӧ����������ɾ���Ի���                                             *
* Date   : 03/3/20                                                                         *
* Parameter     : ��                                                                       *
* Return Code  :  void                                                                     *
* Author   :    He  hao                                                                    *
*******************************************************************************************/
void SearchTab::OnDblclkSearchList(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: Add your control notification handler code here
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	NM_LISTVIEW *pNMListView = (NM_LISTVIEW *)pNMHDR;
	int nItem = pNMListView->iItem;
	if (nItem >= 0 && nItem < m_searchList.GetItemCount())
	{
		DelDialog delDialog;

		delDialog.m_plateNum = m_searchList.GetItemText(nItem, 0);
		delDialog.m_shootDate = m_searchList.GetItemText(nItem, 1);

		delDialog.DoModal();

		if(g_refresh=="REFRESH")
		{
			m_searchList.DeleteItem(nItem);
			g_refresh="";
		}
		

	}
	*pResult = 0;
}
