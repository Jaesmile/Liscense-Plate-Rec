/**************************************************************************************************
* File Name  : Recognition.cpp                                                                    *
* Created   : 15/03/23                                                                            *
* Author   :   He  hao                                                                            *
* Model   : 05YOP                                                                                 *
* Description  :   [[CN]] ʶ����� [[CN]]                                                         *
**************************************************************************************************/

#include "stdafx.h"
#include "Server.h"
#include "Recognition.h"
#include "MyType.h"
#include "CarPlateLocate.h"
#include "CarPlateRecog.h"
#include "ViolationRecordset.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifndef VM_MYRECEVIEDPICMSG
#define VM_MYRECEVIEDPICMSG 102
#endif

/////////////////////////////////////////////////////////////////////////////
// Recognition dialog
char fileName[100] = { 0 };
CarPlateLocate g_CarPlateLocate;
CarPlateRecog g_CarPlateRecog;
Choice_Done g_MyDone;

Recognition::Recognition(CWnd* pParent /*=NULL*/)
	: CDialog(Recognition::IDD, pParent)
{
	//{{AFX_DATA_INIT(Recognition)
	m_plateInfo = _T("");
	m_ratio = _T("");
	m_shootDate = _T("");
	//}}AFX_DATA_INIT
}


void Recognition::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(Recognition)
	DDX_Text(pDX, IDC_EDIT1, m_plateInfo);
	DDX_Text(pDX, IDC_EDIT3, m_ratio);
	DDX_Text(pDX, IDC_EDIT2, m_shootDate);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(Recognition, CDialog)
	//{{AFX_MSG_MAP(Recognition)
	ON_BN_CLICKED(IDC_BUTTON_LOCATE,OnButtonLocate)
	ON_BN_CLICKED(IDC_BUTTON_DOGRAY,OnButtonDoGray)
	ON_BN_CLICKED(IDC_BUTTON_INARYZ,OnButtonDoBinaryZ)
	ON_BN_CLICKED(IDC_BUTTON_ENOIS,OnButtonDoDenois)
	ON_BN_CLICKED(IDC_BUTTON_SEG,OnButtonSegmPlateChar)
	ON_BN_CLICKED(IDC_BUTTON_RECOG,OnButtonPlateCharRecog)
	ON_BN_CLICKED(IDC_BUTTON_SAVE,OnButtonSave)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Recognition message handlers

/*******************************************************************************************
* Function Name : OnInitDialog                                                             *
* Description  :  ��ʼ��                                                                   *
* Date   : 03/3/20                                                                         *
* Parameter     : ��                                                                       *
* Return Code  :  void                                                                     *
* Author   :   Chen xi ming                                                                *
*******************************************************************************************/
BOOL Recognition::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	char copyZ[100] = { 0 };
	memcpy(fileName, copyZ, 100);
	string tempName = g_picPath.GetBuffer(0);
	tempName.copy(fileName, tempName.size(), 0);

	CStatic *pWnd = (CStatic*)GetDlgItem(IDC_STATIC_SrcImg);
	//���þ�̬�ؼ����ڷ��Ϊλͼ������ʾ
	CRect rect;
	pWnd->GetClientRect(&rect);
	pWnd->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE);
	//��ʾͼƬ
	pWnd->SetBitmap((HBITMAP)::LoadImage(NULL,
		_T(g_picPath),  //��Դ�Ż򱾵��ļ���
		IMAGE_BITMAP,       //װ��λͼ IMAGE_CURSOR��� IMAGE_ICONͼ��
		rect.Width(),                  //��� 0ΪĬ�ϴ�С
		rect.Height(),                  //�߶� ����Ϊ��λ
		LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE));

	g_MyDone.DoneLocate = false;
	g_MyDone.DoneGray = false;
	g_MyDone.DoneBinaryZ = false;
	g_MyDone.DoneDenois = false;
	g_MyDone.DoneSegm = false;

	g_CarPlateLocate.ClearData();
	g_CarPlateRecog.ClearData();

	CString picName=g_picPath.Right(g_picPath.GetLength()-(g_picPath.ReverseFind('/')+1)); //��·���з����ļ�����
	CString shootDate=picName.Left(picName.GetLength()-4);	//�޳���׺
	m_shootDate=shootDate;
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

/*******************************************************************************************
* Function Name : OnButtonLocate                                                           *
* Description  :  ��λ                                                                     *
* Date   : 03/3/20                                                                         *
* Parameter     : ��                                                                       *
* Return Code  :  void                                                                     *
* Author   :   Chen xi ming                                                                *
*******************************************************************************************/
void Recognition::OnButtonLocate()
{
	
	CarPlateLocate myCarPlateLocate(fileName);

	myCarPlateLocate.LocateCarPlate(myCarPlateLocate.GetSrcImg());
	g_CarPlateLocate = myCarPlateLocate;
	if (g_CarPlateLocate.GetErrorEnum() == PositionException)
	{
		MessageBox("��λʧ��!", "Error", MB_OK);
		return;
	}
	cvSaveImage("temp\\carPlateImg.bmp", g_CarPlateLocate.GetCarPlateImg());

	//��ȡ�Ի�����ͼƬ�ؼ��ľ��
	CStatic *pWnd = (CStatic*)GetDlgItem(IDC_STATIC_CarPlateImg);
	//���þ�̬�ؼ����ڷ��Ϊλͼ������ʾ
	pWnd->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE);
	//��ʾͼƬ
	pWnd->SetBitmap((HBITMAP)::LoadImage(NULL,
		_T("temp\\carPlateImg.bmp"),  //��Դ�Ż򱾵��ļ���
		IMAGE_BITMAP,       //װ��λͼ IMAGE_CURSOR��� IMAGE_ICONͼ��
		0,                  //��� 0ΪĬ�ϴ�С
		0,                  //�߶� ����Ϊ��λ
		LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE));

	g_MyDone.DoneLocate = true;
}

/*******************************************************************************************
* Function Name : OnButtonDoGray                                                           *
* Description  :  �ҶȻ�                                                                     *
* Date   : 03/3/20                                                                         *
* Parameter     : ��                                                                       *
* Return Code  :  void                                                                     *
* Author   :   Chen xi ming                                                                *
*******************************************************************************************/
void Recognition::OnButtonDoGray()
{
	// TODO: Add your control notification handler code here
	if (g_MyDone.DoneLocate)
	{
		CarPlateRecog myCarPlateRecog(g_CarPlateLocate.GetCarPlateImg());
		g_CarPlateRecog.SetCarPlateImg(myCarPlateRecog.GetCarPlateImg());
		g_CarPlateRecog.DoGray(g_CarPlateRecog.GetCarPlateImg());
		cvSaveImage("temp\\grayImg.bmp", g_CarPlateRecog.GetGrayImg());

		CStatic *pWnd = (CStatic*)GetDlgItem(IDC_PIC_9);
		//���þ�̬�ؼ����ڷ��Ϊλͼ������ʾ
		pWnd->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE);
		//��ʾͼƬ
		pWnd->SetBitmap((HBITMAP)::LoadImage(NULL,
			_T("temp\\grayImg.bmp"),  //��Դ�Ż򱾵��ļ���
			IMAGE_BITMAP,       //װ��λͼ IMAGE_CURSOR��� IMAGE_ICONͼ��
			0,                  //��� 0ΪĬ�ϴ�С
			0,                  //�߶� ����Ϊ��λ
			LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE));

		g_MyDone.DoneGray = true;
	}// end if()
	else
	{
		OnButtonLocate();
		if (g_CarPlateLocate.GetErrorEnum() == IOException)
		{
			return;
		}
		else if (g_CarPlateLocate.GetErrorEnum() == FileTypeException)
		{
			return;
		}
		else if (g_CarPlateLocate.GetErrorEnum() == PositionException)
		{
			return;
		}
		OnButtonDoGray();
	}
}
/*******************************************************************************************
* Function Name : OnButtonDoBinaryZ                                                        *
* Description  :  ��ֵ��                                                                   *
* Date   : 03/3/20                                                                         *
* Parameter     : ��                                                                       *
* Return Code  :  void                                                                     *
* Author   :   Chen xi ming                                                                *
*******************************************************************************************/
void Recognition::OnButtonDoBinaryZ()
{
	// TODO: Add your control notification handler code here
	if (g_MyDone.DoneGray)
	{
		g_CarPlateRecog.DoBinaryZ(g_CarPlateRecog.GetGrayImg());
		cvSaveImage("temp\\binaryZImg.bmp", g_CarPlateRecog.GetBinaryZImg());

		CStatic *pWnd = (CStatic*)GetDlgItem(IDC_PIC_9);
		//���þ�̬�ؼ����ڷ��Ϊλͼ������ʾ
		pWnd->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE);
		//��ʾͼƬ
		pWnd->SetBitmap((HBITMAP)::LoadImage(NULL,
			_T("temp\\binaryZImg.bmp"),  //��Դ�Ż򱾵��ļ���
			IMAGE_BITMAP,       //װ��λͼ IMAGE_CURSOR��� IMAGE_ICONͼ��
			0,                  //��� 0ΪĬ�ϴ�С
			0,                  //�߶� ����Ϊ��λ
			LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE));

		g_MyDone.DoneBinaryZ = true;
	}
	else
	{
		OnButtonDoGray();
		if (g_CarPlateLocate.GetErrorEnum() == PositionException)
		{
			return;
		}
		OnButtonDoBinaryZ();
	}
}

/*******************************************************************************************
* Function Name : OnButtonDoDenois                                                         *
* Description  :  ȥ��                                                                     *
* Date   : 03/3/20                                                                         *
* Parameter     : ��                                                                       *
* Return Code  :  void                                                                     *
* Author   :   Chen xi ming                                                                *
*******************************************************************************************/
void Recognition::OnButtonDoDenois()
{
	// TODO: Add your control notification handler code here
	if (g_MyDone.DoneBinaryZ)
	{
		g_CarPlateRecog.DoDenois(g_CarPlateRecog.GetBinaryZImg());
		cvSaveImage("temp\\denoisImg.bmp", g_CarPlateRecog.GetDenoisImg());

		CStatic *pWnd = (CStatic*)GetDlgItem(IDC_PIC_9);
		//���þ�̬�ؼ����ڷ��Ϊλͼ������ʾ
		pWnd->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE);
		//��ʾͼƬ
		pWnd->SetBitmap((HBITMAP)::LoadImage(NULL,
			_T("temp\\denoisImg.bmp"),  //��Դ�Ż򱾵��ļ���
			IMAGE_BITMAP,       //װ��λͼ IMAGE_CURSOR��� IMAGE_ICONͼ��
			0,                  //��� 0ΪĬ�ϴ�С
			0,                  //�߶� ����Ϊ��λ
			LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE));

		g_MyDone.DoneDenois = true;
	}
	else
	{
		OnButtonDoBinaryZ();
		if (g_CarPlateLocate.GetErrorEnum() == PositionException)
		{
			return;
		}
		OnButtonDoDenois();
	}
}

/*******************************************************************************************
* Function Name : OnButtonSegmPlateCh                                                      *
* Description  :  �и�                                                                     *
* Date   : 03/3/20                                                                         *
* Parameter     : ��                                                                       *
* Return Code  :  void                                                                     *
* Author   :   Chen xi ming                                                                *
*******************************************************************************************/
void Recognition::OnButtonSegmPlateChar()
{
	// TODO: Add your control notification handler code here
	if (g_MyDone.DoneBinaryZ)
	{
		CarPlateRecog myCarPlateRecog(g_CarPlateLocate.GetCarPlateImg());
		myCarPlateRecog.SegmPlateChar(myCarPlateRecog.GetCarPlateImg());
		g_CarPlateRecog = myCarPlateRecog;
		if (g_CarPlateRecog.GetErrorEnum() == SegmException)
		{
			MessageBox("�ָ�ʧ��!", "Error", MB_OK);
			return;
		}

		CStatic *pWnd = (CStatic*)GetDlgItem(IDC_PIC_1);
		//���þ�̬�ؼ����ڷ��Ϊλͼ������ʾ
		CRect rect;
		pWnd->GetClientRect(&rect);
		pWnd->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE);
		//��ʾͼƬ
		pWnd->SetBitmap((HBITMAP)::LoadImage(NULL,
			_T("temp\\wnd_1.bmp"),  //��Դ�Ż򱾵��ļ���
			IMAGE_BITMAP,       //װ��λͼ IMAGE_CURSOR��� IMAGE_ICONͼ��
			rect.Width(),                  //��� 0ΪĬ�ϴ�С
			rect.Height(),                  //�߶� ����Ϊ��λ
			LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE));

		pWnd = (CStatic*)GetDlgItem(IDC_PIC_2);
		//���þ�̬�ؼ����ڷ��Ϊλͼ������ʾ
		pWnd->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE);
		pWnd->GetClientRect(&rect);
		//��ʾͼƬ
		pWnd->SetBitmap((HBITMAP)::LoadImage(NULL,
			_T("temp\\wnd_2.bmp"),  //��Դ�Ż򱾵��ļ���
			IMAGE_BITMAP,       //װ��λͼ IMAGE_CURSOR��� IMAGE_ICONͼ��
			rect.Width(),                  //��� 0ΪĬ�ϴ�С
			rect.Height(),                  //�߶� ����Ϊ��λ
			LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE));

		pWnd = (CStatic*)GetDlgItem(IDC_PIC_3);
		//���þ�̬�ؼ����ڷ��Ϊλͼ������ʾ
		pWnd->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE);
		pWnd->GetClientRect(&rect);
		//��ʾͼƬ
		pWnd->SetBitmap((HBITMAP)::LoadImage(NULL,
			_T("temp\\wnd_3.bmp"),  //��Դ�Ż򱾵��ļ���
			IMAGE_BITMAP,       //װ��λͼ IMAGE_CURSOR��� IMAGE_ICONͼ��
			rect.Width(),                  //��� 0ΪĬ�ϴ�С
			rect.Height(),                  //�߶� ����Ϊ��λ
			LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE));

		pWnd = (CStatic*)GetDlgItem(IDC_PIC_4);
		//���þ�̬�ؼ����ڷ��Ϊλͼ������ʾ
		pWnd->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE);
		pWnd->GetClientRect(&rect);
		//��ʾͼƬ
		pWnd->SetBitmap((HBITMAP)::LoadImage(NULL,
			_T("temp\\wnd_4.bmp"),  //��Դ�Ż򱾵��ļ���
			IMAGE_BITMAP,       //װ��λͼ IMAGE_CURSOR��� IMAGE_ICONͼ��
			rect.Width(),                  //��� 0ΪĬ�ϴ�С
			rect.Height(),                  //�߶� ����Ϊ��λ
			LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE));

		pWnd = (CStatic*)GetDlgItem(IDC_PIC_5);
		//���þ�̬�ؼ����ڷ��Ϊλͼ������ʾ
		pWnd->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE);
		pWnd->GetClientRect(&rect);
		//��ʾͼƬ
		pWnd->SetBitmap((HBITMAP)::LoadImage(NULL,
			_T("temp\\wnd_5.bmp"),  //��Դ�Ż򱾵��ļ���
			IMAGE_BITMAP,       //װ��λͼ IMAGE_CURSOR��� IMAGE_ICONͼ��
			rect.Width(),                  //��� 0ΪĬ�ϴ�С
			rect.Height(),                  //�߶� ����Ϊ��λ
			LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE));

		pWnd = (CStatic*)GetDlgItem(IDC_PIC_6);
		//���þ�̬�ؼ����ڷ��Ϊλͼ������ʾ
		pWnd->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE);
		pWnd->GetClientRect(&rect);
		//��ʾͼƬ
		pWnd->SetBitmap((HBITMAP)::LoadImage(NULL,
			_T("temp\\wnd_6.bmp"),  //��Դ�Ż򱾵��ļ���
			IMAGE_BITMAP,       //װ��λͼ IMAGE_CURSOR��� IMAGE_ICONͼ��
			rect.Width(),                  //��� 0ΪĬ�ϴ�С
			rect.Height(),                 //�߶� ����Ϊ��λ
			LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE));

		pWnd = (CStatic*)GetDlgItem(IDC_PIC_7);
		//���þ�̬�ؼ����ڷ��Ϊλͼ������ʾ
		pWnd->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE);
		pWnd->GetClientRect(&rect);
		//��ʾͼƬ
		pWnd->SetBitmap((HBITMAP)::LoadImage(NULL,
			_T("temp\\wnd_7.bmp"), //��Դ�Ż򱾵��ļ���
			IMAGE_BITMAP,       //װ��λͼ IMAGE_CURSOR��� IMAGE_ICONͼ��
			rect.Width(),                  //��� 0ΪĬ�ϴ�С
			rect.Height(),                 //�߶� ����Ϊ��λ
			LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE));

		g_MyDone.DoneSegm = true;
	}// end if()
	else
	{
		OnButtonDoBinaryZ();
		if (g_CarPlateLocate.GetErrorEnum() == PositionException)
		{
			return;
		}
		OnButtonSegmPlateChar();
	}
}

/*******************************************************************************************
* Function Name : OnButtonPlateCharRecog                                                   *
* Description  :  ��λ                                                                     *
* Date   : 03/3/20                                                                         *
* Parameter     : ��                                                                       *
* Return Code  :  void                                                                     *
* Author   :   Chen xi ming                                                                *
*******************************************************************************************/
void Recognition::OnButtonPlateCharRecog()
{
	// TODO: Add your control notification handler code here
	if (g_MyDone.DoneSegm)
	{
		g_CarPlateRecog.PlateCharRecog(g_CarPlateRecog.GetPlateCharImgs());
		CString plateChar = g_CarPlateRecog.GetPlateChars();
		double recogRatio = g_CarPlateRecog.GetAvgBaiFenBi();
		CString recogRatioInfo;
		recogRatioInfo.Format("%lf", recogRatio * 100);
		recogRatioInfo += "%";

		m_ratio = recogRatioInfo;
		m_plateInfo = plateChar;
		UpdateData(FALSE);
	}
	else
	{
		OnButtonSegmPlateChar();
		if (g_CarPlateLocate.GetErrorEnum() == PositionException)
		{
			return;
		}
		if (g_CarPlateRecog.GetErrorEnum() == SegmException)
		{
			return;
		}
		OnButtonPlateCharRecog();
	}
}

/*******************************************************************************************
* Function Name : OnButtonSave                                                             *
* Description  :  ����                                                                     *
* Date   : 03/3/20                                                                         *
* Parameter     : ��                                                                       *
* Return Code  :  void                                                                     *
* Author   :   Huang rong  / He  hao                                                       *
*******************************************************************************************/
void Recognition::OnButtonSave()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	ViolationRecordset set;
	if (!set.AddRecord(m_plateInfo, m_shootDate))
	{
		MessageBox("���ʧ��");
	}
	else
	{
		MessageBox("��ӳɹ�");
	}
	CString picName=g_picPath.Right(g_picPath.GetLength()-(g_picPath.ReverseFind('/')+1));//��·���з����ļ�����
	
	MoveFile(g_picPath,"stored_picture//"+picName);
    if (MoveFile(g_picPath,"stored_picture//"+picName)!=1)
	{
		DeleteFile(g_picPath);
	}
	g_picPath="recevied_picture//";
	
	PostMessageW(g_hwnd,VM_MYRECEVIEDPICMSG,0,0);
	m_shootDate="";
	m_plateInfo="";
	UpdateData(FALSE);

}
