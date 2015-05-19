/**************************************************************************************************
* File Name  : LogicListOperator.cpp                                                              *
* Created   : 03/08/29                                                                            *
* Author   :                                                                                      *
* Model   : 05YOP                                                                                 *
* Description  :   [[CN]] ������� [[CN]]                                                         *
**************************************************************************************************/

#include "stdafx.h"
#include "Server.h"
#include "MainDialog.h"
#include "ServerSocket.h"
#include "SenderInfoRecordset.h"
#include "DelPicture.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

void thread_func(LPVOID p);
extern DWORD ID;
HANDLE hThread2;
int a = 0;

/////////////////////////////////////////////////////////////////////////////
// MainDialog dialog
MainDialog::MainDialog(CWnd* pParent /*=NULL*/)
	: CDialog(MainDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(MainDialog)
	m_edit_port = 0;
	//}}AFX_DATA_INIT
}

void MainDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(MainDialog)
	DDX_Control(pDX, IDC_STATE_EDIT, m_edit_state);
	DDX_Control(pDX, IDC_IMAGE_LIST, m_image_list);
	DDX_Control(pDX, IDC_TAB, m_tab);
	DDX_Text(pDX, IDC_PORT_EDIT, m_edit_port);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(MainDialog, CDialog)
	//{{AFX_MSG_MAP(MainDialog)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, OnSelchangeTab)
	ON_NOTIFY(NM_CLICK, IDC_IMAGE_LIST, OnClickImageList)
	ON_BN_CLICKED(IDC_SERVESTART_BUTTON, OnServestartButton)
	ON_MESSAGE(WM_MYMESSAGE, HandleMessage)
	ON_MESSAGE(VM_MYRECEVIEDPICMSG, HandleMessage2)
	ON_BN_CLICKED(IDC_STOPSERVE_BUTTON, OnStopserveButton)
	ON_NOTIFY(NM_DBLCLK, IDC_IMAGE_LIST, OnDblclkImageList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// MainDialog message handlers

BOOL MainDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	g_hwnd = m_hWnd;//  ��ȫ�ֱ�����ֵ

	//������ؼ��ĳ�ʼ��
	m_tab.InsertItem(0, "ʶ��");
	m_tab.InsertItem(1, "��ѯ");
	m_tab.InsertItem(2, "����");

	m_image_list.InsertColumn(0, "ͼƬ����", LVCFMT_LEFT, 70, 0);
	m_image_list.InsertColumn(1, "����ʱ��", LVCFMT_LEFT, 70, 0);
	m_image_list.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	m_recognition_tab.Create(IDD_RECOGNITION_TAB, &m_tab);
	m_search_tab.Create(IDD_SEARCH_TAB, &m_tab);
	m_setting_tab.Create(IDD_SETTING_TAB, &m_tab);

	CFileFind* fileFind = new CFileFind();
	CTime tempTime;
	CString str;
	BOOL res = fileFind->FindFile("recevied_picture//*.BMP");
	int i = 0;
	while (res)
	{
		res = fileFind->FindNextFile();
		if (fileFind->IsDirectory())
			continue;
		int temp = (m_image_list.InsertItem(i, fileFind->GetFileName(), i));
		if (fileFind->GetCreationTime(tempTime))
			str = tempTime.Format(_T("%c"));

		m_image_list.SetItemText(temp, 1, str);
		i++;
	}

	CRect loc_main;
	m_tab.GetClientRect(&loc_main);
	loc_main.top += 22;

	m_recognition_tab.MoveWindow(loc_main);
	m_search_tab.MoveWindow(loc_main);
	m_setting_tab.MoveWindow(loc_main);

	m_recognition_tab.ShowWindow(true);
	m_search_tab.ShowWindow(false);
	m_setting_tab.ShowWindow(false);

	m_tab.SetCurSel(0);

	m_image_list.SetItemState(0, LVIS_SELECTED|LVIS_FOCUSED,LVIS_SELECTED|LVIS_FOCUSED); 
	ShowPicture();


	CEdit *edit = (CEdit*)GetDlgItem(IDC_STATE_EDIT);
	edit->EnableWindow(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

/*******************************************************************************************
* Function Name : OnSelchangeTab                                                           *
* Description  :  �л�Tab�ؼ�                                                              *
* Date   : 03/3/20                                                                         *
* Parameter     : ��                                                                       *
* Return Code  :  void                                                                     *
* Author   :   He Hao                                                                      *
*******************************************************************************************/
void MainDialog::OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: Add your control notification handler code her

	int CurSel = m_tab.GetCurSel();
	switch (CurSel)
	{
	case 0:
		m_recognition_tab.ShowWindow(true);
		m_search_tab.ShowWindow(false);
		m_setting_tab.ShowWindow(false);
		break;
	case 1:
		m_recognition_tab.ShowWindow(false);
		m_search_tab.ShowWindow(true);
		m_setting_tab.ShowWindow(false);
		break;
	case 2:
		m_recognition_tab.ShowWindow(false);
		m_search_tab.ShowWindow(false);
		m_setting_tab.ShowWindow(true);
		break;
	default:
		;
	}
	*pResult = 0;
}

/*******************************************************************************************
* Function Name : OnClickImageList                                                         *
* Description  :  ������ʾͼƬ                                                             *
* Date   : 03/3/20                                                                         *
* Parameter     : ��                                                                       *
* Return Code  :  void                                                                     *
* Author   :   He Hao                                                                      *
*******************************************************************************************/
void MainDialog::OnClickImageList(NMHDR* pNMHDR, LRESULT* pResult)
{
	ShowPicture();

	*pResult = 0;
}
/*******************************************************************************************
* Function Name : ShowPicture                                                              *
* Description  :  Delete record from database                                              *
* Date   : 03/3/20                                                                         *
* Parameter     : ��                                                                       *
* Return Code  :  void                                                                     *
* Author   :   He Hao                                                                      *
*******************************************************************************************/
void MainDialog::ShowPicture()
{
	//��ʾѡ���ͼƬ��ͼƬ�ؼ�
	POSITION pos = m_image_list.GetFirstSelectedItemPosition();
	int nId = (int)m_image_list.GetNextSelectedItem(pos);
	g_picPath = m_image_list.GetItemText(nId, 0);
	g_picPath = "recevied_picture//" + g_picPath;
	
	
	CRect r;
	GetClientRect(&r);
	
	HBITMAP hbitmap = (HBITMAP)LoadImage(AfxGetInstanceHandle(), g_picPath, IMAGE_BITMAP, r.Width() / 2, r.Height() / 2, LR_LOADFROMFILE);
	m_recognition_tab.m_picture.SetBitmap(hbitmap);
}
/*******************************************************************************************
* Function Name : OnServestartButton                                                       *
* Description  :  ��������                                                                 *
* Date   : 03/3/20                                                                         *
* Parameter     : ��                                                                       *
* Return Code  :  void                                                                     *
* Author   :   Tu  yu  / He hao                                                            *
*******************************************************************************************/
void MainDialog::OnServestartButton()
{
	// TODO: Add your control notification handler code here

	UpdateData(TRUE);

      //�����̣߳��򿪷�����
	hThread2 = CreateThread(NULL, 100, (LPTHREAD_START_ROUTINE)thread_func, (LPVOID)&m_edit_port, 0, NULL);
	m_edit_state.SetWindowText("�������Ѵ�........");

	UpdateData(FALSE);
        //ʹ��������ť����ѡ��
	CButton *pButton = (CButton*)GetDlgItem(IDC_SERVESTART_BUTTON);
	pButton->EnableWindow(FALSE);

}
/*******************************************************************************************
* Function Name : OnStopserveButton                                                        *
* Description  :  ֹͣ����                                                                 *
* Date   : 03/3/20                                                                         *
* Parameter     : ��                                                                       *
* Return Code  :  void                                                                     *
* Author   :   He Hao                                                                      *
*******************************************************************************************/
void MainDialog::OnStopserveButton()
{
	
    //�����̣߳��ر��׽��֣�ʹ�������Ͽ�
	SuspendThread(hThread2);
	closesocket(g_serSocket);
	m_edit_state.SetWindowText("�������ѹر�........");
	CButton *pButton = (CButton*)GetDlgItem(IDC_SERVESTART_BUTTON);
	pButton->EnableWindow(TRUE);
	UpdateData(FALSE);

}
/*******************************************************************************************
* Function Name : thread_func                                                              *
* Description  :  �̺߳���                                                                 *
* Date   : 03/3/20                                                                         *
* Parameter     : ��                                                                       *
* Return Code  :  void                                                                     *
* Author   :   Tu yu                                                                       *
*******************************************************************************************/
void thread_func(LPVOID p)
{
	ServerSocket server;
	int b = *(int *)p;
	server.Monitor(b);
}
/*******************************************************************************************
* Function Name :HandleMessage                                                             *
* Description  :  ��Ϣ��Ӧ�������������ڷ�����Ϣ                                           *
* Date   : 03/3/20                                                                         *
* Parameter     :WPARAM wParam, LPARAM lParam                                              *
* Return Code  :  void                                                                     *
* Author   :   Huang rong                                                                  *
*******************************************************************************************/
void MainDialog::HandleMessage(WPARAM wParam, LPARAM lParam)
{
	char *flag = strtok((char *)lParam, "#");
	char *name = strtok(NULL, "#");
	char *pass = strtok(NULL, "#");
	SenderInfoRecordset set;

	if (strcmp(flag, "LOGIN") == 0)
	{
		if (set.MatchRecord(name, pass))
		{

			char *message = "LOGOK#";
			PostThreadMessage(ID, WM_MYMESSAGE, 0, (long)message);
		}
		else
		{
			char *message = "LOGNO#";
			PostThreadMessage(ID, WM_MYMESSAGE, 0, (long)message);
		}
	}

	if (strcmp(flag, "REGIS") == 0)
	{

		int nItem = m_setting_tab.m_registerList.InsertItem(0, name);
		m_setting_tab.m_registerList.SetItemText(nItem, 1, pass);
		char *message = "REGOK#";
		PostThreadMessage(ID, WM_MYMESSAGE, 0, (long)message);
	}
}
/*******************************************************************************************
* Function Name : HandleMessage2                                                           *
* Description  :  ��Ϣ��Ӧ����������ͼƬ�б�                                               *
* Date   : 03/3/20                                                                         *
* Parameter     : WPARAM wParam, LPARAM lParam                                             *
* Return Code  :  void                                                                     *
* Author   :   He Hao                                                                      *
*******************************************************************************************/
void MainDialog::HandleMessage2(WPARAM wParam, LPARAM lParam)
{
	m_image_list.DeleteAllItems();
	CFileFind* fileFind = new CFileFind();
	CTime tempTime;
	CString str;
	BOOL res = fileFind->FindFile("recevied_picture//*.BMP");
	int i = 0;
	while (res)
	{
		res = fileFind->FindNextFile();
		if (fileFind->IsDirectory())
			continue;
		int temp = (m_image_list.InsertItem(i, fileFind->GetFileName(), i));
		if (fileFind->GetCreationTime(tempTime))
			str = tempTime.Format(_T("%c"));

		m_image_list.SetItemText(temp, 1, str);
		i++;
	}

}
/*******************************************************************************************
* Function Name : KillProcess                                                              *
* Description  : �ر��̺߳���                                                              *
* Date   : 03/3/20                                                                         *
* Parameter     : ��                                                                       *
* Return Code  :  void                                                                     *
* Author   :   He Hao                                                                      *
*******************************************************************************************/
int MainDialog::KillProcess(LPCSTR pszClassName, LPCSTR pszWindowTitle)
{
	HANDLE hProcessHandle;  //
	ULONG nProcessID;
	HWND TheWindow;

	TheWindow = ::FindWindow(NULL, pszWindowTitle);
	::GetWindowThreadProcessId(TheWindow, &nProcessID);
	hProcessHandle = ::OpenProcess(PROCESS_TERMINATE, FALSE, nProcessID);
	return ::TerminateProcess(hProcessHandle, 4);
}
/*******************************************************************************************
* Function Name : OnClose                                                                  *
* Description  :  �رս��̺���                                                             *
* Date   : 03/3/20                                                                         *
* Parameter     : ��                                                                       *
* Return Code  :  void                                                                     *
* Author   :   He Hao                                                                      *
*******************************************************************************************/
void MainDialog::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	KillProcess(NULL, "Server"); //�������̣��������رպ�̨���н������У���
	CDialog::OnClose();
}
/*******************************************************************************************
* Function Name : PreTranslateMessage                                                      *
* Description  :  ����س���ESC��Ӧ                                                        *
* Date   : 03/3/20                                                                         *
* Parameter     : ��                                                                       *
* Return Code  :  void                                                                     *
* Author   :   He Hao                                                                      *
*******************************************************************************************/
MainDialog::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		switch (pMsg->wParam)
		{
		case VK_RETURN: //�س�
			return TRUE;
		case VK_ESCAPE: //ESC  
			return TRUE;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}
/*******************************************************************************************
* Function Name : OnDblclkImageList                                                        *
* Description  :  ˫����Ӧ������ɾ���Ի���                                                 *
* Date   : 03/3/20                                                                         *
* Parameter     : ��                                                                       *
* Return Code  :  void                                                                     *
* Author   :   He Hao                                                                      *
*******************************************************************************************/
void MainDialog::OnDblclkImageList(NMHDR* pNMHDR, LRESULT* pResult)
{
	//��ֹѡ��հ��г�����Ӧ��Ϣ
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	NM_LISTVIEW *pNMListView = (NM_LISTVIEW *)pNMHDR;
	int nItem = pNMListView->iItem;
	if (nItem >= 0 && nItem < m_image_list.GetItemCount())
	{
		//���ѡ����������
		POSITION pos;
		pos=m_image_list.GetFirstSelectedItemPosition();
		int nItem=m_image_list.GetNextSelectedItem(pos);
		DelPicture delpictureDlg;
		delpictureDlg.DoModal();
		if(g_refresh=="REFRESH")
		{
			m_image_list.DeleteItem(nItem);
			g_refresh="";
		}
		g_picPath="recevied_picture\\";
	}
	*pResult = 0;
}

