/*****************************************************************************************************
*
*	File Name : CarPlateRecog.cpp
*	Created : 2015/3/22
*	Author : 陈希明，胡欣茹
*	Description:车牌识别类的定义文件，包含了对车牌识别类成员函数的定义
*
******************************************************************************************************/

#include "stdafx.h"
#include "CarPlateRecog.h"
#include <iostream>

//全局变量
static double g_recogSrcImgArea;//资源图像的面积
static double g_recogMinRectArea;//矩形最小面积，用于除去面积过小的矩形
static double g_recogMaxRectArea;//矩形最大面积，用于除去面积过大的矩形
static int g_plateImgHeight;//图像的高度
static int g_plateImgWidth;//图像的宽度
static int g_topBoundary = 20;//上边界，截掉车牌图像上边界上面的部分(去除铆钉和边框)
static int g_bottomBoundary = 0; //下边界，截掉车牌图像下边界下面的部分(去除边框)
static double g_leftBoundary;//左边界，截掉车牌图像左边界左边的部分(防止左边框被误认为是车牌字符1)
static double g_rightBoundary;// //右边界，截掉车牌图像右边界右边的部分(防止右边框被误认为是车牌字符1)
static const int g_heightBuf = 4;//高度范围,在寻找高度相似的矩形作为车牌字符矩形时用作高度限制。
static const int g_mobanHeight = 40;//字符模板图像的高
static const int g_mobanWidth = 20;//字符模板图像的宽
static vector<CvRect> g_plateCharRects;//车牌字符矩形
static IplImage* g_pQukuangImg;//去除边框后的车牌图像

/*默认构造函数*/
CarPlateRecog::CarPlateRecog()
{
	//成员变量赋值
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

/*构造函数*/
CarPlateRecog::CarPlateRecog(IplImage *pPlateImg)
{
	if (!pPlateImg)
	{
		m_carPlateImg = NULL;
		return;
	}

	//成员变量赋值
	m_carPlateImg = cvCreateImage(cvGetSize(pPlateImg), 8, 3);
	cvCopyImage(pPlateImg, m_carPlateImg);
	m_grayImg = NULL;
	m_binaryZImg = NULL;
	m_denoisImg = NULL;
	m_plateCharInfo = "";
	m_recogErrorEnum = RecogNormal;
	m_avgBaiFenBi = 0;

	//全局变量赋值
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

/*清楚数据函数*/
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
*	灰度化处理函数
*	Function Name:DoGray
*	Date:2015/3/23
*	Parameter:IplImage *pPlateImg(资源图像)
*	Anthor:胡欣茹
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
*	二值化处理函数
*	Function Name:DoGray
*	Date:2015/3/23
*	Parameter:IplImage *pGrayImg(灰度化处理过的图像)
*	Anthor:胡欣茹
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
*	去噪函数
*	Function Name:DoGray
*	Date:2015/3/23
*	Parameter:IplImage *pBinaryZImg(二值化处理过的图像)
*	Anthor:胡欣茹
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
*	矩形排序函数
*	Function Name:SortRectByXAsc
*	Date:2015/3/23
*	Parameter:vector<CvRect> rawPlateCharRects(轮廓外接矩形)
*	Anthor:陈希明
*
***********************************************************************************/
vector<CvRect> CarPlateRecog::SortRectByXAsc(vector<CvRect> rawPlateCharRects)
{
	vector<CvRect>::iterator itr = rawPlateCharRects.begin();//外层循环迭代器
	vector<CvRect>::iterator innerItr;//内层循环迭代器
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
*	轮廓检测函数
*	Function Name:FindOutline
*	Date:2015/3/23
*	Parameter:IplImage* img(去噪处理后的图像)
*	Anthor:陈希明
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
	//遍历所有轮廓
	for (CvSeq *pC = contours; pC != NULL; pC = pC->h_next)
	{
		CvRect rc = cvBoundingRect(pC, 0);//轮廓的外接矩形
		double rcArea = rc.height*rc.width;//外接矩形的面积

		bool withInArea = (rcArea<g_recogMaxRectArea) && (rcArea>g_recogMinRectArea);
		bool withInBound = (rc.x > g_leftBoundary) && (rc.x < g_rightBoundary);
		bool withInHeight = (rc.height > (double)g_plateImgHeight / 2);
		if (withInArea && withInBound && withInHeight)
		{
			outlineRects.push_back(rc);
		}
	}
	//如果找到的轮廓太少，则认为是边框的影响。因此将高度每次减一，直到找到轮廓数大于等于4
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
		//如果图像的高已经小于原来的1半说明图像倾斜略大，此方法难以消除边框的影响，则记录错误信息，返回
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
*	车牌字符矩形检测函数
*	Function Name:FindPlateCharRects
*	Date:2015/3/23
*	Parameter:vector<CvRect> outlineRects(初次轮廓检测后的轮廓外接矩形)
*	Anthor:陈希明
*
***********************************************************************************/
vector<CvRect> CarPlateRecog::FindPlateCharRects(vector<CvRect> outlineRects)
{
	if (outlineRects.size() < 5)
	{
		m_recogErrorEnum = SegmException;
		return NULL;
	}

	int count = 0;//可能是车牌字符矩形的矩形个数
	int left = -1, right = -1;
	CvRect rc;
	vector<CvRect> plaCharRects;//字符矩形vector
	vector<CvRect>::iterator itr;//内层循环迭代器
	vector<CvRect>::iterator innerItr;//外层循环迭代器

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

	//如果不到4个，则认为没找到，记录错误信息
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
			//更新全局变量上下边界为这些矩形中最小Y值和最大Y加Height值，防止后面截掉边界外的图像时截掉这些字符的部分
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

	//截掉上下边界外的图像,保存
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
*	第二次轮廓检测函数
*	Function Name:FindOutlineSecond
*	Date:2015/3/23
*	Parameter:IplImage *pImg(去掉铆钉和边框的车牌图像)
*	Anthor:陈希明
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
		bool heightInBound = (g_bottomBoundary - g_topBoundary - rc.height <= 18);//最后11,12
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
*	分支处理函数
*	Function Name:SwitchDeal
*	Date:2015/3/23
*	Parameter:vector<CvRect> plateCharRects(车牌字符矩形)
*	Anthor:陈希明
*
***********************************************************************************/
void CarPlateRecog::SwitchDeal(vector<CvRect> plateCharRects)
{
	int avgHeight = 0;//字符矩形的平均高度，用来做中文字符矩形的高
	int avgWidth = 0;//字符矩形的平均宽度，用来做中文字符矩形的宽
	int firstY = -1;//中文字符矩形的Y值
	vector<CvRect>::iterator itr;

	for (itr = plateCharRects.begin(); itr != plateCharRects.end(); itr++)
	{
		//将itr++赋给tempItr,用来计算itr++的X值与itr的X值只差，得到字符矩形间距
		int temp = itr->x;
		itr++;
		vector<CvRect>::iterator tempItr = itr;
		itr--;

		if (tempItr != plateCharRects.end())
		{
			temp = tempItr->x - temp;//字符矩形间距
			//消除一个字符附近存在多个矩形的影响
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

	//如果检测出符合车牌字符矩形的矩形个数为7个，则删除第一个(中文字符)
	if (plateCharRects.size() == 7)
	{
		itr = plateCharRects.begin();
		firstY = itr->y;
		plateCharRects.erase(itr);
	}
	//如果检测出符合车牌字符矩形的矩形个数为6个
	if (plateCharRects.size() == 6)
	{
		itr = plateCharRects.begin();
		for (itr = plateCharRects.begin(); itr != plateCharRects.end(); itr++)
		{
			double temp = double(itr->height) / 2;//矩形高度的一半
			//如果是字符1的矩形
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
		//计算后几个字符矩形的平均间距
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
	}// end if() 检测出符合车牌字符矩形的矩形个数为6个的处理结束
	
	else
	{
		m_recogErrorEnum = SegmException;
		return;
	}

	char wndName[10] = { 0 };//车牌字符图像名
	int count = 0;//用在车牌字符图像名中，表示第count+1张

	plateCharRects = SortRectByXAsc(plateCharRects);

	for (itr = plateCharRects.begin(); itr != plateCharRects.end(); itr++)
	{
		count++;
		sprintf(wndName, "temp\\wnd_%d.bmp", count);

		//切割车牌字符，并保存
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
*	车牌字符分割函数
*	Function Name:SegmPlateChar
*	Date:2015/3/23
*	Parameter:IplImage *pPlateImg(车牌原图像)
*	Anthor:陈希明
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
*	车牌字符识别函数
*	Function Name:PlateCharRecog
*	Date:2015/3/23
*	Parameter:vector<IplImage*> plateCharImgs(车牌字符图像)
*	Anthor:胡欣茹
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
	m_plateCharInfo = "";//字符信息
	m_avgBaiFenBi = 0;
	//遍历所有的车牌字符图片
	for (itr = plateCharImgs.begin(); itr != plateCharImgs.end(); itr++)
	{
		count++;
		CString dir;
		//第一个字符为中文字
		if (count == 1)
		{
			dir = "模板\\汉字";
		}
		//第二个字符为字母
		else if (count == 2)
		{
			dir = "模板\\字母";
		}
		//后面几个字符可能是字母，可能是数字
		else
		{
			dir = "模板\\其它";
		}
		m_plateCharInfo += ProcPicDir(dir, *itr);
	}

	m_avgBaiFenBi = m_avgBaiFenBi / 7;//平均识别率

}

/***********************************************************************************
*
*	模板匹配函数
*	Function Name:ProcPicDir
*	Date:2015/3/23
*	Parameter:CString strPicDir,IplImage *pSrcImg
*	Anthor:胡欣茹
*
***********************************************************************************/
CString CarPlateRecog::ProcPicDir(CString strPicDir, IplImage *pSrcImg)
{
	int countSamePix = 0;//相同像素的个数
	double maxBaiFenBi = 0;//最大百分比
	CFileFind fileFinder;//文件搜索对象
	CString plateChar;//车牌字符
	IplImage *pMoban;//字符模板图像

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

		CString strFilePath = fileFinder.GetFilePath();//模板字符图像路径
		string abc = strFilePath.GetBuffer(0);//模板字符图像路径

		if (fileFinder.IsDirectory())
		{
			//继续遍历目录
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

/*重载的赋值函数*/
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