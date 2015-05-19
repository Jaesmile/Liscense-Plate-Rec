// MainDialog.cpp : implementation file

#include "stdafx.h"
#include "Client.h"
#include "MainDialog.h"
#include "ClientSocket.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define VM_MYMESSAGE 105
/////////////////////////////////////////////////////////////////////////////
// MainDialog dialog

extern int nSize;
MainDialog::MainDialog(CWnd *pParent /*=NULL*/)
	: CDialog(MainDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(MainDialog)
	m_process = _T("");
	//}}AFX_DATA_INIT
}

void MainDialog::DoDataExchange(CDataExchange *pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(MainDialog)
	DDX_Control(pDX, IDC_PROGRESS1, m_progress);
	DDX_Control(pDX, IDC_SEND_LIST, m_sendList);
	DDX_Text(pDX, IDC_EDIT1, m_process);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(MainDialog, CDialog)
	//{{AFX_MSG_MAP(MainDialog)
	ON_BN_CLICKED(IDC_BUTTON_SCAN, OnButtonScan)
	ON_NOTIFY(NM_CLICK, IDC_SEND_LIST, OnClickSendList)
	ON_BN_CLICKED(IDC_BUTTON_SEND, OnButtonSend)
	ON_MESSAGE(VM_MYMESSAGE, HandleMessage)
	ON_BN_CLICKED(IDC_BUTTON_STOP_SEND, OnButtonStopSend)
	ON_NOTIFY(NM_DBLCLK, IDC_SEND_LIST, OnDblclkSendList)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, OnButtonClose)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// MainDialog message handlers

BOOL MainDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_sendList.InsertColumn(0, "文件名", LVCFMT_LEFT, 200, 0);
	m_sendList.InsertColumn(1, "文件路径", LVCFMT_LEFT, 210, 0);

	hwnd = m_hWnd;
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void MainDialog::OnButtonScan()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CString pathName, fileName, fileTitle, fileExt;
	DWORD NewStyle = LVS_EX_FULLROWSELECT;
	m_sendList.SetExtendedStyle(NewStyle);

	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_ALLOWMULTISELECT | OFN_ENABLESIZING | OFN_FILEMUSTEXIST,
		("PICTURE Files(*.BMP)|*.bmp;*.BMP||"),
		NULL);

	dlg.m_ofn.nMaxFile = 500 * MAX_PATH;

	char* ch = new TCHAR[dlg.m_ofn.nMaxFile];
	dlg.m_ofn.lpstrFile = ch;
	ZeroMemory(dlg.m_ofn.lpstrFile, sizeof(TCHAR) * dlg.m_ofn.nMaxFile);

	int retval = dlg.DoModal();
	POSITION pos_file = NULL;
	pos_file = dlg.GetStartPosition();

	while (pos_file != NULL)
	{
		int m = 0;
		pathName = dlg.GetNextPathName(pos_file);
		int length = pathName.GetLength();

		if (length == 0)
		{
			break;
		}

		for (int i = length - 1; i != 0; i--)
		{
			if (pathName[i] == '\\')
			{
				fileName = pathName.Right(length - i - 1);
				break;
			}
		}
		for (int n = length - 1; n != 0; n--)
		{

			if (pathName[n] == '.')
			{
				fileExt = pathName.Right(length - n - 1);
				break;
			}
		}
		if (fileExt == "BMP" || fileExt == "bmp")
		{
			int temp_list = m_sendList.InsertItem(m, fileName);
			m_sendList.SetItemText(temp_list, 1, pathName);
			picPathVector.push_back(pathName);
		}
	}
	delete[] ch;
}

void MainDialog::OnClickSendList(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
}

void MainDialog::OnButtonSend()
{
	m_progress.SetPos(0);
	int presentCount = 1;
	int totalCount = picPathVector.size();
	while (!picPathVector.empty())
	{
		ClientSocket socket;
		if (socket.CreateSocket(IPadd, port) == 0)
		{
			socket.SendPictureToServer(picPathVector.back());
			DeleteFile(picPathVector.back());
			CString str;
			str.Format("%d/%d", presentCount++, totalCount);
			m_process = str;
			UpdateData(FALSE);

			picPathVector.pop_back();
			socket.CloseSocket();

			if (picPathVector.empty())
			{
				m_sendList.DeleteItem(0);
				m_process = "";
				return;
			}
			m_sendList.DeleteItem(0);
		}
		else
		{
			AfxMessageBox("连接失败");
			break;
		}
		Sleep(100);
	}
}

void MainDialog::HandleMessage(WPARAM wParam, LPARAM lParam)
{
	m_progress.SetRange32(0, nSize);
	m_progress.SetPos(0);
	m_progress.SetStep(nSize);
	m_progress.StepIt();
}

void MainDialog::OnButtonStopSend()
{
	picPathVector.clear();
	m_sendList.DeleteAllItems();
}

void MainDialog::OnDblclkSendList(NMHDR* pNMHDR, LRESULT* pResult)
{
	POSITION pos = m_sendList.GetFirstSelectedItemPosition();
	int nItem = m_sendList.GetNextSelectedItem(pos);
	picPathVector.erase(picPathVector.begin()+picPathVector.size()-1-nItem);
	m_sendList.DeleteItem(nItem);

	*pResult = 0;
}

void MainDialog::OnButtonClose() 
{
	OnOK();
}
