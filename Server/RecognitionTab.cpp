/**************************************************************************************************
* File Name  : RecognitionTab.cpp                                                                 *
* Created   : 03/08/29                                                                            *
* Author   :   He  hao                                                                            *
* Model   : 05YOP                                                                                 *
* Description  :  ʶ���Ӵ���                                                                      *
**************************************************************************************************/

#include "stdafx.h"
#include "Server.h"
#include "CarPlateLocate.h"
#include "CarPlateRecog.h"
#include "RecognitionTab.h"
#include "ViolationRecordset.h"

#ifndef VM_MYRECEVIEDPICMSG
#define VM_MYRECEVIEDPICMSG 102
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// RecognitionTab dialog


RecognitionTab::RecognitionTab(CWnd* pParent /*=NULL*/)
: CDialog(RecognitionTab::IDD, pParent)
{
	//{{AFX_DATA_INIT(RecognitionTab)
	m_plateInfo = _T("");
	m_shootDate = _T("");
	//}}AFX_DATA_INIT
}


void RecognitionTab::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(RecognitionTab)
	DDX_Control(pDX, IDC_PICTURE, m_picture);
	DDX_Text(pDX, IDC_EDIT1, m_plateInfo);
	DDX_Text(pDX, IDC_EDIT2, m_shootDate);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(RecognitionTab, CDialog)
//{{AFX_MSG_MAP(RecognitionTab)
ON_BN_CLICKED(IDC_BUTTON_RECOGNITION, OnButtonRecognition)
ON_BN_CLICKED(IDC_BUTTON_KEYRECOGNITION, OnButtonKeyrecognition)
ON_BN_CLICKED(IDC_BUTTON_SAVE, OnButtonSave)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// RecognitionTab message handlers

/*******************************************************************************************
* Function Name : OnButtonRecognition                                                      *
* Description  :  �򿪷ֲ�ʶ�����                                                         *
* Date   : 03/3/20                                                                         *
* Parameter     : ��                                                                       *
* Return Code  :  void                                                                     *
* Author   :    He  hao                                                                    *
*******************************************************************************************/
void RecognitionTab::OnButtonRecognition() 
{
	// TODO: Add your control notification handler code here
	if(g_picPath=="recevied_picture//")
	{
		AfxMessageBox("��ѡ��ͼƬ");
	}
	else
	{
		Recognition m_recognition;
		m_recognition.DoModal();
	}
}
/*******************************************************************************************
* Function Name : OnButtonKeyrecognition                                                   *
* Description  :  һ��ʶ��                                                                 *
* Date   : 03/3/20                                                                         *
* Parameter     : ��                                                                       *
* Return Code  :  void                                                                     *
* Author   :    He  hao                                                                    *
*******************************************************************************************/
void RecognitionTab::OnButtonKeyrecognition() 
{
	// TODO: Add your control notification handler code here
	if(g_picPath=="recevied_picture//")
	{
		AfxMessageBox("��ѡ��ͼƬ");
	}
	else
	{
		string filePath=g_picPath.GetBuffer(0);
		char fileName[100]={0};
		filePath.copy(fileName,filePath.size(),0);
		CarPlateLocate myCarPlateLocate(fileName);
		myCarPlateLocate.LocateCarPlate(myCarPlateLocate.GetSrcImg());
		CarPlateRecog myCarPlateRecog(myCarPlateLocate.GetCarPlateImg());
		myCarPlateRecog.SegmPlateChar(myCarPlateRecog.GetCarPlateImg());
		myCarPlateRecog.PlateCharRecog(myCarPlateRecog.GetPlateCharImgs());
		
		CString fuckText;
		fuckText.Format("%d",myCarPlateRecog.fuck);
		m_plateInfo=myCarPlateRecog.GetPlateChars();
		
		if(myCarPlateLocate.GetErrorEnum()==PositionException)
		{
			MessageBox("��λʧ��!","Error",MB_OK);
			return;
		}
		else if(myCarPlateRecog.GetErrorEnum()==SegmException)
		{
			MessageBox("�и�ʧ��!","Error",MB_OK);
			return;
		}
		
		CString picName=g_picPath.Right(g_picPath.GetLength()-(g_picPath.ReverseFind('/')+1)); //��·���з����ļ�����
		CString shootDate=picName.Left(picName.GetLength()-4);	//�޳���׺
		m_shootDate=shootDate;

		UpdateData(FALSE);
		
		myCarPlateLocate.ClearData();
		myCarPlateRecog.ClearData();
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
void RecognitionTab::OnButtonSave() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	ViolationRecordset set;
	if(m_shootDate==""||m_plateInfo=="")
	{
		MessageBox("��ϢΪ��");
		return;
	}


	if (!set.AddRecord(m_plateInfo,m_shootDate))
	{
		MessageBox("���ʧ��");
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
