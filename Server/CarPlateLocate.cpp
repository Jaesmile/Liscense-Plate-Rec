/*****************************************************************************************************
*
*	File Name : CarPlateLocate.cpp
*	Created : 2015/3/22
*	Author : 陈希明，胡欣茹
*	Description:车牌定位类的定义文件，包含了车牌定位类的成员函数的定义
*
******************************************************************************************************/

#include "stdafx.h"
#include "CarPlateLocate.h"

//全局变量
static double g_srcImgArea;//资源图像的面积
static int g_srcImgHeight;//资源图像的高度
static double g_minRectArea;//矩形最小面积，用于除去面积过小的矩形
static double g_maxRectArea;//矩形最大面积，用于除去面积过大的矩形
static double g_leftBound;//左边界，用于除去左边界左边的矩形
static double g_rightBound;//右边界，用于除去右边界右边的矩形
static vector<CvRect> g_sampleRects;//样本矩形
static const double MIN_WH_RATIO = 2.5;//最小宽高比，用于车牌矩形筛选
static const double MAX_WH_RATIO = 5.9;//最大宽高比，用于车牌矩形筛选
static const double TOP_BOUNDARY = 0.2;//资源图像顶部的20%区域视为非车牌区域

/*默认构造函数*/
CarPlateLocate::CarPlateLocate()
{
	//成员变量赋值
	m_srcImgName = NULL;
	m_srcImg = NULL;
	m_carPlateImg = NULL;
	m_locateError = Normal;//默认能正常定位
}

/*构造函数*/
CarPlateLocate::CarPlateLocate(char *pFilename)
{
	string fileName = pFilename;//文件名
	string postFix;//文件后缀名
	int dotPosition = fileName.find('.');//字符'.'的位置

	//如果未找到'.'，则记录错误信息并返回
	if (dotPosition == string::npos)
	{
		m_locateError = IOException;
		return;
	}
	else
	{
		postFix = fileName.substr(dotPosition + 1);
		//如果后缀不是"bmp","jpg"。则记录错误信息并返回
		if (postFix.compare("bmp") != 0 && postFix.compare("jpg") != 0)
		{
			m_locateError = FileTypeException;
			return;
		}
		else
		{
			//成员变量赋值
			m_srcImgName = pFilename;
			m_carPlateImg = NULL;
			m_locateError = Normal;//默认能正常定位

			m_srcImg = cvLoadImage(m_srcImgName, CV_LOAD_IMAGE_COLOR);//加载资源图像

			//全局变量赋值
			g_srcImgArea = cvGetSize(m_srcImg).height*cvGetSize(m_srcImg).width;//图像面积
			g_srcImgHeight = cvGetSize(m_srcImg).height;//图像的高
			g_minRectArea = g_srcImgArea / 700;//矩形最小面积，用于除去面积过小的矩形
			g_maxRectArea = g_srcImgArea / 10;//矩形最大面积，用于除去面积过大的矩形
			g_leftBound = cvGetSize(m_srcImg).width*0.10;//左边界，用于除去左边界左边的矩形
			g_rightBound = cvGetSize(m_srcImg).width*0.90;//右边界，用于除去右边界右边的矩形
		}
	}
}

/*清楚数据函数*/
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
*	图像预处理函数
*	Function Name:ImgPreProcess
*	Date:2015/3/23
*	Parameter:IplImage *pSrcImg(资源图像)
*	Anthor:胡欣茹
*
***********************************************************************************/
IplImage* CarPlateLocate::ImgPreProcess(IplImage *pSrcImg)
{
	if (!pSrcImg)
	{
		return NULL;
	}

	//灰度化处理
	IplImage *pImgGray = cvCreateImage(cvGetSize(pSrcImg), IPL_DEPTH_8U, 1);
	cvCvtColor(pSrcImg, pImgGray, CV_BGR2GRAY);

	//索贝尔边缘检测
	IplImage *pSobel = cvCreateImage(cvGetSize(pImgGray), IPL_DEPTH_16S, 1);
	cvSobel(pImgGray, pSobel, 2, 0, 7);
	IplImage *pTemp = cvCreateImage(cvGetSize(pSobel), IPL_DEPTH_8U, 1);
	cvConvertScale(pSobel, pTemp, 0.00390625, 0);

	//二值化处理
	IplImage *pThreshold = cvCreateImage(cvGetSize(pTemp), IPL_DEPTH_8U, 1);
	cvThreshold(pTemp, pThreshold, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);

	//形态学处理,x方向上的处理
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
*	轮廓检测函数
*	Function Name:FindOutline
*	Date:2015/3/23
*	Parameter:IplImage *pImg(图像预处理后的图像资源)
*	Anthor:陈希明
*
***********************************************************************************/
vector<CvRect> CarPlateLocate::FindOutline(IplImage *pImg)
{
	if (!pImg)
	{
		return NULL;
	}

	IplImage *pCopy = cvCloneImage(pImg);//原图像的拷贝，防止对原图像造成影响
	//轮廓检测
	CvMemStorage *pStorage = cvCreateMemStorage();
	CvSeq *pContours;
	CvRect rc;
	vector<CvRect> rects;
	cvFindContours(pCopy, pStorage, &pContours);
	//遍历所有轮廓
	while (pContours != NULL)
	{
		rc = cvBoundingRect(pContours);//轮廓的外接矩形
		double rcAre = rc.height*rc.width;//轮廓外接矩形的面积

		//用面积和边界筛选掉部分轮廓的外接矩形
		bool withInArea = (rcAre > g_minRectArea) && (rcAre < g_maxRectArea);
		bool withInBound = (rc.x > g_leftBound) && (rc.x + rc.width < g_rightBound) && (rc.y > TOP_BOUNDARY*g_srcImgHeight);
		if (withInArea && withInBound)
		{
			rects.push_back(cvBoundingRect(pContours));//BoundingRect:连通域外围矩形
		}
		pContours = pContours->h_next;
	}

	cvReleaseImage(&pCopy);
	return rects;
}

/*********************************************************************************
*
*	连通矩形检测处理函数
*	Function Name:FindConnectDomain
*	Date:2015/3/23
*	Parameter:vector<CvRect> *pSampleRects(轮廓检测得到的轮廓外接矩形)
*	Anthor:陈希明
*
***********************************************************************************/
bool CarPlateLocate::FindConnectDomain(vector<CvRect> *pSampleRects)
{
	if (!pSampleRects)
	{
		return false;
	}

	vector<CvRect>::iterator itr;//外层循环迭代器
	vector<CvRect>::iterator innerItr;//内层循环迭代器
	vector<CvRect> rectsCopy = *pSampleRects;
	pSampleRects->clear();//清空原样本矩形vector中的矩形
	CvRect rc2;
	bool retFound = false;//是否找到需要连通处理的矩形。用作返回值

	for (itr = rectsCopy.begin(); itr != rectsCopy.end(); itr++)
	{
		innerItr = itr;
		rc2 = *itr;
		bool find = false;//针对具体的一个矩形是否找到需要与它进行连通处理的矩形，若未找到则添加回原样本矩形中。
		for (innerItr++; innerItr != rectsCopy.end(); innerItr++)
		{
			//在右边
			bool onRight = (innerItr->x > itr->x);
			//在右边较近范围内
			bool rightInBound = (innerItr->x >= (itr->x + itr->width) && (innerItr->x + innerItr->width - itr->x - (itr->width + innerItr->width) <= 10))
				|| ((innerItr->x <= (itr->x + itr->width)) && (innerItr->x + innerItr->width > (itr->x + itr->width)));
			//在左边
			bool onLeft = (itr->x > innerItr->x);
			//在左边较近范围内
			bool leftInBound = (itr->x >= (innerItr->x + innerItr->width) && (itr->x + itr->width - innerItr->x - (innerItr->width + itr->width) <= 10))
				|| ((itr->x <= (innerItr->x + innerItr->width)) && (itr->x + itr->width > (innerItr->x + innerItr->width)));
			//矩形中心的Y值相差不大
			bool sameY = (innerItr->y + (double)innerItr->height / 2 - itr->y - (double)itr->height / 2 <= 8 &&
				innerItr->y + (double)innerItr->height / 2 - itr->y - (double)itr->height / 2 >= -8);
			//矩形的高度相差不大
			bool sameHeight = (innerItr->height - itr->height<12 && innerItr->height - itr->height>-12);

			if (((onRight&&rightInBound) || (onLeft &&leftInBound)) && sameY && sameHeight)
			{
				//连接两个矩形
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
				//将连接后的矩形添加到原样本矩形vector中
				pSampleRects->push_back(rc2);
				rectsCopy.erase(innerItr);
				find = true;
				retFound = true;
				break;
			}
		}//end for(...) 内部循环结束
		if (find == false)
		{
			pSampleRects->push_back(rc2);
		}
	}//end for(...) 外部循环结束

	return retFound;
}

/*********************************************************************************
*
*	车牌矩形筛选函数
*	Function Name:SelectPlateRect
*	Date:2015/3/23
*	Parameter:vector<CvRect> sampleRects
*	Anthor:陈希明
*
***********************************************************************************/
CvRect CarPlateLocate::SelectPlateRect(vector<CvRect> sampleRects)
{
	vector<CvRect> lastRects = sampleRects;
	sampleRects.clear();
	vector<CvRect>::iterator itr;
	CvRect rc;
	g_minRectArea = g_srcImgArea / 200; //最小面积矩形改为原图像面积的1/200
	g_maxRectArea = g_srcImgArea / 10; //最大面积矩形改为原图像面积的1/10

	for (itr = lastRects.begin(); itr != lastRects.end(); itr++)
	{
		rc = *itr;
		double WHRatio = (double)rc.width / rc.height;//宽高比
		double rectArea = rc.width*rc.height;

		//筛选出在面积范围内，边界范围内，宽高范围内的矩形
		bool withInArea = (rectArea > g_minRectArea) && (rectArea < g_maxRectArea);
		bool withInBound = (rc.y > g_srcImgHeight*TOP_BOUNDARY);
		bool withInWHRatio = (WHRatio > MIN_WH_RATIO) && (WHRatio < MAX_WH_RATIO);
		if (withInArea && withInBound && withInWHRatio)
		{
			sampleRects.push_back(rc);
		}
	}

	//若筛选出的矩形只有1个，视为车牌
	if (sampleRects.size() == 1)
	{
		rc = *sampleRects.begin();
	}
	//若筛选出的矩形大于1个，这做最后一步筛选
	else if (sampleRects.size() > 1)
	{
		rc = lastSelect(sampleRects, m_srcImg);
	}
	//若没有筛选出符合条件的矩形，记录错误信息
	else if (sampleRects.size() == 0)
	{
		m_locateError = PositionException;
	}

	return rc;
}

/*********************************************************************************
*
*	车牌定位函数
*	Function Name:LocateCarPlate
*	Date:2015/3/23
*	Parameter:IplImage *pSrcImg
*	Anthor:陈希明
*
***********************************************************************************/
IplImage* CarPlateLocate::LocateCarPlate(IplImage *pSrcImg)
{
	if (!pSrcImg)
	{
		return NULL;
	}

	bool findConDomain = true;//默认存在需要连通处理的矩形
	CvRect carPlateRect;
	g_sampleRects = FindOutline(ImgPreProcess(pSrcImg));
	//直到所有需要连通处理的矩形都连通处理完
	while (findConDomain)
	{
		findConDomain = FindConnectDomain(&g_sampleRects);
	}
	carPlateRect = SelectPlateRect(g_sampleRects);
	if (m_locateError == PositionException)
	{
		return NULL;
	}

	//统一输出车牌图像的大小为200*60
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
*	最后一次筛选函数
*	Function Name:lastSelect
*	Date:2015/3/23
*	Parameter:vector<CvRect> plateRects,IplImage *pSrcImg
*	Anthor:陈希明
*
***********************************************************************************/
CvRect CarPlateLocate::lastSelect(vector<CvRect> plateRects, IplImage *pSrcImg)
{
	bool find = false;//在可能是车牌的几个矩形中找到矩形范围内存在蓝色像素区域的矩形
	CvRect plateRect;
	vector<CvRect>::iterator itr;
	//原图像的拷贝，防止函数对原图像造成影响
	IplImage *pSrcImgCopy = cvCreateImage(cvGetSize(pSrcImg), 8, 3);
	cvCopyImage(pSrcImg, pSrcImgCopy);
	double ratio = 0;//蓝色区域占矩形面积的比例
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
	//如果这些矩形范围内都不存在蓝色像素区，则记录错误信息
	if (find == false)
	{
		m_locateError = PositionException;
	}
	return plateRect;
}

/*********************************************************************************
*
*	蓝色像素区检测函数
*	Function Name:findBluePixArea
*	Date:2015/3/23
*	Parameter:IplImage *pSrc
*	Anthor:陈希明
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
	int count = 0;//蓝色像素的个数
	int height = cvGetSize(pSrc).height;
	int width = cvGetSize(pSrc).width;

	cvCvtColor(pSrc, pHSV, CV_BGR2HSV);
	cvSplit(pHSV, pHImg, pSImg, pVImg, NULL);
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			//判断是否为蓝色像素
			if (2 * cvGet2D(pHImg, i, j).val[0]>190 && 2 * cvGet2D(pHImg, i, j).val[0] < 245
				&& (double)cvGetReal2D(pSImg, i, j) / 255 > 0.35 && (double)cvGetReal2D(pVImg, i, j) / 255 > 0.3)
			{
				count++;
			}
		}
	}

	return (double)count / (height*width);
}

/*重载的赋值函数*/
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