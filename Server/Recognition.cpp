/**************************************************************************************************
* File Name  : Recognition.cpp                                                                    *
* Created   : 15/03/23                                                                            *
* Author   :   He  hao                                                                            *
* Model   : 05YOP                                                                                 *
* Description  :   [[CN]] 识别界面 [[CN]]                                                         *
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
* Description  :  初始化                                                                   *
* Date   : 03/3/20                                                                         *
* Parameter     : 无                                                                       *
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
	//设置静态控件窗口风格为位图居中显示
	CRect rect;
	pWnd->GetClientRect(&rect);
	pWnd->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE);
	//显示图片
	pWnd->SetBitmap((HBITMAP)::LoadImage(NULL,
		_T(g_picPath),  //资源号或本地文件名
		IMAGE_BITMAP,       //装载位图 IMAGE_CURSOR光标 IMAGE_ICON图标
		rect.Width(),                  //宽度 0为默认大小
		rect.Height(),                  //高度 像素为单位
		LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE));

	g_MyDone.DoneLocate = false;
	g_MyDone.DoneGray = false;
	g_MyDone.DoneBinaryZ = false;
	g_MyDone.DoneDenois = false;
	g_MyDone.DoneSegm = false;

	g_CarPlateLocate.ClearData();
	g_CarPlateRecog.ClearData();

	CString picName=g_picPath.Right(g_picPath.GetLength()-(g_picPath.ReverseFind('/')+1)); //从路径中分离文件名称
	CString shootDate=picName.Left(picName.GetLength()-4);	//剔除后缀
	m_shootDate=shootDate;
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

/*******************************************************************************************
* Function Name : OnButtonLocate                                                           *
* Description  :  定位                                                                     *
* Date   : 03/3/20                                                                         *
* Parameter     : 无                                                                       *
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
		MessageBox("定位失败!", "Error", MB_OK);
		return;
	}
	cvSaveImage("temp\\carPlateImg.bmp", g_CarPlateLocate.GetCarPlateImg());

	//获取对话框上图片控件的句柄
	CStatic *pWnd = (CStatic*)GetDlgItem(IDC_STATIC_CarPlateImg);
	//设置静态控件窗口风格为位图居中显示
	pWnd->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE);
	//显示图片
	pWnd->SetBitmap((HBITMAP)::LoadImage(NULL,
		_T("temp\\carPlateImg.bmp"),  //资源号或本地文件名
		IMAGE_BITMAP,       //装载位图 IMAGE_CURSOR光标 IMAGE_ICON图标
		0,                  //宽度 0为默认大小
		0,                  //高度 像素为单位
		LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE));

	g_MyDone.DoneLocate = true;
}

/*******************************************************************************************
* Function Name : OnButtonDoGray                                                           *
* Description  :  灰度化                                                                     *
* Date   : 03/3/20                                                                         *
* Parameter     : 无                                                                       *
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
		//设置静态控件窗口风格为位图居中显示
		pWnd->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE);
		//显示图片
		pWnd->SetBitmap((HBITMAP)::LoadImage(NULL,
			_T("temp\\grayImg.bmp"),  //资源号或本地文件名
			IMAGE_BITMAP,       //装载位图 IMAGE_CURSOR光标 IMAGE_ICON图标
			0,                  //宽度 0为默认大小
			0,                  //高度 像素为单位
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
* Description  :  二值化                                                                   *
* Date   : 03/3/20                                                                         *
* Parameter     : 无                                                                       *
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
		//设置静态控件窗口风格为位图居中显示
		pWnd->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE);
		//显示图片
		pWnd->SetBitmap((HBITMAP)::LoadImage(NULL,
			_T("temp\\binaryZImg.bmp"),  //资源号或本地文件名
			IMAGE_BITMAP,       //装载位图 IMAGE_CURSOR光标 IMAGE_ICON图标
			0,                  //宽度 0为默认大小
			0,                  //高度 像素为单位
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
* Description  :  去噪                                                                     *
* Date   : 03/3/20                                                                         *
* Parameter     : 无                                                                       *
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
		//设置静态控件窗口风格为位图居中显示
		pWnd->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE);
		//显示图片
		pWnd->SetBitmap((HBITMAP)::LoadImage(NULL,
			_T("temp\\denoisImg.bmp"),  //资源号或本地文件名
			IMAGE_BITMAP,       //装载位图 IMAGE_CURSOR光标 IMAGE_ICON图标
			0,                  //宽度 0为默认大小
			0,                  //高度 像素为单位
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
* Description  :  切割                                                                     *
* Date   : 03/3/20                                                                         *
* Parameter     : 无                                                                       *
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
			MessageBox("分割失败!", "Error", MB_OK);
			return;
		}

		CStatic *pWnd = (CStatic*)GetDlgItem(IDC_PIC_1);
		//设置静态控件窗口风格为位图居中显示
		CRect rect;
		pWnd->GetClientRect(&rect);
		pWnd->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE);
		//显示图片
		pWnd->SetBitmap((HBITMAP)::LoadImage(NULL,
			_T("temp\\wnd_1.bmp"),  //资源号或本地文件名
			IMAGE_BITMAP,       //装载位图 IMAGE_CURSOR光标 IMAGE_ICON图标
			rect.Width(),                  //宽度 0为默认大小
			rect.Height(),                  //高度 像素为单位
			LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE));

		pWnd = (CStatic*)GetDlgItem(IDC_PIC_2);
		//设置静态控件窗口风格为位图居中显示
		pWnd->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE);
		pWnd->GetClientRect(&rect);
		//显示图片
		pWnd->SetBitmap((HBITMAP)::LoadImage(NULL,
			_T("temp\\wnd_2.bmp"),  //资源号或本地文件名
			IMAGE_BITMAP,       //装载位图 IMAGE_CURSOR光标 IMAGE_ICON图标
			rect.Width(),                  //宽度 0为默认大小
			rect.Height(),                  //高度 像素为单位
			LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE));

		pWnd = (CStatic*)GetDlgItem(IDC_PIC_3);
		//设置静态控件窗口风格为位图居中显示
		pWnd->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE);
		pWnd->GetClientRect(&rect);
		//显示图片
		pWnd->SetBitmap((HBITMAP)::LoadImage(NULL,
			_T("temp\\wnd_3.bmp"),  //资源号或本地文件名
			IMAGE_BITMAP,       //装载位图 IMAGE_CURSOR光标 IMAGE_ICON图标
			rect.Width(),                  //宽度 0为默认大小
			rect.Height(),                  //高度 像素为单位
			LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE));

		pWnd = (CStatic*)GetDlgItem(IDC_PIC_4);
		//设置静态控件窗口风格为位图居中显示
		pWnd->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE);
		pWnd->GetClientRect(&rect);
		//显示图片
		pWnd->SetBitmap((HBITMAP)::LoadImage(NULL,
			_T("temp\\wnd_4.bmp"),  //资源号或本地文件名
			IMAGE_BITMAP,       //装载位图 IMAGE_CURSOR光标 IMAGE_ICON图标
			rect.Width(),                  //宽度 0为默认大小
			rect.Height(),                  //高度 像素为单位
			LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE));

		pWnd = (CStatic*)GetDlgItem(IDC_PIC_5);
		//设置静态控件窗口风格为位图居中显示
		pWnd->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE);
		pWnd->GetClientRect(&rect);
		//显示图片
		pWnd->SetBitmap((HBITMAP)::LoadImage(NULL,
			_T("temp\\wnd_5.bmp"),  //资源号或本地文件名
			IMAGE_BITMAP,       //装载位图 IMAGE_CURSOR光标 IMAGE_ICON图标
			rect.Width(),                  //宽度 0为默认大小
			rect.Height(),                  //高度 像素为单位
			LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE));

		pWnd = (CStatic*)GetDlgItem(IDC_PIC_6);
		//设置静态控件窗口风格为位图居中显示
		pWnd->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE);
		pWnd->GetClientRect(&rect);
		//显示图片
		pWnd->SetBitmap((HBITMAP)::LoadImage(NULL,
			_T("temp\\wnd_6.bmp"),  //资源号或本地文件名
			IMAGE_BITMAP,       //装载位图 IMAGE_CURSOR光标 IMAGE_ICON图标
			rect.Width(),                  //宽度 0为默认大小
			rect.Height(),                 //高度 像素为单位
			LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE));

		pWnd = (CStatic*)GetDlgItem(IDC_PIC_7);
		//设置静态控件窗口风格为位图居中显示
		pWnd->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE);
		pWnd->GetClientRect(&rect);
		//显示图片
		pWnd->SetBitmap((HBITMAP)::LoadImage(NULL,
			_T("temp\\wnd_7.bmp"), //资源号或本地文件名
			IMAGE_BITMAP,       //装载位图 IMAGE_CURSOR光标 IMAGE_ICON图标
			rect.Width(),                  //宽度 0为默认大小
			rect.Height(),                 //高度 像素为单位
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
* Description  :  定位                                                                     *
* Date   : 03/3/20                                                                         *
* Parameter     : 无                                                                       *
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
* Description  :  保存                                                                     *
* Date   : 03/3/20                                                                         *
* Parameter     : 无                                                                       *
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
		MessageBox("添加失败");
	}
	else
	{
		MessageBox("添加成功");
	}
	CString picName=g_picPath.Right(g_picPath.GetLength()-(g_picPath.ReverseFind('/')+1));//从路径中分离文件名称
	
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
