/*****************************************************************************************************
*
*	File Name : CarPlateRecog.cpp
*	Created : 2015/3/22
*	Author : ��ϣ����������
*	Description:����ʶ����Ķ����ļ��������˶Գ���ʶ�����Ա�����Ķ���
*
******************************************************************************************************/

#include "stdafx.h"
#include "CarPlateRecog.h"
#include <iostream>

//ȫ�ֱ���
static double g_recogSrcImgArea;//��Դͼ������
static double g_recogMinRectArea;//������С��������ڳ�ȥ�����С�ľ���
static double g_recogMaxRectArea;//���������������ڳ�ȥ�������ľ���
static int g_plateImgHeight;//ͼ��ĸ߶�
static int g_plateImgWidth;//ͼ��Ŀ��
static int g_topBoundary = 20;//�ϱ߽磬�ص�����ͼ���ϱ߽�����Ĳ���(ȥ��í���ͱ߿�)
static int g_bottomBoundary = 0; //�±߽磬�ص�����ͼ���±߽�����Ĳ���(ȥ���߿�)
static double g_leftBoundary;//��߽磬�ص�����ͼ����߽���ߵĲ���(��ֹ��߿�����Ϊ�ǳ����ַ�1)
static double g_rightBoundary;// //�ұ߽磬�ص�����ͼ���ұ߽��ұߵĲ���(��ֹ�ұ߿�����Ϊ�ǳ����ַ�1)
static const int g_heightBuf = 4;//�߶ȷ�Χ,��Ѱ�Ҹ߶����Ƶľ�����Ϊ�����ַ�����ʱ�����߶����ơ�
static const int g_mobanHeight = 40;//�ַ�ģ��ͼ��ĸ�
static const int g_mobanWidth = 20;//�ַ�ģ��ͼ��Ŀ�
static vector<CvRect> g_plateCharRects;//�����ַ�����
static IplImage* g_pQukuangImg;//ȥ���߿��ĳ���ͼ��

/*Ĭ�Ϲ��캯��*/
CarPlateRecog::CarPlateRecog()
{
	//��Ա������ֵ
	m_carPlateImg = NULL;
	m_grayImg = NULL;
	m_binaryZImg = NULL;
	m_denoisImg = NULL;
	m_plateCharInfo = "";
	m_recogErrorEnum = RecogNormal;
	m_avgBaiFenBi = 0;
	g_topBoundary = 20;
	g_bottomBoundary = 0;
}

/*���캯��*/
CarPlateRecog::CarPlateRecog(IplImage *pPlateImg)
{
	if (!pPlateImg)
	{
		m_carPlateImg = NULL;
		return;
	}

	//��Ա������ֵ
	m_carPlateImg = cvCreateImage(cvGetSize(pPlateImg), 8, 3);
	cvCopyImage(pPlateImg, m_carPlateImg);
	m_grayImg = NULL;
	m_binaryZImg = NULL;
	m_denoisImg = NULL;
	m_plateCharInfo = "";
	m_recogErrorEnum = RecogNormal;
	m_avgBaiFenBi = 0;

	//ȫ�ֱ�����ֵ
	g_recogSrcImgArea = cvGetSize(m_carPlateImg).height*cvGetSize(m_carPlateImg).width;
	g_plateImgHeight = cvGetSize(m_carPlateImg).height;
	g_plateImgWidth = cvGetSize(m_carPlateImg).width;
	g_topBoundary = 20;
	g_bottomBoundary = 0;
	g_leftBoundary = cvGetSize(m_carPlateImg).width / 9;
	g_rightBoundary = cvGetSize(m_carPlateImg).width*0.94;
	g_recogMinRectArea = g_recogSrcImgArea / 80;
	g_recogMaxRectArea = g_recogSrcImgArea / 5;
}

/*������ݺ���*/
void CarPlateRecog::ClearData()
{
	cvReleaseImage(&m_carPlateImg);
	cvReleaseImage(&m_grayImg);
	cvReleaseImage(&m_binaryZImg);
	cvReleaseImage(&m_denoisImg);
	m_carPlateImg = NULL;
	m_grayImg = NULL;
	m_binaryZImg = NULL;
	m_denoisImg = NULL;
	m_plateCharImgs.clear();
	g_plateCharRects.clear();
	m_plateCharInfo = "";
	m_avgBaiFenBi = 0;
	m_recogErrorEnum = RecogNormal;
}

/*********************************************************************************
*
*	�ҶȻ�������
*	Function Name:DoGray
*	Date:2015/3/23
*	Parameter:IplImage *pPlateImg(��Դͼ��)
*	Anthor:������
*
***********************************************************************************/
void CarPlateRecog::DoGray(IplImage *pPlateImg)
{
	if (!pPlateImg)
	{
		return;
	}

	IplImage *pImgGray = cvCreateImage(cvGetSize(pPlateImg), IPL_DEPTH_8U, 1);
	cvCvtColor(pPlateImg, pImgGray, CV_BGR2GRAY);
	m_grayImg = cvCreateImage(cvGetSize(pImgGray), IPL_DEPTH_8U, 1);
	cvCopyImage(pImgGray, m_grayImg);
}

/***********************************************************************************
*
*	��ֵ��������
*	Function Name:DoGray
*	Date:2015/3/23
*	Parameter:IplImage *pGrayImg(�ҶȻ��������ͼ��)
*	Anthor:������
*
***********************************************************************************/
void CarPlateRecog::DoBinaryZ(IplImage *pGrayImg)
{
	if (!pGrayImg)
	{
		return;
	}

	IplImage *pThreshold = cvCreateImage(cvGetSize(pGrayImg), IPL_DEPTH_8U, 1);
	cvThreshold(pGrayImg, pThreshold, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
	m_binaryZImg = cvCreateImage(cvGetSize(pThreshold), IPL_DEPTH_8U, 1);
	cvCopyImage(pThreshold, m_binaryZImg);
}

/***********************************************************************************
*
*	ȥ�뺯��
*	Function Name:DoGray
*	Date:2015/3/23
*	Parameter:IplImage *pBinaryZImg(��ֵ���������ͼ��)
*	Anthor:������
*
***********************************************************************************/
void CarPlateRecog::DoDenois(IplImage *pBinaryZImg)
{
	if (!pBinaryZImg)
	{
		return;
	}

	IplImage *pTemp = cvCreateImage(cvGetSize(pBinaryZImg), IPL_DEPTH_8U, 1);
	cvCopyImage(pBinaryZImg, pTemp);
	m_denoisImg = pTemp;
	cvSmooth(m_denoisImg, m_denoisImg, CV_MEDIAN, 3, 3, 0, 0);
}

/***********************************************************************************
*
*	����������
*	Function Name:SortRectByXAsc
*	Date:2015/3/23
*	Parameter:vector<CvRect> rawPlateCharRects(������Ӿ���)
*	Anthor:��ϣ��
*
***********************************************************************************/
vector<CvRect> CarPlateRecog::SortRectByXAsc(vector<CvRect> rawPlateCharRects)
{
	vector<CvRect>::iterator itr = rawPlateCharRects.begin();//���ѭ��������
	vector<CvRect>::iterator innerItr;//�ڲ�ѭ��������
	vector<CvRect>::iterator minXRectItr;
	vector<CvRect> rects;
	while (itr != rawPlateCharRects.end())
	{
		minXRectItr = itr;
		for (innerItr = rawPlateCharRects.begin(); innerItr != rawPlateCharRects.end(); innerItr++)
		{
			if (innerItr->x < minXRectItr->x)
			{
				minXRectItr = innerItr;
			}
		}
		rects.push_back(*minXRectItr);
		rawPlateCharRects.erase(minXRectItr);
		itr = rawPlateCharRects.begin();
	}
	return rects;
}

/***********************************************************************************
*
*	������⺯��
*	Function Name:FindOutline
*	Date:2015/3/23
*	Parameter:IplImage* img(ȥ�봦����ͼ��)
*	Anthor:��ϣ��
*
***********************************************************************************/
vector<CvRect> CarPlateRecog::FindOutline(IplImage *pImg, bool fromBottom, bool fromTop)
{
	if (!pImg)
	{
		return NULL;
	}

	IplImage *pImgCopy = cvCreateImage(cvGetSize(pImg), IPL_DEPTH_8U, 1);
	cvCopyImage(pImg, pImgCopy);
	IplImage *pTempImg = cvCreateImage(cvGetSize(pImg), IPL_DEPTH_8U, 1);
	IplImage *pTempImg2 = cvCreateImage(cvGetSize(pImg), IPL_DEPTH_8U, 1);
	CvSeq* contours = NULL;
	CvMemStorage *pStorage = cvCreateMemStorage(0);
	vector<CvRect> outlineRects;
	CvRect tempRect = cvRect(0, 0, cvGetSize(pImg).width, cvGetSize(pImg).height);

	cvFindContours(pImgCopy, pStorage, &contours, sizeof(CvContour), CV_RETR_EXTERNAL);
	//������������
	for (CvSeq *pC = contours; pC != NULL; pC = pC->h_next)
	{
		CvRect rc = cvBoundingRect(pC, 0);//��������Ӿ���
		double rcArea = rc.height*rc.width;//��Ӿ��ε����

		bool withInArea = (rcArea<g_recogMaxRectArea) && (rcArea>g_recogMinRectArea);
		bool withInBound = (rc.x > g_leftBoundary) && (rc.x < g_rightBoundary);
		bool withInHeight = (rc.height > (double)g_plateImgHeight / 2);
		if (withInArea && withInBound && withInHeight)
		{
			outlineRects.push_back(rc);
		}
	}
	//����ҵ�������̫�٣�����Ϊ�Ǳ߿��Ӱ�졣��˽��߶�ÿ�μ�һ��ֱ���ҵ����������ڵ���4
	if (outlineRects.size() < 5 && fromBottom)
	{
		tempRect.height--;
		if (tempRect.height > ((double)g_plateImgHeight / 2))
		{
			cvCopyImage(pImgCopy, pTempImg);
			cvSetImageROI(pTempImg, tempRect);
			outlineRects = FindOutline(pTempImg, true, false);
			cvResetImageROI(pTempImg);
		}
		//���ͼ��ĸ��Ѿ�С��ԭ����1��˵��ͼ����б�Դ󣬴˷������������߿��Ӱ�죬���¼������Ϣ������
		else
		{
			return outlineRects;
		}
	}
	tempRect.height = cvGetSize(pImg).height;
	tempRect.y = 0;
	if (outlineRects.size() < 5 && fromTop)
	{
		tempRect.y++;
		tempRect.height--;
		if (tempRect.height > ((double)g_plateImgHeight / 2))
		{
			cvCopyImage(pImgCopy, pTempImg2);
			cvSetImageROI(pTempImg2, tempRect);
			outlineRects = FindOutline(pTempImg2, false, true);
			cvResetImageROI(pTempImg);
		}
		else
		{
			return outlineRects;
		}
	}

	cvReleaseMemStorage(&pStorage);
	cvReleaseImage(&pImgCopy);
	cvReleaseImage(&pTempImg);
	return outlineRects;
}

/***********************************************************************************
*
*	�����ַ����μ�⺯��
*	Function Name:FindPlateCharRects
*	Date:2015/3/23
*	Parameter:vector<CvRect> outlineRects(�������������������Ӿ���)
*	Anthor:��ϣ��
*
***********************************************************************************/
vector<CvRect> CarPlateRecog::FindPlateCharRects(vector<CvRect> outlineRects)
{
	if (outlineRects.size() < 5)
	{
		m_recogErrorEnum = SegmException;
		return NULL;
	}

	int count = 0;//�����ǳ����ַ����εľ��θ���
	int left = -1, right = -1;
	CvRect rc;
	vector<CvRect> plaCharRects;//�ַ�����vector
	vector<CvRect>::iterator itr;//�ڲ�ѭ��������
	vector<CvRect>::iterator innerItr;//���ѭ��������

	for (itr = outlineRects.begin(); itr != outlineRects.end(); itr++)
	{
		rc = *itr;
		for (innerItr = outlineRects.begin(); innerItr != outlineRects.end(); innerItr++)
		{
			bool heightInBound = (innerItr->height - rc.height<4) && (innerItr->height - rc.height>-4);
			if (heightInBound)
			{
				count++;
			}
		}
		if (count >= 3) //4
		{
			break;
		}
		count = 0;
	}

	//�������4��������Ϊû�ҵ�����¼������Ϣ
	if (count < 3) //4
	{
		m_recogErrorEnum = SegmException;
		return NULL;
	}

	for (itr = outlineRects.begin(); itr != outlineRects.end(); itr++)
	{
		bool heightInBound = (itr->height - rc.height<4) && (itr->height - rc.height>-4);
		if (heightInBound && (itr->x<g_rightBoundary) && (itr->x>g_leftBoundary))
		{
			//����ȫ�ֱ������±߽�Ϊ��Щ��������СYֵ�����Y��Heightֵ����ֹ����ص��߽����ͼ��ʱ�ص���Щ�ַ��Ĳ���
			if (itr->y < g_topBoundary)
			{
				g_topBoundary = itr->y;
			}
			if (itr->y + itr->height > g_bottomBoundary)
			{
				g_bottomBoundary = itr->y + itr->height;
			}
			plaCharRects.push_back(*itr);
		}
	}

	rc.x = 0;
	rc.width = g_plateImgWidth;

	//�ص����±߽����ͼ��,����
	rc.y = g_topBoundary;
	rc.height = g_bottomBoundary - g_topBoundary;
	cvSetImageROI(m_binaryZImg, rc);
	g_pQukuangImg = cvCreateImage(cvGetSize(m_binaryZImg), IPL_DEPTH_8U, 1);
	cvCopyImage(m_binaryZImg, g_pQukuangImg);
	cvResetImageROI(m_binaryZImg);
	return plaCharRects;
}

/***********************************************************************************
*
*	�ڶ���������⺯��
*	Function Name:FindOutlineSecond
*	Date:2015/3/23
*	Parameter:IplImage *pImg(ȥ��í���ͱ߿�ĳ���ͼ��)
*	Anthor:��ϣ��
*
***********************************************************************************/
vector<CvRect> CarPlateRecog::FindOutlineSecond(IplImage *pImg)
{
	IplImage *pImgCopy = cvCreateImage(cvGetSize(pImg), IPL_DEPTH_8U, 1);
	cvCopyImage(pImg, pImgCopy);
	IplImage *pTempImg = cvCreateImage(cvGetSize(pImg), IPL_DEPTH_8U, 1);
	CvSeq *pContours = NULL;
	CvMemStorage *pStorage = cvCreateMemStorage(0);
	vector<CvRect> outlineRects;

	cvFindContours(pImgCopy, pStorage, &pContours, sizeof(CvContour), CV_RETR_EXTERNAL);
	for (CvSeq *pC = pContours; pC != NULL; pC = pC->h_next)
	{
		CvRect rc = cvBoundingRect(pC, 0);
		double rcArea = rc.height*rc.width;

		bool withInArea = (rcArea < g_recogMaxRectArea) && (rcArea > g_recogMinRectArea);
		bool withInBound = (rc.x >= g_leftBoundary) && (rc.x <= g_rightBoundary);
		bool heightInBound = (g_bottomBoundary - g_topBoundary - rc.height <= 18);//���11,12
		if (withInArea && withInBound && heightInBound)
		{
			outlineRects.push_back(rc);
		}
	}

	cvReleaseImage(&pImgCopy);
	cvReleaseImage(&pTempImg);
	cvReleaseMemStorage(&pStorage);
	return outlineRects;
}

/***********************************************************************************
*
*	��֧������
*	Function Name:SwitchDeal
*	Date:2015/3/23
*	Parameter:vector<CvRect> plateCharRects(�����ַ�����)
*	Anthor:��ϣ��
*
***********************************************************************************/
void CarPlateRecog::SwitchDeal(vector<CvRect> plateCharRects)
{
	int avgHeight = 0;//�ַ����ε�ƽ���߶ȣ������������ַ����εĸ�
	int avgWidth = 0;//�ַ����ε�ƽ����ȣ������������ַ����εĿ�
	int firstY = -1;//�����ַ����ε�Yֵ
	vector<CvRect>::iterator itr;

	for (itr = plateCharRects.begin(); itr != plateCharRects.end(); itr++)
	{
		//��itr++����tempItr,��������itr++��Xֵ��itr��Xֵֻ��õ��ַ����μ��
		int temp = itr->x;
		itr++;
		vector<CvRect>::iterator tempItr = itr;
		itr--;

		if (tempItr != plateCharRects.end())
		{
			temp = tempItr->x - temp;//�ַ����μ��
			//����һ���ַ��������ڶ�����ε�Ӱ��
			if (temp < itr->width)
			{
				plateCharRects.erase(tempItr);
			}
		}

		if (plateCharRects.size() >= 7)
		{
			if (itr->x < 3 && (double)itr->height / itr->width>3)
			{
				plateCharRects.erase(itr);
				itr = plateCharRects.begin();
			}
			if (g_plateImgWidth - (itr->x + itr->width) < 3 && (double)itr->height / itr->width > 3.3)
			{
				plateCharRects.erase(itr);
				itr = plateCharRects.begin();
			}
		}
	}

	if(plateCharRects.size()>7)
	{
		itr=plateCharRects.begin();
		while(itr!=plateCharRects.end())
		{
			if((double)itr->height/itr->width>4)
			{
				plateCharRects.erase(itr);
				itr=plateCharRects.begin();
				continue;
			}
			else
			{
				break;
			}
		}
	}

	//����������ϳ����ַ����εľ��θ���Ϊ7������ɾ����һ��(�����ַ�)
	if (plateCharRects.size() == 7)
	{
		itr = plateCharRects.begin();
		firstY = itr->y;
		plateCharRects.erase(itr);
	}
	//����������ϳ����ַ����εľ��θ���Ϊ6��
	if (plateCharRects.size() == 6)
	{
		itr = plateCharRects.begin();
		for (itr = plateCharRects.begin(); itr != plateCharRects.end(); itr++)
		{
			double temp = double(itr->height) / 2;//���θ߶ȵ�һ��
			//������ַ�1�ľ���
			if (temp > 2 * itr->width)
			{
				itr->x = (int)(((double)(itr->x + (itr->x + itr->width)) / 2) - temp / 2) - 1;
				itr->width = (int)temp + 2;
				itr->y = itr->y - 1;
				itr->height = itr->height + 2;
			}
			avgHeight += itr->height;
			avgWidth += itr->width;
		}
		avgHeight = (double)avgHeight / 6;
		avgWidth = (double)avgWidth / 6;

		int count = 0;
		double temp = 0;
		//����󼸸��ַ����ε�ƽ�����
		for (itr = plateCharRects.begin(); itr != plateCharRects.end(); itr++)
		{
			if (itr->x > (4.5*itr->width))
			{
				count++;
				vector<CvRect>::iterator tempItr;
				tempItr = itr;
				tempItr--;
				temp += itr->x - tempItr->x;
			}
		}
		temp = temp / count;

		itr = plateCharRects.begin();
		if (firstY < 0)
		{
			firstY = itr->y;
		}
		CvRect rc = cvRect(itr->x - temp, firstY, avgWidth, avgHeight);//
		plateCharRects.push_back(rc);
	}// end if() �������ϳ����ַ����εľ��θ���Ϊ6���Ĵ������
	
	else
	{
		m_recogErrorEnum = SegmException;
		return;
	}

	char wndName[10] = { 0 };//�����ַ�ͼ����
	int count = 0;//���ڳ����ַ�ͼ�����У���ʾ��count+1��

	plateCharRects = SortRectByXAsc(plateCharRects);

	for (itr = plateCharRects.begin(); itr != plateCharRects.end(); itr++)
	{
		count++;
		sprintf(wndName, "temp\\wnd_%d.bmp", count);

		//�и���ַ���������
		cvSetImageROI(g_pQukuangImg, *itr);
		IplImage *pTemp = cvCreateImage(cvGetSize(g_pQukuangImg), IPL_DEPTH_8U, 1);
		IplImage *pTemp2 = cvCreateImage(cvSize(20, 40), IPL_DEPTH_8U, 1);
		cvCopyImage(g_pQukuangImg, pTemp);
		cvResize(pTemp, pTemp2, CV_INTER_AREA);
		cvSaveImage(wndName, pTemp2);
		m_plateCharImgs.push_back(pTemp2);
	}
}

/***********************************************************************************
*
*	�����ַ��ָ��
*	Function Name:SegmPlateChar
*	Date:2015/3/23
*	Parameter:IplImage *pPlateImg(����ԭͼ��)
*	Anthor:��ϣ��
*
***********************************************************************************/
void CarPlateRecog::SegmPlateChar(IplImage *pPlateImg)
{
	if (!pPlateImg)
	{
		return;
	}

	DoGray(pPlateImg);
	DoBinaryZ(m_grayImg);
	g_plateCharRects = FindOutline(m_binaryZImg, true, true);
	if (m_recogErrorEnum == SegmException)
	{
		return;
	}
	g_plateCharRects = FindPlateCharRects(g_plateCharRects);
	if (m_recogErrorEnum == SegmException)
	{
		return;
	}
	g_plateCharRects = FindOutlineSecond(g_pQukuangImg);
	g_plateCharRects = SortRectByXAsc(g_plateCharRects);

	SwitchDeal(g_plateCharRects);
}

/***********************************************************************************
*
*	�����ַ�ʶ����
*	Function Name:PlateCharRecog
*	Date:2015/3/23
*	Parameter:vector<IplImage*> plateCharImgs(�����ַ�ͼ��)
*	Anthor:������
*
***********************************************************************************/
void CarPlateRecog::PlateCharRecog(vector<IplImage*> plateCharImgs)
{
	if (plateCharImgs.size() == 0)
	{
		return;
	}

	vector<IplImage*>::iterator itr;
	int count = 0;
	m_plateCharInfo = "";//�ַ���Ϣ
	m_avgBaiFenBi = 0;
	//�������еĳ����ַ�ͼƬ
	for (itr = plateCharImgs.begin(); itr != plateCharImgs.end(); itr++)
	{
		count++;
		CString dir;
		//��һ���ַ�Ϊ������
		if (count == 1)
		{
			dir = "ģ��\\����";
		}
		//�ڶ����ַ�Ϊ��ĸ
		else if (count == 2)
		{
			dir = "ģ��\\��ĸ";
		}
		//���漸���ַ���������ĸ������������
		else
		{
			dir = "ģ��\\����";
		}
		m_plateCharInfo += ProcPicDir(dir, *itr);
	}

	m_avgBaiFenBi = m_avgBaiFenBi / 7;//ƽ��ʶ����

}

/***********************************************************************************
*
*	ģ��ƥ�亯��
*	Function Name:ProcPicDir
*	Date:2015/3/23
*	Parameter:CString strPicDir,IplImage *pSrcImg
*	Anthor:������
*
***********************************************************************************/
CString CarPlateRecog::ProcPicDir(CString strPicDir, IplImage *pSrcImg)
{
	int countSamePix = 0;//��ͬ���صĸ���
	double maxBaiFenBi = 0;//���ٷֱ�
	CFileFind fileFinder;//�ļ���������
	CString plateChar;//�����ַ�
	IplImage *pMoban;//�ַ�ģ��ͼ��

	if (strPicDir.Right(1) == TEXT("\\"))
	{
		int nPos = strPicDir.ReverseFind(TEXT('\\'));
		strPicDir = strPicDir.Left(nPos);
	}

	CString strPicFile = TEXT("");
	strPicFile.Format(TEXT("%s\\%s"), strPicDir, TEXT("*.*"));

	BOOL bWorking = fileFinder.FindFile(strPicFile);
	while (bWorking)
	{
		bWorking = fileFinder.FindNextFile();
		if (fileFinder.IsDots())
		{
			continue;
		}

		CString strFilePath = fileFinder.GetFilePath();//ģ���ַ�ͼ��·��
		string abc = strFilePath.GetBuffer(0);//ģ���ַ�ͼ��·��

		if (fileFinder.IsDirectory())
		{
			//��������Ŀ¼
			ProcPicDir(strFilePath, pSrcImg);
		}
		else
		{
			int nPos = strFilePath.Find(TEXT('-'), 0);
			CString strExt = strFilePath.Right(strFilePath.GetLength() - nPos);
			char fileName[200] = { 0 };
			abc.copy(fileName, abc.size(), 0);

			string postfix=strExt.Mid(strExt.Find(TEXT('.'))+1).GetBuffer(0);
			if(postfix.compare("bmp")==0)
			{
				pMoban = cvLoadImage(fileName, CV_LOAD_IMAGE_ANYCOLOR);
				
				for (int i = 0; i < g_mobanHeight; i++)
				{
					for (int j = 0; j < g_mobanWidth; j++)
					{
						if (cvGet2D(pSrcImg, i, j).val[0] == cvGet2D(pMoban, i, j).val[0])
						{
							countSamePix++;
						}
					}
				}
				if (((double)countSamePix / (g_mobanHeight*g_mobanWidth)) > maxBaiFenBi)
				{
					maxBaiFenBi = (double)countSamePix / (g_mobanHeight*g_mobanWidth);
					int leftIndex = strFilePath.Find(TEXT('='), 0);
					plateChar = strFilePath.Mid(nPos + 1, leftIndex - nPos - 1);
				}
				countSamePix = 0;
			}
		}// end else()
	}// end while()

	fileFinder.Close();
	m_avgBaiFenBi += maxBaiFenBi;
	return plateChar;
}

/*���صĸ�ֵ����*/
void CarPlateRecog::operator = (CarPlateRecog & carPlateRecog)
{
	m_carPlateImg = carPlateRecog.GetCarPlateImg();
	m_grayImg = carPlateRecog.GetGrayImg();
	m_binaryZImg = carPlateRecog.GetBinaryZImg();
	m_denoisImg = carPlateRecog.GetDenoisImg();
	m_plateCharImgs = carPlateRecog.GetPlateCharImgs();
	m_plateCharInfo = carPlateRecog.GetPlateChars();
	m_recogErrorEnum = carPlateRecog.GetErrorEnum();
	m_avgBaiFenBi = carPlateRecog.GetAvgBaiFenBi();
}

IplImage* CarPlateRecog::GetCarPlateImg()
{
	return m_carPlateImg;
}

void CarPlateRecog::SetCarPlateImg(IplImage *pImg)
{
	m_carPlateImg = pImg;
}

vector<IplImage*> CarPlateRecog::GetPlateCharImgs()
{
	return m_plateCharImgs;
}

void CarPlateRecog::SetPlateCharImgs(vector<IplImage*> imgs)
{
	m_plateCharImgs = imgs;
}

IplImage* CarPlateRecog::GetGrayImg()
{
	return m_grayImg;
}

IplImage* CarPlateRecog::GetBinaryZImg()
{
	return m_binaryZImg;
}

IplImage* CarPlateRecog::GetDenoisImg()
{
	return m_denoisImg;
}

CString CarPlateRecog::GetPlateChars()
{
	return m_plateCharInfo;
}

RecogErrorEnum CarPlateRecog::GetErrorEnum()
{
	return m_recogErrorEnum;
}

double CarPlateRecog::GetAvgBaiFenBi()
{
	return m_avgBaiFenBi;
}