/*****************************************************************************************************
*
*	File Name : CarPlateLocate.cpp
*	Created : 2015/3/22
*	Author : ��ϣ����������
*	Description:���ƶ�λ��Ķ����ļ��������˳��ƶ�λ��ĳ�Ա�����Ķ���
*
******************************************************************************************************/

#include "stdafx.h"
#include "CarPlateLocate.h"

//ȫ�ֱ���
static double g_srcImgArea;//��Դͼ������
static int g_srcImgHeight;//��Դͼ��ĸ߶�
static double g_minRectArea;//������С��������ڳ�ȥ�����С�ľ���
static double g_maxRectArea;//���������������ڳ�ȥ�������ľ���
static double g_leftBound;//��߽磬���ڳ�ȥ��߽���ߵľ���
static double g_rightBound;//�ұ߽磬���ڳ�ȥ�ұ߽��ұߵľ���
static vector<CvRect> g_sampleRects;//��������
static const double MIN_WH_RATIO = 2.5;//��С��߱ȣ����ڳ��ƾ���ɸѡ
static const double MAX_WH_RATIO = 5.9;//����߱ȣ����ڳ��ƾ���ɸѡ
static const double TOP_BOUNDARY = 0.2;//��Դͼ�񶥲���20%������Ϊ�ǳ�������

/*Ĭ�Ϲ��캯��*/
CarPlateLocate::CarPlateLocate()
{
	//��Ա������ֵ
	m_srcImgName = NULL;
	m_srcImg = NULL;
	m_carPlateImg = NULL;
	m_locateError = Normal;//Ĭ����������λ
}

/*���캯��*/
CarPlateLocate::CarPlateLocate(char *pFilename)
{
	string fileName = pFilename;//�ļ���
	string postFix;//�ļ���׺��
	int dotPosition = fileName.find('.');//�ַ�'.'��λ��

	//���δ�ҵ�'.'�����¼������Ϣ������
	if (dotPosition == string::npos)
	{
		m_locateError = IOException;
		return;
	}
	else
	{
		postFix = fileName.substr(dotPosition + 1);
		//�����׺����"bmp","jpg"�����¼������Ϣ������
		if (postFix.compare("bmp") != 0 && postFix.compare("jpg") != 0)
		{
			m_locateError = FileTypeException;
			return;
		}
		else
		{
			//��Ա������ֵ
			m_srcImgName = pFilename;
			m_carPlateImg = NULL;
			m_locateError = Normal;//Ĭ����������λ

			m_srcImg = cvLoadImage(m_srcImgName, CV_LOAD_IMAGE_COLOR);//������Դͼ��

			//ȫ�ֱ�����ֵ
			g_srcImgArea = cvGetSize(m_srcImg).height*cvGetSize(m_srcImg).width;//ͼ�����
			g_srcImgHeight = cvGetSize(m_srcImg).height;//ͼ��ĸ�
			g_minRectArea = g_srcImgArea / 700;//������С��������ڳ�ȥ�����С�ľ���
			g_maxRectArea = g_srcImgArea / 10;//���������������ڳ�ȥ�������ľ���
			g_leftBound = cvGetSize(m_srcImg).width*0.10;//��߽磬���ڳ�ȥ��߽���ߵľ���
			g_rightBound = cvGetSize(m_srcImg).width*0.90;//�ұ߽磬���ڳ�ȥ�ұ߽��ұߵľ���
		}
	}
}

/*������ݺ���*/
void CarPlateLocate::ClearData()
{
	cvReleaseImage(&m_srcImg);
	cvReleaseImage(&m_carPlateImg);

	m_srcImgName = NULL;
	m_srcImg = NULL;
	m_carPlateImg = NULL;
	m_locateError = Normal;
}

/*********************************************************************************
*
*	ͼ��Ԥ������
*	Function Name:ImgPreProcess
*	Date:2015/3/23
*	Parameter:IplImage *pSrcImg(��Դͼ��)
*	Anthor:������
*
***********************************************************************************/
IplImage* CarPlateLocate::ImgPreProcess(IplImage *pSrcImg)
{
	if (!pSrcImg)
	{
		return NULL;
	}

	//�ҶȻ�����
	IplImage *pImgGray = cvCreateImage(cvGetSize(pSrcImg), IPL_DEPTH_8U, 1);
	cvCvtColor(pSrcImg, pImgGray, CV_BGR2GRAY);

	//��������Ե���
	IplImage *pSobel = cvCreateImage(cvGetSize(pImgGray), IPL_DEPTH_16S, 1);
	cvSobel(pImgGray, pSobel, 2, 0, 7);
	IplImage *pTemp = cvCreateImage(cvGetSize(pSobel), IPL_DEPTH_8U, 1);
	cvConvertScale(pSobel, pTemp, 0.00390625, 0);

	//��ֵ������
	IplImage *pThreshold = cvCreateImage(cvGetSize(pTemp), IPL_DEPTH_8U, 1);
	cvThreshold(pTemp, pThreshold, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);

	//��̬ѧ����,x�����ϵĴ���
	IplConvKernel *pKernal = cvCreateStructuringElementEx(3, 1, 1, 0, CV_SHAPE_RECT);
	IplImage *pErode_dilate = cvCreateImage(cvGetSize(pThreshold), IPL_DEPTH_8U, 1);
	cvDilate(pThreshold, pErode_dilate, pKernal, 3);
	cvErode(pErode_dilate, pErode_dilate, pKernal, 5);
	cvDilate(pErode_dilate, pErode_dilate, pKernal, 2);

	cvReleaseImage(&pImgGray);
	cvReleaseImage(&pSobel);
	cvReleaseImage(&pTemp);
	cvReleaseImage(&pThreshold);
	return pErode_dilate;
}

/*********************************************************************************
*
*	������⺯��
*	Function Name:FindOutline
*	Date:2015/3/23
*	Parameter:IplImage *pImg(ͼ��Ԥ������ͼ����Դ)
*	Anthor:��ϣ��
*
***********************************************************************************/
vector<CvRect> CarPlateLocate::FindOutline(IplImage *pImg)
{
	if (!pImg)
	{
		return NULL;
	}

	IplImage *pCopy = cvCloneImage(pImg);//ԭͼ��Ŀ�������ֹ��ԭͼ�����Ӱ��
	//�������
	CvMemStorage *pStorage = cvCreateMemStorage();
	CvSeq *pContours;
	CvRect rc;
	vector<CvRect> rects;
	cvFindContours(pCopy, pStorage, &pContours);
	//������������
	while (pContours != NULL)
	{
		rc = cvBoundingRect(pContours);//��������Ӿ���
		double rcAre = rc.height*rc.width;//������Ӿ��ε����

		//������ͱ߽�ɸѡ��������������Ӿ���
		bool withInArea = (rcAre > g_minRectArea) && (rcAre < g_maxRectArea);
		bool withInBound = (rc.x > g_leftBound) && (rc.x + rc.width < g_rightBound) && (rc.y > TOP_BOUNDARY*g_srcImgHeight);
		if (withInArea && withInBound)
		{
			rects.push_back(cvBoundingRect(pContours));//BoundingRect:��ͨ����Χ����
		}
		pContours = pContours->h_next;
	}

	cvReleaseImage(&pCopy);
	return rects;
}

/*********************************************************************************
*
*	��ͨ���μ�⴦����
*	Function Name:FindConnectDomain
*	Date:2015/3/23
*	Parameter:vector<CvRect> *pSampleRects(�������õ���������Ӿ���)
*	Anthor:��ϣ��
*
***********************************************************************************/
bool CarPlateLocate::FindConnectDomain(vector<CvRect> *pSampleRects)
{
	if (!pSampleRects)
	{
		return false;
	}

	vector<CvRect>::iterator itr;//���ѭ��������
	vector<CvRect>::iterator innerItr;//�ڲ�ѭ��������
	vector<CvRect> rectsCopy = *pSampleRects;
	pSampleRects->clear();//���ԭ��������vector�еľ���
	CvRect rc2;
	bool retFound = false;//�Ƿ��ҵ���Ҫ��ͨ����ľ��Ρ���������ֵ

	for (itr = rectsCopy.begin(); itr != rectsCopy.end(); itr++)
	{
		innerItr = itr;
		rc2 = *itr;
		bool find = false;//��Ծ����һ�������Ƿ��ҵ���Ҫ����������ͨ����ľ��Σ���δ�ҵ�����ӻ�ԭ���������С�
		for (innerItr++; innerItr != rectsCopy.end(); innerItr++)
		{
			//���ұ�
			bool onRight = (innerItr->x > itr->x);
			//���ұ߽Ͻ���Χ��
			bool rightInBound = (innerItr->x >= (itr->x + itr->width) && (innerItr->x + innerItr->width - itr->x - (itr->width + innerItr->width) <= 10))
				|| ((innerItr->x <= (itr->x + itr->width)) && (innerItr->x + innerItr->width > (itr->x + itr->width)));
			//�����
			bool onLeft = (itr->x > innerItr->x);
			//����߽Ͻ���Χ��
			bool leftInBound = (itr->x >= (innerItr->x + innerItr->width) && (itr->x + itr->width - innerItr->x - (innerItr->width + itr->width) <= 10))
				|| ((itr->x <= (innerItr->x + innerItr->width)) && (itr->x + itr->width > (innerItr->x + innerItr->width)));
			//�������ĵ�Yֵ����
			bool sameY = (innerItr->y + (double)innerItr->height / 2 - itr->y - (double)itr->height / 2 <= 8 &&
				innerItr->y + (double)innerItr->height / 2 - itr->y - (double)itr->height / 2 >= -8);
			//���εĸ߶�����
			bool sameHeight = (innerItr->height - itr->height<12 && innerItr->height - itr->height>-12);

			if (((onRight&&rightInBound) || (onLeft &&leftInBound)) && sameY && sameHeight)
			{
				//������������
				if (onRight)
				{
					rc2.x = itr->x;
					rc2.width = innerItr->x - itr->x + innerItr->width;
				}
				else
				{
					rc2.x = innerItr->x;
					rc2.width = itr->x - innerItr->x + itr->width;
				}
				if (innerItr->y > itr->y)
				{
					rc2.y = itr->y;
				}
				else
				{
					rc2.y = innerItr->y;
				}
				if (innerItr->height > itr->height)
				{
					rc2.height = innerItr->height;
				}
				else
				{
					rc2.height = itr->height;
				}
				//�����Ӻ�ľ�����ӵ�ԭ��������vector��
				pSampleRects->push_back(rc2);
				rectsCopy.erase(innerItr);
				find = true;
				retFound = true;
				break;
			}
		}//end for(...) �ڲ�ѭ������
		if (find == false)
		{
			pSampleRects->push_back(rc2);
		}
	}//end for(...) �ⲿѭ������

	return retFound;
}

/*********************************************************************************
*
*	���ƾ���ɸѡ����
*	Function Name:SelectPlateRect
*	Date:2015/3/23
*	Parameter:vector<CvRect> sampleRects
*	Anthor:��ϣ��
*
***********************************************************************************/
CvRect CarPlateLocate::SelectPlateRect(vector<CvRect> sampleRects)
{
	vector<CvRect> lastRects = sampleRects;
	sampleRects.clear();
	vector<CvRect>::iterator itr;
	CvRect rc;
	g_minRectArea = g_srcImgArea / 200; //��С������θ�Ϊԭͼ�������1/200
	g_maxRectArea = g_srcImgArea / 10; //���������θ�Ϊԭͼ�������1/10

	for (itr = lastRects.begin(); itr != lastRects.end(); itr++)
	{
		rc = *itr;
		double WHRatio = (double)rc.width / rc.height;//��߱�
		double rectArea = rc.width*rc.height;

		//ɸѡ���������Χ�ڣ��߽緶Χ�ڣ���߷�Χ�ڵľ���
		bool withInArea = (rectArea > g_minRectArea) && (rectArea < g_maxRectArea);
		bool withInBound = (rc.y > g_srcImgHeight*TOP_BOUNDARY);
		bool withInWHRatio = (WHRatio > MIN_WH_RATIO) && (WHRatio < MAX_WH_RATIO);
		if (withInArea && withInBound && withInWHRatio)
		{
			sampleRects.push_back(rc);
		}
	}

	//��ɸѡ���ľ���ֻ��1������Ϊ����
	if (sampleRects.size() == 1)
	{
		rc = *sampleRects.begin();
	}
	//��ɸѡ���ľ��δ���1�����������һ��ɸѡ
	else if (sampleRects.size() > 1)
	{
		rc = lastSelect(sampleRects, m_srcImg);
	}
	//��û��ɸѡ�����������ľ��Σ���¼������Ϣ
	else if (sampleRects.size() == 0)
	{
		m_locateError = PositionException;
	}

	return rc;
}

/*********************************************************************************
*
*	���ƶ�λ����
*	Function Name:LocateCarPlate
*	Date:2015/3/23
*	Parameter:IplImage *pSrcImg
*	Anthor:��ϣ��
*
***********************************************************************************/
IplImage* CarPlateLocate::LocateCarPlate(IplImage *pSrcImg)
{
	if (!pSrcImg)
	{
		return NULL;
	}

	bool findConDomain = true;//Ĭ�ϴ�����Ҫ��ͨ����ľ���
	CvRect carPlateRect;
	g_sampleRects = FindOutline(ImgPreProcess(pSrcImg));
	//ֱ��������Ҫ��ͨ����ľ��ζ���ͨ������
	while (findConDomain)
	{
		findConDomain = FindConnectDomain(&g_sampleRects);
	}
	carPlateRect = SelectPlateRect(g_sampleRects);
	if (m_locateError == PositionException)
	{
		return NULL;
	}

	//ͳһ�������ͼ��Ĵ�СΪ200*60
	cvSetImageROI(pSrcImg, carPlateRect);
	IplImage *pPlateImg = cvCreateImage(cvGetSize(pSrcImg), IPL_DEPTH_8U, 3);
	IplImage *pResizePlateImg = cvCreateImage(cvSize(200, 60), IPL_DEPTH_8U, 3);
	cvCopyImage(pSrcImg, pPlateImg);
	cvResize(pPlateImg, pResizePlateImg, CV_INTER_LINEAR);
	cvResetImageROI(pSrcImg);

	cvReleaseImage(&pPlateImg);
	m_carPlateImg = pResizePlateImg;
	return pResizePlateImg;
}

/*********************************************************************************
*
*	���һ��ɸѡ����
*	Function Name:lastSelect
*	Date:2015/3/23
*	Parameter:vector<CvRect> plateRects,IplImage *pSrcImg
*	Anthor:��ϣ��
*
***********************************************************************************/
CvRect CarPlateLocate::lastSelect(vector<CvRect> plateRects, IplImage *pSrcImg)
{
	bool find = false;//�ڿ����ǳ��Ƶļ����������ҵ����η�Χ�ڴ�����ɫ��������ľ���
	CvRect plateRect;
	vector<CvRect>::iterator itr;
	//ԭͼ��Ŀ�������ֹ������ԭͼ�����Ӱ��
	IplImage *pSrcImgCopy = cvCreateImage(cvGetSize(pSrcImg), 8, 3);
	cvCopyImage(pSrcImg, pSrcImgCopy);
	double ratio = 0;//��ɫ����ռ��������ı���
	for (itr = plateRects.begin(); itr != plateRects.end(); itr++)
	{
		plateRect = *itr;
		cvSetImageROI(pSrcImgCopy, *itr);
		IplImage *pTemp = cvCreateImage(cvGetSize(pSrcImgCopy), 8, 3);
		cvCopyImage(pSrcImgCopy, pTemp);
		cvResetImageROI(pSrcImgCopy);
		ratio = findBluePixArea(pTemp);
		if (ratio > 0.1)
		{
			find = true;
			break;
		}
	}
	//�����Щ���η�Χ�ڶ���������ɫ�����������¼������Ϣ
	if (find == false)
	{
		m_locateError = PositionException;
	}
	return plateRect;
}

/*********************************************************************************
*
*	��ɫ��������⺯��
*	Function Name:findBluePixArea
*	Date:2015/3/23
*	Parameter:IplImage *pSrc
*	Anthor:��ϣ��
*
***********************************************************************************/
double CarPlateLocate::findBluePixArea(IplImage *pSrc)
{
	if (!pSrc)
	{
		return 0;
	}

	IplImage *pHSV = cvCreateImage(cvGetSize(pSrc), 8, 3);
	IplImage *pHImg = cvCreateImage(cvGetSize(pSrc), 8, 1);
	IplImage *pSImg = cvCreateImage(cvGetSize(pSrc), 8, 1);
	IplImage *pVImg = cvCreateImage(cvGetSize(pSrc), 8, 1);
	int count = 0;//��ɫ���صĸ���
	int height = cvGetSize(pSrc).height;
	int width = cvGetSize(pSrc).width;

	cvCvtColor(pSrc, pHSV, CV_BGR2HSV);
	cvSplit(pHSV, pHImg, pSImg, pVImg, NULL);
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			//�ж��Ƿ�Ϊ��ɫ����
			if (2 * cvGet2D(pHImg, i, j).val[0]>190 && 2 * cvGet2D(pHImg, i, j).val[0] < 245
				&& (double)cvGetReal2D(pSImg, i, j) / 255 > 0.35 && (double)cvGetReal2D(pVImg, i, j) / 255 > 0.3)
			{
				count++;
			}
		}
	}

	return (double)count / (height*width);
}

/*���صĸ�ֵ����*/
void CarPlateLocate::operator = (CarPlateLocate & carPlateLocate)
{
	m_srcImgName = carPlateLocate.GetSrcImgName();
	m_srcImg = carPlateLocate.GetSrcImg();
	m_carPlateImg = carPlateLocate.GetCarPlateImg();
	m_locateError = carPlateLocate.GetErrorEnum();
}

void CarPlateLocate::SetSrcImg(IplImage *pSrcImg)
{
	m_srcImg = pSrcImg;
}

IplImage* CarPlateLocate::GetSrcImg()
{
	return m_srcImg;
}

char* CarPlateLocate::GetSrcImgName()
{
	return m_srcImgName;
}

IplImage* CarPlateLocate::GetCarPlateImg()
{
	return m_carPlateImg;
}

LocateErrorEnum CarPlateLocate::GetErrorEnum()
{
	return m_locateError;
}